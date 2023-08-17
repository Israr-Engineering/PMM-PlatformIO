#include <Arduino.h>
#include <ProjectDef.h>
#include <PmmTypes.h>

// Include your board here
#include <PMM1003.h>

// #include <PmmGlobalFunctions.h>

void setup()
{

  // STEP01: Initialize Product IOs
  hardwareInit();
  // STEP0: Initialize WatchDog timer
  if (MasterIOMode & FactoryDefualtMode)
    PmmWatchDoggy.setup(WDT_HARDCYCLE1S);
  else if (MasterIOMode & !FactoryDefualtMode)
    PmmWatchDoggy.setup(WDT_HARDCYCLE4S);
  else
    PmmWatchDoggy.setup(WDT_HARDCYCLE8S);
  // STEP03: Warmup some period
  delay(50);
  // STEP04: tell serial i am starting
  SerialUSB.println("New Starting ...");
  // STEP05: Start threads if you need it
  // Scheduler.startLoop(PMMConfiguration);
  // Scheduler.startLoop(PMMCommunication);
  // Scheduler.startLoop(PMMTimers);
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */

void loop()
{

  // STEP01 : Clear WatchDog to prevent Restart
  PmmWatchDoggy.clear(); // NVIC_SystemReset();// Restart MCU Command

  // STEP02 : Check Alarms
  GenerateAlarm();

  // STEP03 : Update system IO images every cycle
  updateInputBuffers();
  syncModbusBuffers();
  updateOutputBuffers();

  // STEP04 : do anything you need in 1 second base timer
  if ((millis() - MainLoopTimer) > 1000)
  {
    ModbusTCPServer.PmmModBusTCPServerInputRead(0); // Do nothing but needed to keep system alive
    ///////////////////////////////////////////////////////////////
    // Print system variables for monitring
    SerialUSB.print("Slave Device ID : ");
    SerialUSB.println(Pmm1003ID);
    SerialUSB.print("IP Address : ");
    SerialUSB.println(Ethernet.localIP());
    SerialUSB.print("Hand shaking Input value : ");
    SerialUSB.println(int_input[2]);
    SerialUSB.print("Hand shaking Output value : ");
    SerialUSB.println(int_output[2]);
    SerialUSB.print("Fiber : ");
    SerialUSB.println(digitalRead(FIBERAVALIBLE));
    SerialUSB.print("External Ethernet : ");
    SerialUSB.println(digitalRead(ETHERNETAVALIBLE));
    SerialUSB.print("Ethernet/Fiber For tcp  : ");
    SerialUSB.println(digitalRead(FIBERTCP));

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
    //  StartCommandHttpServer(); // this Should be here always
    //  PmmReadCommands();
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
  if ((millis() - CommunicationTimer) > 100)
  {

    // Manage product and Extension Boards
    updateInputBuffers();
    syncModbusBuffers();
    updateOutputBuffers();

    // ThisExtensionBoarsUpdate();

    // Manage Protocol Servers
    // ThisProtocolUpdate();

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
