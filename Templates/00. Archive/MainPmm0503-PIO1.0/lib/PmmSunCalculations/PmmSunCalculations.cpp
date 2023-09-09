#include "PmmSunCalculations.h"
#include <PmmTimeLib.h>
#include <Wire.h>

using namespace std;

PmmSunCalculations::PmmSunCalculations(/* args */)
{
}

PmmSunCalculations::~PmmSunCalculations()
{
}

int16_t PmmSunCalculations::PMMReturnDayOfYear(int year, int month, int day)
{
    // Yazan way
    // boost::gregorian::date d(date.getYear(), date.getMonth(), date.getDay());
    // int16_t dayNumber = d.day_of_year();

    // My Way
    // Given a day, month, and year (4 digit), returns
    // the day of year. Errors return 999.

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Verify we got a 4-digit year
    if (year < 1000)
    {
        return 999;
    }

    // Check if it is a leap year, this is confusing business
    // See: https://support.microsoft.com/en-us/kb/214019

    if (year % 4 == 0)
    {
        if (year % 100 != 0)
        {
            daysInMonth[1] = 29;
        }
        else
        {
            if (year % 400 == 0)
            {
                daysInMonth[1] = 29;
            }
        }
    }

    // Make sure we are on a valid day of the month
    if (day < 1)
    {
        return 999;
    }
    else if (day > daysInMonth[month - 1])
    {
        return 999;
    }

    int doy = 0;
    for (int i = 0; i < month - 1; i++)
    {
        doy += daysInMonth[i];
    }

    doy += day;
    return doy;

    // return dayNumber;
}

time_t PmmSunCalculations::PMMSetDatetime(int year, int month, int day)
{
    tmElements_t tm;
    tm.Year = year;
    tm.Month = month;
    tm.Day = day;
    tm.Hour = 12;
    tm.Minute = 0;
    tm.Second = 0;

    time_t tSet = makeTime(tm);
    return tSet;
}

time_t PmmSunCalculations::PMMSetAnDatetime(int year, int month, int day, int hour, int minute, int second)
{
    tmElements_t tm;
    tm.Year = year;
    tm.Month = month;
    tm.Day = day;
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = second;

    time_t tSet = makeTime(tm);
    return tSet;
}

// UpdateCalculations

