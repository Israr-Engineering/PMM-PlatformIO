#include "ProjectDef.h"
#include <PMMFlashMemoryLib.h>

long PMMTime;
int EpromValue1 = 0;
long EpromValue2 = 0;
String EpromValue3 = "PMM";
float EpromValue4 = 0.0;

void setup() {

  SerialUSB.begin(9600);
  
  PMMFlashWriteInt(1, 100);           //Edit value for address 0 in flash to 100
  PMMFlashWriteLong(2, 100000);       //Edit value for address 2 in flash to 100000
  PMMFlashWriteString(4, "PMM LLC");  //Edit value for address 4 in flash to PMM LLC
  PMMFlashWriteFloat(6, 100.5);       //Edit value for address 6 in flash to 100.5

  Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
}

void loop() {

  if ((millis() - PMMTime) > 15000) {

    EpromValue1 = PMMFlashReadInt(1, EpromValue1);     // Function to read int value from address 1 in Flash
    EpromValue2 = PMMFlashReadLong(2, EpromValue2);    // Function to read long value from address 2 in Flash
    EpromValue3 = PMMFlashReadString(4, EpromValue3);  // Function to read String value from address 4 in Flash
    EpromValue4 = PMMFlashReadFloat(6, EpromValue4);   // Function to read float value from address 6 in Flash

    SerialUSB.println(EpromValue1);  //Print to serial 100
    SerialUSB.println(EpromValue2);  //Print to serial 100000
    SerialUSB.println(EpromValue3);  //Print to serial PMM LLC
    SerialUSB.println(EpromValue4);  //Print to serial 100.5

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
