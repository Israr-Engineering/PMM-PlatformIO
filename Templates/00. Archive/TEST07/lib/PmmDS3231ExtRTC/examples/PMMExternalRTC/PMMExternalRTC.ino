#include "ProjectDef.h"
#include <PMMGlobalFunctions.h>
#include <PmmExternalRTC.h>

long PMMTime;

void setup() {

  SerialUSB.begin(9600);
  PMMInitializeEEPROM();
  PMMInitializeExternalRTC();  //Initialize External RTC

  SerialUSB.print("RTC Found : ");
  SerialUSB.println(PMMRTCCheck());  // Check External RTC if found

  Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
}

void loop() {

  if ((millis() - PMMTime) > 30000) {

    //ConvertStanderstringToString : Function to convert stander string (string) to arduino string (String)
    //ConvertUint32_tTostring : Function to convert Uint32_t to stander string (string)
    //ConvertUint8_tTostring : Function to convert Uint8_t to stander string (string)
    //ConvertUint16_tTostring :Function to convert Uint16_t to stander string (string)

    String UNIXTime = ConvertStanderstringToString(ConvertUint32_tTostring(PMMGetRTCUNIXTime()));            // Return RTC UNIXTime Now
    String MonthNumber = ConvertStanderstringToString(ConvertUint8_tTostring(PMMReturnMonthNumber()));       // Return RTC Current Month Number
    String DayNumber = ConvertStanderstringToString(ConvertUint8_tTostring(PMMReturnDayNumber()));           // Return RTC Current Day Number
    String DayOfYear = ConvertStanderstringToString(ConvertUint8_tTostring(PMMReturnDayOfYear()));           // Return RTC Current Day Order of year
    String CurrentHour = ConvertStanderstringToString(ConvertUint16_tTostring(PMMReturnCurrentHour()));      // Return RTC Current hour Number
    String CurrentMinute = ConvertStanderstringToString(ConvertUint16_tTostring(PMMReturnCurrentMinute()));  // Return RTC Current minute Number
    String StringTime = ConvertStanderstringToString(PMMRTCStringTime());                                    // Return RTC string Time

    //PMMSetRTCUNIXTime(uint32_t unixTime);  ==> To edit RTC time to unix date time

    SerialUSB.print("RTC Datetime Unix Now : ");
    SerialUSB.println(UNIXTime);

    SerialUSB.print("RTC Current Month : ");
    SerialUSB.println(MonthNumber);

    SerialUSB.print("RTC Current Day : ");
    SerialUSB.println(DayNumber);

    SerialUSB.print("RTC Order Day In Year : ");
    SerialUSB.println(DayOfYear);

    SerialUSB.print("RTC Current Hour : ");
    SerialUSB.println(CurrentHour);

    SerialUSB.print("RTC Current Minute : ");
    SerialUSB.println(CurrentMinute);

    SerialUSB.print("RTC Datetime Now String : ");
    SerialUSB.println(StringTime);

    PMMTime = millis();
  }
}

void PMMConfiguration() {
}

void PMMCommunication() {

  // We must call 'yield' at a regular basis to pass

  // control to other tasks.

  yield();
}