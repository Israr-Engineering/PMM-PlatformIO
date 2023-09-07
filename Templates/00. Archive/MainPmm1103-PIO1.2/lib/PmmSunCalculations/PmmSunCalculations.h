#ifndef PmmSUNCALC_h
#define PmmSUNCALC_h

// #include <array>
// #include <sstream>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Arduino.h"

using namespace std;

/*
    DateTime dt(2023, 2, 6, 13, 40, 0);
    array<string, 18> ResulteArray = SunCalculations(dt, 31.5320459, 36.0276305);
*/

// #ifdef PMMSunCalculations
#define M_PI 3.14159265358979323846 // pi

class PmmSunCalculations
{

private:
    /* data */

public:
    // Inptus
    float Latitude = 31.53;  // ×100| = 31.53
    float Longitude = 36.02; // ×100| = 36.02
    int TimeZone = 3;
    float TrackerWidth = 4;
    float Post2Post = 11;


    // Calaculation resulte
    int16_t DayOfYear = 0;
    float B = 0;
    float EOT = 0;
    float Declination = 0;
    float LSTM = 0;
    time_t LST = 0;
    float TC = 0;
    float HRA = 0;
    float HRARAD = 0;
    float HRAZRAD = 0;
    float Elevation = 0;
    float AzimuthRAD = 0;
    float Azimuth = 0;
    time_t Sunrise = 0;
    time_t Sunset = 0;
    float Zenith = 0;
    float ZenithRAD = 0;
    float TrueAngle = 0;
    float TrueAngleNerl = 0;
    float BackTrackerAngle = 0;

    // MPU Calibration
    float MinAngle = -60;
    float MaxAngle = 60;
    float MinCalAngle = -55;
    float MaxCalAngle = 55;

    PmmSunCalculations(/* args */);
    ~PmmSunCalculations();

    // string printDigits(int digits);
    int16_t PMMReturnDayOfYear(int year, int month, int day);
    // string PMMDatetimeNowStr(time_t date);
    time_t PMMSetDatetime(int year, int month, int day);
    time_t PMMSetAnDatetime(int year, int month, int day, int hour, int minute, int second);
    // array<string, 18> SunCalculations(time_t date, double lat, double lang, int TimeZone = 3, double TrackerWidth = 4, double Post2Post = 11);
    void UpdateCalculations(time_t date, float lat, float lang, int TimeZone = 3, float TrackerWidth = 4, float Post2Post = 11);
    void UpdateCalculationsOLD(time_t date, float lat, float lang, int TimeZone = 3, float TrackerWidth = 4, float Post2Post = 11);
    // string PmmDateTimeToString(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);

    // Simple MPU setup and read one axies
    void PmmMPUSetup();
    float PmmMPUReadRawAngle();
    float PmmMPUReadCalAngle(float MinAngle, float MaxAngle, float MinCalAngle, float MaxCalAngle);
};

#endif
