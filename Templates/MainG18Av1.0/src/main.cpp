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

    // SerialUSB.print(PmmRTCInternal.getYear());SerialUSB.print("-");
    //   SerialUSB.print(PmmRTCInternal.getMonth());SerialUSB.print("-");
    //    SerialUSB.print(PmmRTCInternal.getDay());SerialUSB.print(" ");

    //  SerialUSB.print(PmmRTCInternal.getHours());SerialUSB.print(":");
    //   SerialUSB.print(PmmRTCInternal.getMinutes());SerialUSB.print(":");
    //    SerialUSB.print(PmmRTCInternal.getSeconds());

    //     SerialUSB.print(PmmRTCExternal.getYear());SerialUSB.print("-");
    //   SerialUSB.print(PmmRTCExternal.getMonth(true));SerialUSB.print("-");
    //    SerialUSB.print(PmmRTCExternal.getDate());SerialUSB.print(" ");

    //  SerialUSB.print(PmmRTCExternal.getHour(false));SerialUSB.print(":");
    //   SerialUSB.print(PmmRTCExternal.getMinute());SerialUSB.print(":");
    //    SerialUSB.print(PmmRTCExternal.getSecond()); GetExternalRTC
    SerialUSB.println("Internal RTC :");
    SerialUSB.println(GetInternalRTC());
    
    SerialUSB.println("External RTC :");
    SerialUSB.println(GetExternalRTC());
     
        //SerialUSB.println( PMMRTCStringTime().c_str());

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
    AllExtensionBoarsdUpdate();

    //Manage Modbus Server
    for (int i=0 ;i<120 ; i++)
    {

      
    }




    

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
