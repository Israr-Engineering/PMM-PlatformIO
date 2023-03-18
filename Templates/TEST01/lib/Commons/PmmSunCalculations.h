#include "ProjectDef.h"

#ifdef PMMSunCalculations

class DateTime
{

private:
#pragma region Data Members
    double year;
    double month;
    double day;
    double hour;
    double min;
    double sec;
#pragma endregion

public:
#pragma region Constructor
    // constructor
    DateTime(int y, int m, int d, int h, int min, int s) : year(y), month(m), day(d), hour(h), min(min), sec(s) {}

    // default constructor
    DateTime() : year(0), month(0), day(0), hour(0), min(0), sec(0) {}

#pragma endregion

#pragma region setter functions
    void setYear(double y)
    {
        year = y;
    }
    void setDay(double d) { day = d; }
    void setMonth(double m) { month = m; }
    void setHour(double h) { hour = h; }
    void setMin(double m) { min = m; }
    void setSec(double s) { sec = s; }
#pragma endregion

#pragma region getter functions
    double getYear() const
    {
        return year;
    }
    double getDay() const { return day; }
    double getMonth() const { return month; }
    double getHour() const { return hour; }
    double getMin() const { return min; }
    double getSec() const { return sec; }
#pragma endregion

#pragma region Member Functions
    // function to return all data members as a string
    string toString() const
    {
        string str = "";

        str += to_string((int)year) + "-" + to_string((int)month) + "-" + to_string((int)day) + "  " + to_string((int)hour) + ":" + to_string((int)min) + ":" + to_string((int)sec);
        return str;
    }

    DateTime AddMinutes(double value)
    {
        DateTime temp;
        if (abs(value) >= 1 && abs(value) < 60)
        {
            temp.setYear(this->getYear());
            temp.setMonth(this->getMonth());
            temp.setDay(this->getDay());
            temp.setHour(this->getHour());
            temp.setMin(this->getMin() + value); // add the Min here
            temp.setSec(this->getSec());
        }
        else if (value > 60)
        {
            value /= 60;

            double intPart, fracPart;
            fracPart = modf(value, &intPart);
            fracPart *= 60;

            temp.setYear(this->getYear());
            temp.setMonth(this->getMonth());
            temp.setDay(this->getDay());
            temp.setHour(this->getHour() + value);  // add the Hour here
            temp.setMin(this->getMin() + fracPart); // add the Min here
            temp.setSec(this->getSec());
        }
        else if (abs(value) < 1)
        {
            temp.setYear(this->getYear());
            temp.setMonth(this->getMonth());
            temp.setDay(this->getDay());
            temp.setHour(this->getHour());
            temp.setMin(this->getMin());
            temp.setSec(this->getSec() + value); // add the Sec here
        }

        return temp;
    }

    DateTime AddHours(double value)
    {
        DateTime temp;

        if (abs(value) >= 1)
        {
            double intPart, fracPart;
            fracPart = modf(value, &intPart);
            fracPart *= 60;

            temp.setYear(this->getYear());
            temp.setMonth(this->getMonth());
            temp.setDay(this->getDay());
            temp.setHour(this->getHour() + intPart); // add the Hr here
            temp.setMin(this->getMin() + fracPart);  // add the Min here
            temp.setSec(this->getSec());
        }
        else if (abs(value) < 1)
        {
            value *= 60;
            temp.setYear(this->getYear());
            temp.setMonth(this->getMonth());
            temp.setDay(this->getDay());
            temp.setHour(this->getHour());
            temp.setMin(this->getMin() + value); // add the Min here
            temp.setSec(this->getSec());
        }

        return temp;
    }

#pragma endregion
};

int16_t PMMReturnDayOfYear(DateTime date)
{
    boost::gregorian::date d(date.getYear(), date.getMonth(), date.getDay());
    int16_t dayNumber = d.day_of_year();
    return dayNumber;
}

array<string, 18> SunCalculations(DateTime date, double lat, double lang, int TimeZone = 3, double TrackerWidth = 4, double Post2Post = 11)
{

    array<string, 18> arr;
    string Resulte = "Time : " + date.toString() + " (Lang,Lat) = " + to_string(lang) + " , " + to_string(lat) + "; \n";

#pragma region Equation of Time
    double tmpB = 360.0 / 365;
    int16_t x = PMMReturnDayOfYear(date);
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
    DateTime LST = date.AddMinutes(TC);

#pragma endregion

#pragma region Hour Angle(HRA)

    DateTime tmpLSTh = LST.AddHours(-12);

    double HRA = (double)((tmpLSTh.getHour() * 60) + LST.getMin() + ((double)LST.getSec() / 60)) * (double)15 / 60;

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

    DateTime dateSetRise(date.getYear(), date.getMonth(), date.getDay(), 12, 0, 0);

    DateTime Sunset = dateSetRise.AddMinutes(-TC);
    Sunset = Sunset.AddHours(-tmp);
    Sunset = Sunset.AddHours(-12);

    DateTime Sunrise = dateSetRise.AddMinutes(-TC);
    Sunrise = Sunrise.AddHours(-tmp);
    Sunrise = Sunrise.AddHours(-12);

    Resulte = Resulte + "Sunrise = " + Sunrise.toString() + "\n";
    arr[11] = Sunrise.toString();

    Resulte = Resulte + "Sunrise = " + Sunset.toString() + "\n";
    arr[12] = Sunset.toString();

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

#endif

/*
    DateTime dt(2023, 2, 6, 13, 40, 0);
    array<string, 18> ResulteArray = SunCalculations(dt, 31.5320459, 36.0276305);
*/