void PmmSunCalculations::UpdateCalculationsOLD(time_t date, float lat, float lang, int TimeZone, float TrackerWidth, float Post2Post)
{
    // TimeZone = 3, TrackerWidth = 4, Post2Post = 11;

    // #pragma region Equation of Time
    float tmpB = 360.0 / 365;

    DayOfYear = PMMReturnDayOfYear(year(date), month(date), day(date));

    B = (float)tmpB * (DayOfYear - 81);
    B = B * (M_PI / 180); // convert to Radians

    EOT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);

    // #pragma region Declination
    Declination = 23.45 * sin(B);

    // #pragma region Local Standard Time Meridian
    LSTM = 15 * TimeZone;

    // #pragma region Time Correction Factor(TC)
    TC = 4 * (lang - LSTM) + EOT;

    // #pragma region Local Solar Time(LST)

    // need to be edited
    time_t LST = date + (60 * TC);

    // #pragma region Hour Angle(HRA)

    time_t tmpLSTh = LST - 43200;

    HRA = 360 + ((float)((hour(tmpLSTh) * 60) + minute(LST) + ((float)(second(LST)) / 60)) * (float)15 / 60);

    // #pragma region HRA = HRA + 180;

    HRARAD = HRA * (M_PI / 180); // convert to Radians

    if (HRA > 180)
        HRA = HRA - 360;

    HRAZRAD = HRA * (M_PI / 180); // convert to Radians

    // #pragma region Elevation
    double latRAD = lat * M_PI / 180;                 // convert to Radians
    double DeclinationRAD = Declination * M_PI / 180; // convert to Radians
    double ElevationRAD = asin((sin(DeclinationRAD) * sin(latRAD)) + (cos(latRAD) * cos(DeclinationRAD) * cos(HRARAD)));

    Elevation = ElevationRAD * (180 / M_PI);

    // #pragma region Azimuth
    // AzimuthRAD = acos((sin(DeclinationRAD) * cos(latRAD) - cos(DeclinationRAD) * sin(latRAD) * cos(HRARAD)) / cos(ElevationRAD));

    // Azimuth = AzimuthRAD * RAD_TO_DEG;

    // if (Azimuth > 180)
    // {
    //     Azimuth = 360 - Azimuth;
    //     AzimuthRAD = Azimuth * M_PI / 180;
    // }

    AzimuthRAD = acos((sin(DeclinationRAD) * cos(latRAD) - cos(DeclinationRAD) * sin(latRAD) * cos(HRARAD)) / cos(ElevationRAD));
    Azimuth = AzimuthRAD * RAD_TO_DEG;
    // Calculate Zenith
    ZenithRAD = DEG_TO_RAD * (90 - Elevation);

    // #pragma region Sunrise and Sunset
    double tmp = (1.0 / 15) * acos(tan(DeclinationRAD) * tan(latRAD));
    tmp = tmp * 180 / M_PI;
    time_t dateSetRise = PMMSetDatetime((year(date) - 1970), month(date), day(date));
    // DateTime dateSetRise(date.getYear(), date.getMonth(), date.getDay(), 12, 0, 0);
    Sunset = dateSetRise + (43200);
    Sunrise = dateSetRise + (43200);
    Sunset = Sunset - (3600 * tmp);
    Sunrise = Sunrise + (3600 * tmp);
    Sunset = Sunset - (60 * TC);
    Sunrise = Sunrise - (60 * TC);
    Zenith = 90 - Elevation; // Bs

    double Ys = 180 - Azimuth; // Ys
    double Yt = 360;
    Ys = Ys * M_PI / 180;
    Yt = Yt * M_PI / 180;
    ZenithRAD = Zenith * M_PI / 180;

    int Angleinvers = -1;
    // if (HRA < 0)
    // {
    //     Angleinvers = -1;
    // }

    double a = cos(Yt) * sin(ZenithRAD) * sin(Ys) - sin(Yt) * sin(ZenithRAD) * cos(Ys);
    double b = cos(ZenithRAD);
    TrueAngle = Angleinvers * atan2(a, b);
    TrueAngle = TrueAngle * (180 / M_PI);

    // #pragma region TrueAngle(NREL Method)
    //  Solar Elevation Bs, Solar Azimuth Ys
    double Bs = Elevation;
    Ys = 360 - Azimuth; // Ys
    Ys = Ys * M_PI / 180;
    Bs = Bs * M_PI / 180;
    double Sx = cos(Bs) * sin(Ys);
    double Sy = cos(Bs) * cos(Ys);
    double Sz = sin(Bs);
    double Ya = 0; // Asmith
    double Yg = 0;
    double Bg = 0;
    double Dy = Ya - Yg;
    double Ba = atan(tan(Bg) * cos(Dy));
    // Ba = 180;
    Sx = Sx * M_PI / 180;
    Sy = Sy * M_PI / 180;
    Sz = Sz * M_PI / 180;
    double Sxx = (Sx * cos(Ya)) - (Sy * sin(Ya));
    double Szz = (Sx * sin(Ya) * sin(Ba)) + (Sy * sin(Ba) * cos(Ya)) + (Sz * cos(Ba));
    TrueAngleNerl = atan2(Sxx, Szz);
    TrueAngleNerl = TrueAngleNerl * (180 / M_PI);

    // #pragma region TrueAngle Backracking
    BackTrackerAngle = TrueAngle;
    double GCR = (double)TrackerWidth / Post2Post;
    double GCR_RAD = GCR * M_PI / 180;
    double TrueAngleCutOff = acos(GCR_RAD);
    double TrueAngleCutOffDeg = TrueAngleCutOff * (180 / M_PI);

    if (TrueAngleCutOffDeg > fabs(TrueAngle))
    {
        double tmpAngle = sin((90 - TrueAngle) * M_PI / 180);
        tmpAngle = asin(tmpAngle * M_PI / 180);
        if (TrueAngle < 0)
        {
            BackTrackerAngle = 90 - tmpAngle + TrueAngle;
        }
        else
        {
            BackTrackerAngle = tmpAngle - 90 + TrueAngle;
        }
    }
}

