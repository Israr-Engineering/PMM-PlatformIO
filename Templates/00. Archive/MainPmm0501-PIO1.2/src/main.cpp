#include <Arduino.h>
#include <ProjectDef.h>
#include <PmmTypes.h>
#include <PmmScheduler.h>

// Include your board here
#include <PMM0501.h>

#ifdef PMMMCUWITHCONFIG
#include <PmmGlobalFunctions.h>
#endif

void setup()
{

  // STEP01: Initialize Product IOs
  hardwareInit();
  // STEP02: Do something general here !!!

  // STEP03: Warmup some period
  delay(50);
  // STEP04: tell serial i am starting
  SerialUSB.println("New Starting ...");
// STEP05: Start threads if you need it
#ifdef PMMMCUWITHCONFIG
  Scheduler.startLoop(PMMConfiguration);
#endif
#ifdef PMMCOMMTHREAD
  Scheduler.startLoop(PMMCommunication);
#endif
#ifdef PMMTIMERS
  Scheduler.startLoop(PMMTimers);
#endif
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */
unsigned long CycleTime = 0;
void loop()
{

  // STEP01 : Clear WatchDog to prevent Restart / calc Cycletime
  CycleTime = micros() - CycleTime;

  PmmWatchDoggy.clear(); // NVIC_SystemReset();// Restart MCU Command

  // STEP02 : Check Alarms
  GenerateAlarm();
// STEP03 : Update system IO images every cycle
#ifndef PMMCOMMTHREAD
  updateInputBuffers();
  syncModbusBuffers();
  updateOutputBuffers();
#endif
  // syncModbusBuffers();

  // STEP04 : do anything you need in 1 second base timer
  if ((millis() - MainLoopTimer) > 1000)
  {
    SerialUSB.print("Cycle-μs : ");
    SerialUSB.println(CycleTime);
    
    //Print system variables for monitring
    SerialUSB.print("localIP : ");
    SerialUSB.print(Ethernet.localIP());
    SerialUSB.print(" /BaudRate01 : ");
    SerialUSB.print(BaudRate01);
    SerialUSB.print(" /Port01 : ");
    SerialUSB.print(ThisProduct.PmmSerial[0].Port01); 
     SerialUSB.print(" /Port02 : ");
    SerialUSB.print(ThisProduct.PmmSerial[0].Port02); 
    SerialUSB.print(" /SettingSerial01 : ");
    SerialUSB.print(SettingSerial01); //SettingSerial01
    SerialUSB.print(" /P2P : ");
    SerialUSB.print(ThisProduct.PmmSerial[0].PeerToPeer); //SettingSerial01
    SerialUSB.print(" /SerialServer : ");
    SerialUSB.print(ThisProduct.PmmSerial[0].SerialServer); //SettingSerial01
    SerialUSB.print(" /RemoteIP : ");
    SerialUSB.print(ThisProduct.PmmSerial[0].RemoteIP); //tmpTracking
    SerialUSB.print(" /tmpTracking : ");
    SerialUSB.print(tmpTracking); //tmpTracking
    SerialUSB.println("");

    // Send to serial 
    //Serial.write("hi");

    MainLoopTimer = millis();
  }
  CycleTime = micros();
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 02 : Configuration and commands updating    //
/////////////////////////////////////////////////// */
#ifdef PMMMCUWITHCONFIG
void PMMConfiguration()
{
  if ((millis() - ConfigurationTimer) > 500)
  {
//#ifdef PMMMCUWITHCONFIG

#ifdef PMMHTTPSERVER
    //StartCommandHttpServer(); // this Should be here always
    PmmReadHttpCommands();
#endif
    PmmReadUSBCommands();
//#endif
    ConfigurationTimer = millis();
  }
  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}
#endif

/* ///////////////////////////////////////////////////
// Loop 03 : Communication updating loop            //
/////////////////////////////////////////////////// */
#ifdef PMMCOMMTHREAD
void PMMCommunication()
{

  if ((millis() - CommunicationTimer) > 10)
  {
    // UpDateLEDs();
    //  Manage product and Extension Boards
    // STEP02 : Check Alarms
    // GenerateAlarm();
    // STEP03 : Update system IO images every cycle
    updateInputBuffers();
    // syncModbusBuffers();
    updateOutputBuffers();

    // ThisExtensionBoarsUpdate();

    // Manage Protocol Servers
    // ThisProtocolUpdate();

    CommunicationTimer = millis();
  }
  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}
#endif

/* ///////////////////////////////////////////////////
// Loop 04 : Timers updating loop                   //
/////////////////////////////////////////////////// */
#ifdef PMMTIMERS
void PMMTimers()
{
  if ((micros() - CommunicationTimer) > 10000)
  {
    CommunicationTimer = micros();
  }
  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}
#endif