#ifndef PmmSUNCALC_h
#define PmmSUNCALC_h


 #include <array>
 #include <sstream>



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

//#ifdef PMMSunCalculations
#define M_PI 3.14159265358979323846 // pi

class PmmSunCalculations
{

private:
    /* data */

public:

    PmmSunCalculations(/* args */);
    ~PmmSunCalculations();

    string printDigits(int digits);
    int16_t PMMReturnDayOfYear(int year, int month, int day);
    string PMMDatetimeNowStr(time_t date);
    time_t PMMSetDatetime(int year, int month, int day);
    time_t PMMSetAnDatetime(int year, int month, int day,int hour,int minute,int second);
    array<string, 18> SunCalculations(time_t date, double lat, double lang, int TimeZone = 3, double TrackerWidth = 4, double Post2Post = 11);
    string SunCalculationsStr(time_t date, double lat, double lang, int TimeZone = 3, double TrackerWidth = 4, double Post2Post = 11);
    string PmmDateTimeToString(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);

};

//extern PmmSunCalculations PmmSunCalculator;

// extern string printDigits(int digits);
// extern int16_t PMMReturnDayOfYear(int year, int month, int day);
// extern string PMMDatetimeNowStr(time_t date);
// extern time_t PMMSetDatetime(int year, int month, int day);
// extern time_t PMMSetAnDatetime(int year, int month, int day,int hour,int minute,int second);
// extern array<string, 18> SunCalculations(time_t date, double lat, double lang, int TimeZone = 3, double TrackerWidth = 4, double Post2Post = 11);
// extern string SunCalculationsStr(time_t date, double lat, double lang, int TimeZone = 3, double TrackerWidth = 4, double Post2Post = 11);
// extern string PmmDateTimeToString(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);

#endif
