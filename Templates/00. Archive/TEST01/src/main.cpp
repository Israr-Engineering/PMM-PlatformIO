#include <Arduino.h>
#include <ProjectDef.h>


#include <PmmGlobalFunctions.h>
#include <PmmCommands.h>
//#include <PmmSunCalculations.h>



bool x = false;
long TT = 0;

void PMMConfiguration();
void PMMCommunication();

void setup()
{

  Scheduler.startLoop(PMMConfiguration);

  Scheduler.startLoop(PMMCommunication);
}

void loop()
{
  time_t now = time(0);

  // if ((millis() - TT) > 3000)
  // {
  //   now = PMMSetAnDatetime(53,3,21,13,00,0);
  //   SerialUSB.println(SunCalculationsStr(now,31.5320459, 36.0276305,3,4,11).c_str());
  //   TT = millis();
  // }
}

void PMMConfiguration()
{

  yield();
}

void PMMCommunication()
{

  // We must call 'yield' at a regular basis to pass

  // control to other tasks.

  yield();
}

