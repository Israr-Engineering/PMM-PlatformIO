#include <Arduino.h>
#include <ProjectDef.h>
#include <PmmTypes.h>
#include <PmmScheduler.h>

// Include your board here
#include <PMM1103V1.2.h>

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

  ModbusRTU01Server.PmmModBusRTUServerInputRead(0);
  ModbusRTU01Server.PmmModBusTCPServerHoldingWrite(0, 1948);

  // STEP04 : do anything you need in 1 second base timer
  if ((millis() - MainLoopTimer) > 1000)
  {
    SerialUSB.print("Cycle-us : ");
    SerialUSB.println(CycleTime);

    ModbusRTU01Server.PmmModBusRTUServerInputRead(0); // Do nothing but needed to keep system alive
    ///////////////////////////////////////////////////////////////
    // Timers
    if (Pmmm1103.EMERGENCYTimer > 0 ) Pmmm1103.EMERGENCYTimer = Pmmm1103.EMERGENCYTimer -1; 
    if (Pmmm1103.EMERGENCYTimerIN > 0 ) Pmmm1103.EMERGENCYTimerIN = Pmmm1103.EMERGENCYTimerIN -1; // Pmmm1103.EMERGENCYTimerIN
    // Print system variables for monitring
    if(Pmmm1103.PROGRAMMINGMODE) ShowDebugSummary();

    SerialUSB.println(Pmmm1103.MINAngle);
    SerialUSB.println(Pmmm1103.MAXAngle);
    SerialUSB.println(Pmmm1103.MINCalAngle);
    SerialUSB.println(Pmmm1103.MAXCalAngle);  //int_output[10]
    
    // SerialUSB.println((int16_t)int_output[10]);
    // SerialUSB.println(int_output[8]);
    


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
#ifdef PMMMCUWITHCONFIG

#ifdef PMMHTTPSERVER
    StartCommandHttpServer(); // this Should be here always
#endif
    PmmReadCommands();
#endif
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
