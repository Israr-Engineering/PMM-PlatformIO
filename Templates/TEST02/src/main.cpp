#include <Arduino.h>


#include <ProjectDef.h>

#include <PmmGlobalFunctions.h>
#include <PmmCommands.h>
#include <PmmSunCalculations.h>

bool x = false;
long TT = 0;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 55);
PmmWDTZero PmmWatchDoggy;

void PMMConfiguration();
void PMMCommunication();

void setup()
{

  PmmWatchDoggy.setup(WDT_SOFTCYCLE2M);
  PMMInitializeEthernet(ip, mac);
  SerialUSB.begin(9600);
  Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
}

void loop()
{
  PmmWatchDoggy.clear();
  if ((millis() - TT) > 3000)
  {
    time_t now = PMMSetAnDatetime(53, 3, 22, 16, 0, 0);
    SerialUSB.println(SunCalculationsStr(now, 31.5320459, 36.0276305, 3, 4, 11).c_str());
    TT = millis();
  }
}

void PMMConfiguration()
{ 
  PMMReadCommands();
  yield();
}

void PMMCommunication()
{
  

  // We must call 'yield' at a regular basis to pass

  // control to other tasks.

  yield();
}
