#include "ProjectDef.h"
#include <PMMGlobalFunctions.h>
#include <PmmInternalRTCLib.h>

long PMMTime;

void setup() {

  SerialUSB.begin(9600);
  PMMInternalRTCSetup();  //Initialize Internal RTC

  Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
}

void loop() {

  if ((millis() - PMMTime) > 30000) {

    //ConvertStanderstringToString : Function to convert stander string (string) to arduino string (String)
    //ConvertUint32_tTostring : Function to convert Uint32_t to stander string (string)
    //ConvertUint16_tTostring :Function to convert Uint16_t to stander string (string)

    String UNIXTime = ConvertStanderstringToString(ConvertUint32_tTostring(PMMInternalRTCGetUnixTime()));  // Return RTC UNIXTime Now
    String Year = ConvertStanderstringToString(ConvertUint16_tTostring(PMMInternalRTCGetYear()));          // Return RTC Current Year
    String Month = ConvertStanderstringToString(ConvertUint16_tTostring(PMMInternalRTCGetMonth()));        // Return RTC Current Month
    String Day = ConvertStanderstringToString(ConvertUint16_tTostring(PMMInternalRTCGetDay()));            // Return RTC Current Day 
    String Hours = ConvertStanderstringToString(ConvertUint16_tTostring(PMMInternalRTCGetHours()));        // Return RTC Current hours
    String Minutes = ConvertStanderstringToString(ConvertUint16_tTostring(PMMInternalRTCGetMinutes()));    // Return RTC Current minutes
    String Seconds = ConvertStanderstringToString(ConvertUint16_tTostring(PMMInternalRTCGetSeconds()));    // Return RTC Current seconds

    //PMMInternalRTCSetUnixTime(int32_t UnixTime, float TimeZone);  ==> To Addition unixtime and timezone multiplied with 3600  then edit rtc time

    SerialUSB.print("RTC Datetime Unix Now : ");
    SerialUSB.println(UNIXTime);

    SerialUSB.print("RTC Current Year : ");
    SerialUSB.println(Year);

    SerialUSB.print("RTC Current Month : ");
    SerialUSB.println(Month);

    SerialUSB.print("RTC Current Day : ");
    SerialUSB.println(Day);

    SerialUSB.print("RTC Current Hours : ");
    SerialUSB.println(Hours);

    SerialUSB.print("RTC Current Minutes : ");
    SerialUSB.println(Minutes);

    SerialUSB.print("RTC Current Seconds : ");
    SerialUSB.println(Seconds);

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