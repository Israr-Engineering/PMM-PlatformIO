#include <Arduino.h>
#include "ProjectDef.h"
#include <PmmExternalEEPROMLib.h>

long PMMTime;
int16_t EpromValue1 = 0;
int32_t EpromValue2 = 0;
String EpromValue3 = "PMM";
float EpromValue4 = 0.0;

void setup() {

  EEpromSetup();  // Initialize EEPROM
  SerialUSB.begin(9600);

  PutIntDataToEEprom(1, 100);        //Edit value for address 0 in eeprom to 100
  PutLongDataToEEprom(2, 100000);     //Edit value for address 2 in eeprom to 100000
  PutStringDataToEEprom(4, "PMM LLC");  //Edit value for address 4 in eeprom to PMM LLC
  PutFloatDataToEEprom(6, 100.5);      //Edit value for address 6 in eeprom to 100.5

  Scheduler.startLoop(PMMConfiguration);  //Start scheduler for configuration loop
  Scheduler.startLoop(PMMCommunication);  //Start scheduler for communication loop
}

void loop() {

  if ((millis() - PMMTime) > 15000) {

    EpromValue1 = GetIntDataFromEEprom(1);     // Function to read int16_t value from address 1 in eeprom EEPROM
    EpromValue2 = GetLongDataFromEEprom(2);    // Function to read int32_t value from address 2 in eeprom EEPROM
    EpromValue3 = GetStringDataFromEEprom(4);  // Function to read String value from address 4 in eeprom EEPROM
    EpromValue4 = GetFloatDataFromEEprom(6);   // Function to read float value from address 6 in eeprom EEPROM
    
    SerialUSB.println(EpromValue1);            //Print to serial 100
    SerialUSB.println(EpromValue2);            //Print to serial 100000
    SerialUSB.println(EpromValue3);            //Print to serial PMM LLC
    SerialUSB.println(EpromValue4);            //Print to serial 100.5

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
