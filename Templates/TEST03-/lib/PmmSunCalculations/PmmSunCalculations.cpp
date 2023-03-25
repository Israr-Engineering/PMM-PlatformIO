#include "PmmSunCalculations.h"
#include <PmmTimeLib.h>

#include <Wire.h>
#include <String>
#include <array>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <random>

using namespace std;

// string printDigits(int digits);
// int16_t PMMReturnDayOfYear(int year, int month, int day);
// string PMMDatetimeNowStr(time_t date);
// time_t PMMSetDatetime(int year, int month, int day);
// time_t PMMSetAnDatetime(int year, int month, int day, int hour, int minute, int second);
// array<string, 18> SunCalculations(time_t date, double lat, double lang, int TimeZone, double TrackerWidth, double Post2Post);
// string SunCalculationsStr(time_t date, double lat, double lang, int TimeZone, double TrackerWidth, double Post2Post);
// string PmmDateTimeToString(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);


PmmSunCalculations::PmmSunCalculations(/* args */)
{
}

PmmSunCalculations::~PmmSunCalculations()
{
}


string PmmSunCalculations::printDigits(int digits)
{
    string x = "";
    if (digits < 10)
    {
        x = to_string(digits) + "0";
    }
    else
    {
        x = to_string(digits);
    }
    return x;
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

    // int year = date.getYear() ;
    // int month = date.getMonth();
    // int day = date.getDay();

    // int year = year(date);
    // int month = month(date);
    // int day = day(date);

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

string PmmSunCalculations::PMMDatetimeNowStr(time_t date)
{
    return to_string(year(date)) + "-" + to_string(month(date)) + "-" + to_string(day(date)) + " " + to_string(hour(date)) + ":" + to_string(minute(date)) + ":" + to_string(second(date));
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

array<string, 18> PmmSunCalculations::SunCalculations(time_t date, double lat, double lang, int TimeZone, double TrackerWidth, double Post2Post)
{
    TimeZone = 3, TrackerWidth = 4, Post2Post = 11;
    array<string, 18> arr;
    string Resulte = "Time : " + PMMDatetimeNowStr(date) + " (Lang,Lat) = " + to_string(lang) + " , " + to_string(lat) + "; \n";

#pragma region Equation of Time
    double tmpB = 360.0 / 365;

    int16_t x = PMMReturnDayOfYear(year(date), month(date), day(date));

    double B = (double)tmpB * (x - 81);
    B = B * (M_PI / 180); // convert to Radians

    Resulte += "B  = " + to_string(B) + "\n";
    arr[0] = to_string(B);

    double EOT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
    Resulte += "EOT = " + to_string(EOT) + "\n";
    arr[1] = to_string(EOT);
#pragma endregion

#pragma region Declination
    double Declination = 23.45 * sin(B);
    Resulte += "Declination = " + to_string(Declination) + "\n";
    arr[2] = to_string(Declination);
#pragma endregion

#pragma region Local Standard Time Meridian
    double LSTM = 15 * TimeZone;
    Resulte += "LSTM = " + to_string(LSTM) + "\n";
    arr[3] = to_string(LSTM);
#pragma endregion

#pragma region Time Correction Factor(TC)
    double TC = 4 * (lang - LSTM) + EOT;
    Resulte += "TC = " + to_string(TC) + " minutes." + "\n";
    arr[4] = to_string(TC);
#pragma endregion

#pragma region Local Solar Time(LST)

    // need to be edited
    time_t LST = date + (60 * TC);

#pragma endregion

#pragma region Hour Angle(HRA)

    time_t tmpLSTh = LST - 43200;

    double HRA = (double)((hour(tmpLSTh) * 60) + minute(LST) + ((double)(second(LST)) / 60)) * (double)15 / 60;

#pragma endregion

#pragma region HRA = HRA + 180;

    Resulte += "HRA = " + to_string(HRA) + "\n";
    arr[5] = to_string(HRA);

    double HRARAD = HRA * (M_PI / 180); // convert to Radians
    arr[6] = to_string(HRARAD);

    if (HRA > 180)
        HRA = HRA - 360;

    Resulte += "HRA-Hazza Project = " + to_string(HRA) + "\n";

    double HRAZRAD = HRA * (M_PI / 180); // convert to Radians

    arr[7] = to_string(HRAZRAD);

#pragma endregion

#pragma region Elevation
    double latRAD = lat * M_PI / 180;                 // convert to Radians
    double DeclinationRAD = Declination * M_PI / 180; // convert to Radians

    double Elevation = asin((sin(DeclinationRAD) * sin(latRAD)) + (cos(latRAD) * cos(DeclinationRAD) * cos(HRARAD)));
    double ElevationRAD = Elevation;
    Elevation = Elevation * (180 / M_PI);

    Resulte += "Elevation = " + to_string(Elevation) + "\n";
    arr[8] = to_string(Elevation);

#pragma endregion

#pragma region Azimuth
    double Azimuth = acos(
        (sin(DeclinationRAD) * cos(latRAD) - cos(DeclinationRAD) * sin(latRAD) * cos(HRARAD)) / cos(ElevationRAD));

    double AzimuthRAD = Azimuth;
    arr[9] = to_string(AzimuthRAD);

    Azimuth = Azimuth * (180 / M_PI);

    if (HRA > 0)
    {
        Azimuth = 360 - Azimuth;
        AzimuthRAD = Azimuth * M_PI / 180;
    }

    Resulte += "Azimuth = " + to_string(Azimuth) + "\n";
    arr[10] = to_string(Azimuth);

#pragma endregion

#pragma region Sunrise and Sunset

    double tmp = (1.0 / 15) * acos(tan(DeclinationRAD) * tan(latRAD));
    tmp = tmp * 180 / M_PI;

    time_t dateSetRise = PMMSetDatetime((year(date) - 1970), month(date), day(date));
    // DateTime dateSetRise(date.getYear(), date.getMonth(), date.getDay(), 12, 0, 0);

    time_t Sunset = dateSetRise + (43200);
    time_t Sunrise = dateSetRise + (43200);

    Sunset = Sunset - (3600 * tmp);
    Sunrise = Sunrise + (3600 * tmp);

    Sunset = Sunset - (60 * TC);
    Sunrise = Sunrise - (60 * TC);

    Resulte = Resulte + "Sunrise = " + PmmDateTimeToString((year(Sunrise)), month(Sunrise), day(Sunrise), hour(Sunrise), minute(Sunrise), second(Sunrise)) + "\n";
    arr[11] = PmmDateTimeToString((year(Sunrise) - 1970), month(Sunrise), day(Sunrise), hour(Sunrise), minute(Sunrise), second(Sunrise));

    Resulte = Resulte + "Sunrise = " + PmmDateTimeToString((year(Sunset)), month(Sunset), day(Sunset), hour(Sunset), minute(Sunset), second(Sunset)) + "\n";
    arr[12] = PmmDateTimeToString((year(Sunset) - 1970), month(Sunset), day(Sunset), hour(Sunset), minute(Sunset), second(Sunset));

#pragma endregion

#pragma region TrueAngle and Zenith
    double Zenith = 90 - Elevation; // Bs
    Resulte = Resulte + "Zenith = " + to_string(Zenith) + "\n";
    arr[13] = to_string(Zenith);

    double Ys = 180 - Azimuth; // Ys
    double Yt = 360;

    Ys = Ys * M_PI / 180;
    Yt = Yt * M_PI / 180;
    double ZenithRAD = Zenith * M_PI / 180;
    arr[14] = to_string(ZenithRAD);

    double a = cos(Yt) * sin(ZenithRAD) * sin(Ys) - sin(Yt) * sin(ZenithRAD) * cos(Ys);
    double b = cos(ZenithRAD);

    double TrueAngle = -1 * atan2(a, b);
    TrueAngle = TrueAngle * (180 / M_PI);

    Resulte = Resulte + "TrueAngle = " + to_string(TrueAngle) + "\n";
    arr[15] = to_string(TrueAngle);
#pragma endregion

#pragma region TrueAngle(NREL Method)
    // Solar Elevation Bs, Solar Azimuth Ys
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

    double TrueAngleNerl = atan2(Sxx, Szz);

    TrueAngleNerl = TrueAngleNerl * (180 / M_PI);

    Resulte = Resulte + "NREL TrueAngle = " + to_string(TrueAngleNerl);
    arr[16] = to_string(TrueAngleNerl);
#pragma endregion

#pragma region TrueAngle Backracking
    double BackTrackerAngle = TrueAngle;
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
    arr[17] = to_string(BackTrackerAngle);
#pragma endregion

    return arr;
}

string PmmSunCalculations::SunCalculationsStr(time_t date, double lat, double lang, int TimeZone, double TrackerWidth, double Post2Post)
{
    TimeZone = 3, TrackerWidth = 4, Post2Post = 11;
    array<string, 18> arr;
    string Resulte = "Time : " + PMMDatetimeNowStr(date) + " (Lang,Lat) = " + to_string(lang) + " , " + to_string(lat) + "; \n";

#pragma region Equation of Time
    double tmpB = 360.0 / 365;

    int16_t x = PMMReturnDayOfYear(year(date), month(date), day(date));

    double B = (double)tmpB * (x - 81);
    B = B * (M_PI / 180); // convert to Radians

    Resulte += "B  = " + to_string(B) + "\n";
    arr[0] = to_string(B);

    double EOT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
    Resulte += "EOT = " + to_string(EOT) + "\n";
    arr[1] = to_string(EOT);
#pragma endregion

#pragma region Declination
    double Declination = 23.45 * sin(B);
    Resulte += "Declination = " + to_string(Declination) + "\n";
    arr[2] = to_string(Declination);
#pragma endregion

#pragma region Local Standard Time Meridian
    double LSTM = 15 * TimeZone;
    Resulte += "LSTM = " + to_string(LSTM) + "\n";
    arr[3] = to_string(LSTM);
#pragma endregion

#pragma region Time Correction Factor(TC)
    double TC = 4 * (lang - LSTM) + EOT;
    Resulte += "TC = " + to_string(TC) + " minutes." + "\n";
    arr[4] = to_string(TC);
#pragma endregion

#pragma region Local Solar Time(LST)

    // need to be edited
    time_t LST = date + (60 * TC);

#pragma endregion

#pragma region Hour Angle(HRA)

    time_t tmpLSTh = LST - 43200;

    double HRA = (double)((hour(tmpLSTh) * 60) + minute(LST) + ((double)(second(LST)) / 60)) * (double)15 / 60;

#pragma endregion

#pragma region HRA = HRA + 180;

    Resulte += "HRA = " + to_string(HRA) + "\n";
    arr[5] = to_string(HRA);

    double HRARAD = HRA * (M_PI / 180); // convert to Radians
    arr[6] = to_string(HRARAD);

    if (HRA > 180)
        HRA = HRA - 360;

    Resulte += "HRA-Hazza Project = " + to_string(HRA) + "\n";

    double HRAZRAD = HRA * (M_PI / 180); // convert to Radians

    arr[7] = to_string(HRAZRAD);

#pragma endregion

#pragma region Elevation
    double latRAD = lat * M_PI / 180;                 // convert to Radians
    double DeclinationRAD = Declination * M_PI / 180; // convert to Radians

    double Elevation = asin((sin(DeclinationRAD) * sin(latRAD)) + (cos(latRAD) * cos(DeclinationRAD) * cos(HRARAD)));
    double ElevationRAD = Elevation;
    Elevation = Elevation * (180 / M_PI);

    Resulte += "Elevation = " + to_string(Elevation) + "\n";
    arr[8] = to_string(Elevation);

#pragma endregion

#pragma region Azimuth
    double Azimuth = acos(
        (sin(DeclinationRAD) * cos(latRAD) - cos(DeclinationRAD) * sin(latRAD) * cos(HRARAD)) / cos(ElevationRAD));

    double AzimuthRAD = Azimuth;
    arr[9] = to_string(AzimuthRAD);

    Azimuth = Azimuth * (180 / M_PI);

    if (HRA > 0)
    {
        Azimuth = 360 - Azimuth;
        AzimuthRAD = Azimuth * M_PI / 180;
    }

    Resulte += "Azimuth = " + to_string(Azimuth) + "\n";
    arr[10] = to_string(Azimuth);

#pragma endregion

#pragma region Sunrise and Sunset

    double tmp = (1.0 / 15) * acos(tan(DeclinationRAD) * tan(latRAD));
    tmp = tmp * 180 / M_PI;

    time_t dateSetRise = PMMSetDatetime((year(date) - 1970), month(date), day(date));
    // DateTime dateSetRise(date.getYear(), date.getMonth(), date.getDay(), 12, 0, 0);

    // DateTime Sunset = dateSetRise.AddMinutes(-TC);

    time_t Sunset = dateSetRise + (43200);
    time_t Sunrise = dateSetRise + (43200);

    Sunset = Sunset - (3600 * tmp);
    Sunrise = Sunrise + (3600 * tmp);

    Sunset = Sunset - (60 * TC);
    Sunrise = Sunrise - (60 * TC);

    // Sunset = Sunset.AddHours(-tmp);
    // Sunset = Sunset.AddHours(-12);

    // DateTime Sunrise = dateSetRise.AddMinutes(-TC);
    // Sunrise = Sunrise.AddHours(-tmp);
    // Sunrise = Sunrise.AddHours(-12);

    // Sunrise = Sunrise - (60 * tmp);
    // Sunrise = Sunrise - (60 * 12);

    Resulte = Resulte + "Sunrise = " + PmmDateTimeToString((year(Sunrise)), month(Sunrise), day(Sunrise), hour(Sunrise), minute(Sunrise), second(Sunrise)) + "\n";
    arr[11] = PmmDateTimeToString((year(Sunrise) - 1970), month(Sunrise), day(Sunrise), hour(Sunrise), minute(Sunrise), second(Sunrise));

    Resulte = Resulte + "Sunrise = " + PmmDateTimeToString((year(Sunset)), month(Sunset), day(Sunset), hour(Sunset), minute(Sunset), second(Sunset)) + "\n";
    arr[12] = PmmDateTimeToString((year(Sunset) - 1970), month(Sunset), day(Sunset), hour(Sunset), minute(Sunset), second(Sunset));

#pragma endregion

#pragma region TrueAngle and Zenith
    double Zenith = 90 - Elevation; // Bs
    Resulte = Resulte + "Zenith = " + to_string(Zenith) + "\n";
    arr[13] = to_string(Zenith);

    double Ys = 180 - Azimuth; // Ys
    double Yt = 360;

    Ys = Ys * M_PI / 180;
    Yt = Yt * M_PI / 180;
    double ZenithRAD = Zenith * M_PI / 180;
    arr[14] = to_string(ZenithRAD);

    double a = cos(Yt) * sin(ZenithRAD) * sin(Ys) - sin(Yt) * sin(ZenithRAD) * cos(Ys);
    double b = cos(ZenithRAD);

    double TrueAngle = -1 * atan2(a, b);
    TrueAngle = TrueAngle * (180 / M_PI);

    Resulte = Resulte + "TrueAngle = " + to_string(TrueAngle) + "\n";
    arr[15] = to_string(TrueAngle);
#pragma endregion

#pragma region TrueAngle(NREL Method)
    // Solar Elevation Bs, Solar Azimuth Ys
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

    double TrueAngleNerl = atan2(Sxx, Szz);

    TrueAngleNerl = TrueAngleNerl * (180 / M_PI);

    Resulte = Resulte + "NREL TrueAngle = " + to_string(TrueAngleNerl);
    arr[16] = to_string(TrueAngleNerl);
#pragma endregion

#pragma region TrueAngle Backracking
    double BackTrackerAngle = TrueAngle;
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
    arr[17] = to_string(BackTrackerAngle);
#pragma endregion

    return Resulte;
}

string PmmSunCalculations::PmmDateTimeToString(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    string str = "";

    str += to_string((int)year) + "-" + to_string((int)month) + "-" + to_string((int)day) + "  " + to_string((int)hour) + ":" + to_string((int)min) + ":" + to_string((int)sec);
    return str;
}

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
