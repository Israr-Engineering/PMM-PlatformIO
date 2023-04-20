#include <Arduino.h>
#include <ProjectDef.h>

#include <PmmCommands.h>
#include <PmmGlobalFunctions.h>




void setup()
{

  // STEP00: Start usb serial for debug
  SerialUSB.begin(9600);

  // STEP01: Read Flash ROM and update Settings

  PmmInitializeProjectSettings();

  // STEP02: Initialize Extra needed Modules
  
    PmmRTCInternal.begin();

  // STEP05: Warmup 2 seconds

  SerialUSB.println("New Start");
  
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */

int x1 = 0;

void loop()
{

  PmmWatchDoggy.clear();

  if ((millis() - MainLoopTimer) > 1000)
  {

    // PmmModbusRTUServer.poll();
    // if (PmmModbusRTUServer.holdingRegisterWrite(0, x1) == 1)
    // {
    //   SerialUSB.println(" Write done");
    //   SerialUSB.println(PmmModbusRTUServer.holdingRegisterRead(0));
    // }
    // else
    // {
    //   SerialUSB.println(" Write fail");
    // }

    SerialUSB.print(PmmRTCInternal.getYear());SerialUSB.print("-");
      SerialUSB.print(PmmRTCInternal.getMonth());SerialUSB.print("-");
       SerialUSB.print(PmmRTCInternal.getDay());SerialUSB.print(" ");

     SerialUSB.print(PmmRTCInternal.getHours());SerialUSB.print(":");
      SerialUSB.print(PmmRTCInternal.getMinutes());SerialUSB.print(":");
       SerialUSB.print(PmmRTCInternal.getSeconds());
       
     SerialUSB.println(" ..Running...");
     MainLoopTimer = millis();
    x1++;
  }
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 02 : Configuration and commands updating    //
/////////////////////////////////////////////////// */

void PMMConfiguration()
{

  if ((millis() - ConfigurationTimer) > 500)
  {
    PMMReadCommands();
    ConfigurationTimer = millis();
  }
  // ConfigurationTimer

  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 03 : Communication updating loop            //
/////////////////////////////////////////////////// */

void PMMCommunication()
{

  if ((millis() - CommunicationTimer) > 500)
  {

    // Manage Extension Boards





    

    CommunicationTimer = millis();
  }

  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 04 : Timers updating loop                   //
/////////////////////////////////////////////////// */

void PMMTimers()
{

  if ((micros() - CommunicationTimer) > 10000)
  {

    CommunicationTimer = micros();
  }

  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

/* ///////////////////////////////////////////////////
// WATCHDOG: Reset Controller                       //
/////////////////////////////////////////////////// */