void PmmSunCalculations::UpdateCalculations(time_t date, float lat, float lang, int TimeZone, float TrackerWidth, float Post2Post)
{

    float Longitude = lang;
    float Latitude = lat;
    //1. Calculate B
    DayOfYear = PMMReturnDayOfYear(year(date), month(date), day(date));
    B = (360.0 / 365.0) * (DayOfYear - 81);
    B = B * PI / 180.0;
    //2. Calculate EOT
    EOT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
    //3. Calculate LTSM
    LSTM = 15.0 * TimeZone;
    //4. Calculate TC
    TC = 4 * (Longitude - LSTM) + EOT;
    //5. Calculate LST 
    int currentTimeInMinute = hour(date) * 60 + minute(date);
    LST = currentTimeInMinute + TC;
    //6. Calculate HRA
    HRA = 15 * (LST) / 60 + 180;
    if (HRA > 180)
        HRA = HRA - 360;
    HRARAD = DEG_TO_RAD * HRA;

    int Angleinvers = 1;
    if (HRA < 0)
    {
        Angleinvers = -1;
    }
    //7. Calculate Elevation
    DeclinationRAD = (23.45 * sin(B)) * DEG_TO_RAD;
    Declination = DeclinationRAD * RAD_TO_DEG;
    latRAD = Latitude * DEG_TO_RAD; // convert to Radians
    Elevation = asin((sin(DeclinationRAD) * sin(latRAD)) + (cos(latRAD) * cos(DeclinationRAD) * cos(HRARAD)));
    double ElevationRAD = Elevation;
    Elevation = Elevation * RAD_TO_DEG;
    //8. Calculate Azimuth
    AzimuthRAD = acos((sin(DeclinationRAD) * cos(latRAD) - cos(DeclinationRAD) * sin(latRAD) * cos(HRARAD)) / cos(ElevationRAD));
    Azimuth = AzimuthRAD * RAD_TO_DEG;
    //9. Calculate Zenith
    ZenithRAD = DEG_TO_RAD * (90 - Elevation);
    Zenith= ZenithRAD * RAD_TO_DEG;
    //10. Calculate true angle
    float Ys = 180 - Azimuth; // Ys
    float Yt = 360;
    Ys = DEG_TO_RAD * Ys;
    Yt = DEG_TO_RAD * Yt;
    float a = cos(Yt) * sin(ZenithRAD) * sin(Ys) - sin(Yt) * sin(ZenithRAD) * cos(Ys);
    float b = cos(ZenithRAD);
    TrueAngle = Angleinvers * atan(a / b);
    TrueAngle = TrueAngle * RAD_TO_DEG;

    // Sunrise and Sunset
    double tmp = (1.0 / 15) * acos(tan(DeclinationRAD) * tan(latRAD));
    tmp = tmp * RAD_TO_DEG;
    time_t dateSetRise = PMMSetDatetime((year(date) - 1970), month(date), day(date));
    // DateTime dateSetRise(date.getYear(), date.getMonth(), date.getDay(), 12, 0, 0);
     Sunset = dateSetRise + (43200);
     Sunrise = dateSetRise + (43200);
    Sunset = Sunset - (3600 * tmp);
    Sunrise = Sunrise + (3600 * tmp);
    Sunset = Sunset - (60 * TC);
    Sunrise = Sunrise - (60 * TC);

    //11. TrueAngle(NREL Method)
    //  Solar Elevation Bs, Solar Azimuth Ys
    float Bs = Elevation;
    Ys = 360 - Azimuth; // Ys
    Ys = Ys * M_PI / 180;
    Bs = Bs * M_PI / 180;
    float Sx = cos(Bs) * sin(Ys);
    float Sy = cos(Bs) * cos(Ys);
    float Sz = sin(Bs);
    float Ya = 0; // Asmith
    float Yg = 0;
    float Bg = 0;
    float Dy = Ya - Yg;
    float Ba = atan(tan(Bg) * cos(Dy));
    // Ba = 180;
    Sx = Sx * M_PI / 180;
    Sy = Sy * M_PI / 180;
    Sz = Sz * M_PI / 180;
    float Sxx = (Sx * cos(Ya)) - (Sy * sin(Ya));
    float Szz = (Sx * sin(Ya) * sin(Ba)) + (Sy * sin(Ba) * cos(Ya)) + (Sz * cos(Ba));
    TrueAngleNerl = atan2(Sxx, Szz);
    TrueAngleNerl =  TrueAngleNerl * (180 / M_PI);
    //12. region TrueAngle Backracking
    BackTrackerAngle = TrueAngle;
    float GCR = (float)(TrackerWidth / Post2Post);
    float GCR_RAD = GCR * M_PI / 180;
    float TrueAngleCutOff = acos(GCR_RAD);
    float TrueAngleCutOffDeg = TrueAngleCutOff * (180 / M_PI);
    if (TrueAngleCutOffDeg > fabs(TrueAngle))
    {
        float tmpAngle = sin((90 - TrueAngle) * M_PI / 180);
        tmpAngle = asin(tmpAngle * M_PI / 180);
        if (TrueAngle < 0)
        {
            BackTrackerAngle = 90 - tmpAngle + TrueAngle;
        }
        else
        {
            BackTrackerAngle = tmpAngle - 90 + TrueAngle;
        }
    }
}

