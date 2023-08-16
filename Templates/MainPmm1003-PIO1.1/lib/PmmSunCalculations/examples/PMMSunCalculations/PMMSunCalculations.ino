#include "ProjectDef.h"
#include <PMMGlobalFunctions.h>
#include <PmmSunCalculations.h>

long PMMTime;

void setup() {

  SerialUSB.begin(9600);

  Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
}

void loop() {


  if ((millis() - PMMTime) > 30000) {

    time_t now = PMMSetAnDatetime(2023, 3, 25, 16, 0, 0);
    //SerialUSB.println(SunCalculationsStr(now, 31.5320459, 36.0276305, 3, 4, 11).c_str());

    double lat = 31.5320459;
    double lang = 36.0276305;

    array<string, 18> arr = SunCalculations(now, lat, lang,3, 4, 11);

    SerialUSB.print("B : ");
    SerialUSB.println(ConvertStanderstringToString(arr[0]));

    SerialUSB.print("EOT : ");
    SerialUSB.println(ConvertStanderstringToString(arr[1]));

    SerialUSB.print("Declation : ");
    SerialUSB.println(ConvertStanderstringToString(arr[2]));

    SerialUSB.print("LSTM : ");
    SerialUSB.println(ConvertStanderstringToString(arr[3]));

    SerialUSB.print("TC : ");
    SerialUSB.println(ConvertStanderstringToString(arr[4]));

    SerialUSB.print("HRA : ");
    SerialUSB.println(ConvertStanderstringToString(arr[5]));

    SerialUSB.print("HRARAD : ");
    SerialUSB.println(ConvertStanderstringToString(arr[6]));

    SerialUSB.print("HRAZRAD : ");
    SerialUSB.println(ConvertStanderstringToString(arr[7]));

    SerialUSB.print("Elevation : ");
    SerialUSB.println(ConvertStanderstringToString(arr[8]));

    SerialUSB.print("AzimuthRAD : ");
    SerialUSB.println(ConvertStanderstringToString(arr[9]));

    SerialUSB.print("Azimuth : ");
    SerialUSB.println(ConvertStanderstringToString(arr[10]));

    SerialUSB.print("Sunrise : ");
    SerialUSB.println(ConvertStanderstringToString(arr[11]));

    SerialUSB.print("Sunset : ");
    SerialUSB.println(ConvertStanderstringToString(arr[12]));

    SerialUSB.print("Zenith : ");
    SerialUSB.println(ConvertStanderstringToString(arr[13]));

    SerialUSB.print("ZenithRAD : ");
    SerialUSB.println(ConvertStanderstringToString(arr[14]));

    SerialUSB.print("TrueAngle : ");
    SerialUSB.println(ConvertStanderstringToString(arr[15]));

    SerialUSB.print("TrueAngleRAD : ");
    SerialUSB.println(ConvertStanderstringToString(arr[16]));

    SerialUSB.print("BackTrackerAngle : ");
    SerialUSB.println(ConvertStanderstringToString(arr[17]));

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