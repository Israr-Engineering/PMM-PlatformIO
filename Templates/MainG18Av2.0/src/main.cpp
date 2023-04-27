#include <Arduino.h>
#include <ProjectDef.h>

#include <PmmCommands.h>
#include <PmmGlobalFunctions.h>

// Include your board here
#include <PMM0620.h>

void setup()
{
  // STEP00: Start usb serial for debug
  SerialUSB.begin(9600);
  // STEP01: Read Flash ROM and update Settings
 

  PmmInitializeProjectSettings();

  //  STEP02: Initialize Extra needed Modules
  ThisProductSetup();

  // STEP03: Warmup 1 sec
  delay(1000);
  SerialUSB.println("New Starting ...");
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */

void loop()
{
  PmmWatchDoggy.clear();
  if ((millis() - MainLoopTimer) > 1000)
  {

    // SerialUSB.print(PmmRTCInternal.getYear());SerialUSB.print("-");
    // SerialUSB.print(PmmRTCInternal.getMonth());SerialUSB.print("-");
    // SerialUSB.print(PmmRTCInternal.getDay());SerialUSB.print(" ");
    // SerialUSB.print(PmmRTCExternal.getYear());SerialUSB.print("-");
    // SerialUSB.print(PmmRTCExternal.getMonth(true));SerialUSB.print("-");
    // SerialUSB.print(PmmRTCExternal.getDate());SerialUSB.print(" ");

    // SerialUSB.println("Internal RTC :");
    // SerialUSB.println(GetInternalRTC());

    // SerialUSB.println("External RTC :");
    // SerialUSB.println(GetExternalRTC());
// 
  //SerialUSB.println(" ..Running...");

    //2023-04-27 => Yazan
    //SerialUSB.print(" ..Test Ethernet : ");
    //PMMInitializeEthernet();
    // SerialUSB.println(ThisProduct.EthernetRunning);
    //  if (ThisProduct.EthernetRunning == true)
    //  {
    //  SerialUSB.println(" Ethernet Connected ..");
    //  }
    // else
    // {
    //   SerialUSB.println(" Ethernet Disonnected ..");
    // }

    MainLoopTimer = millis();
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

    StartCommandHttpServer(); // this Should be here always
    
    PMMReadCommands();
    ConfigurationTimer = millis();
  }
  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 03 : Communication updating loop            //
/////////////////////////////////////////////////// */

void PMMCommunication()
{
  if ((millis() - CommunicationTimer) > ThisProduct.PmmTimers.CommUpdateTimer)
  {
    // Manage Extension Boards
    AllExtensionBoarsdUpdate();
    // Manage Modbus Server
    //ModbusPort[1].ModbusServersUpdate(1);

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