// MPU simple functions

void PmmSunCalculations::PmmMPUSetup()
{
    Wire.begin();
    Wire.beginTransmission(0x69);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

float PmmSunCalculations::PmmMPUReadRawAngle() // This Function Get MPU Angle Without Calibration *** This Function Use Only To calibration Function
{
    int16_t AcY, AcZ;
    // int16_t AcX ;
    int minVal = 165;
    int maxVal = 602;
    Wire.beginTransmission(0x69);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x69, 14, true);
    // AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    // int xAng = map(AcX, minVal, maxVal, -90, 90);
    int yAng = map(AcY, minVal, maxVal, -90, 90);
    int zAng = map(AcZ, minVal, maxVal, -90, 90);

    double x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    // double y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
    // double z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
    if (abs(x) > 180)
    {
        x = x - 360;
    }
    return x;
}

float PmmSunCalculations::PmmMPUReadCalAngle(float MinAngle, float MaxAngle, float MinCalAngle, float MaxCalAngle) // This Function Get MPU Angle With Calibration *** This Function Uesd in All of The Code
{
    float x = PmmMPUReadRawAngle();

    x = map(x * 10, MinCalAngle * 10, MaxCalAngle * 10, MinAngle * 10, MaxAngle * 10);
    x = x / 10;
    return x;
}

/*
string PmmSunCalculations::PmmDateTimeToString(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    string str = "";

    str += to_string((int)year) + "-" + to_string((int)month) + "-" + to_string((int)day) + "  " + to_string((int)hour) + ":" + to_string((int)min) + ":" + to_string((int)sec);
    return str;
}
*/
// arr[0] --> B
// arr[1] --> EOT
// arr[2] --> Declation
// arr[3] --> LSTM
// arr[4] --> TC
// arr[5] --> HRA
// arr[6] --> HRARAD
// arr[7] --> HRAZRAD
// arr[8] --> Elevation
// arr[9] --> AzimuthRAD
// arr[10] --> Azimuth
// arr[11] --> Sunrise
// arr[12] --> Sunset
// arr[13] --> Zenith
// arr[14] --> ZenithRAD
// arr[15] --> TrueAngle
// arr[16] --> TrueAngelNerl
// arr[17] --> BackTrackerAngle
