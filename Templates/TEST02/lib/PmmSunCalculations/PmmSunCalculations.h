#include <String>
#include <array>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <random>

#ifdef PMMSunCalculations

#define M_PI 3.14159265358979323846 // pi

extern string printDigits(int digits);
extern int16_t PMMReturnDayOfYear(int year, int month, int day);
extern string PMMDatetimeNowStr(time_t date);
extern time_t PMMSetDatetime(int year, int month, int day);
extern time_t PMMSetAnDatetime(int year, int month, int day,int hour,int minute,int second);
extern array<string, 18> SunCalculations(time_t date, double lat, double lang, int TimeZone = 3, double TrackerWidth = 4, double Post2Post = 11);
extern string SunCalculationsStr(time_t date, double lat, double lang, int TimeZone = 3, double TrackerWidth = 4, double Post2Post = 11);
extern string PmmDateTimeToString(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);

#endif

/*
    DateTime dt(2023, 2, 6, 13, 40, 0);
    array<string, 18> ResulteArray = SunCalculations(dt, 31.5320459, 36.0276305);
*/