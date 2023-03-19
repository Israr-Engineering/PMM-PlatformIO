#include <Arduino.h>

//#include <PmmScheduler.h>

#include <ProjectDef.h>

void PMMConfiguration();
void PMMCommunication();

void setup() {

  Scheduler.startLoop(PMMConfiguration);
  
  Scheduler.startLoop(PMMCommunication);
}

void loop() {
 
}

void PMMConfiguration() {

   yield();
}

void PMMCommunication() {


  // We must call 'yield' at a regular basis to pass

  // control to other tasks.

  yield();
}