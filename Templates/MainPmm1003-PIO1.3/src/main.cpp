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
  if (MasterIOMode & FactoryDefualtMode & ModbusTCPMode)
    PmmWatchDoggy.setup(WDT_HARDCYCLE1S);
  else if (MasterIOMode & FactoryDefualtMode & !ModbusTCPMode)
    PmmWatchDoggy.setup(WDT_HARDCYCLE2S);
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
   //Scheduler.startLoop(PMMCommunication);
  // Scheduler.startLoop(PMMTimers);
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */
unsigned long CycleTime = 0 ;
void loop()
{
  CycleTime = micros() - CycleTime;
  // STEP01 : Clear WatchDog to prevent Restart
  PmmWatchDoggy.clear(); // NVIC_SystemReset();// Restart MCU Command

  // STEP02 : Check Alarms
  GenerateAlarm();

  // STEP03 : Update system IO images every cycle
  updateInputBuffers();
  
  syncModbusBuffers();
  
  updateOutputBuffers();

  //if(!ModbusTCPMode) delay(10);

  // STEP04 : do anything you need in 1 second base timer
  if ((millis() - MainLoopTimer) > 1000)
  {
    SerialUSB.print("Cycle in micro second : ");
    SerialUSB.println(CycleTime);
     
    ModbusRTUServer.PmmModBusRTUServerInputRead(0); // Do nothing but needed to keep system alive
    ModbusTCPServer.PmmModBusTCPServerInputRead(0); // Do nothing but needed to keep system alive
    ///////////////////////////////////////////////////////////////
    // Print system variables for monitring
    if (FIBERTCP)
    {
    
    
    SerialUSB.print("Slave Device ID : ");
    SerialUSB.print(Pmm1003ID);
    SerialUSB.print("/ IP Address : ");
    SerialUSB.println(Ethernet.localIP());
    //  SerialUSB.print("/ Master Mode : ");
    // SerialUSB.print(MasterIOMode);
    //  SerialUSB.print("/ TCP Mode : ");
    // SerialUSB.print(ModbusTCPMode);
    // SerialUSB.print("/ Hand shaking Input value : ");
    // SerialUSB.print(int_input[2]);
    // SerialUSB.print("/ Hand shaking Output value : ");
    // SerialUSB.println(int_output[2]);

    // SerialUSB.print("Fiber : ");
    // SerialUSB.print(digitalRead(FIBERAVALIBLE));
    // SerialUSB.print("/ External Ethernet : ");
    // SerialUSB.print(digitalRead(ETHERNETAVALIBLE));
    // SerialUSB.print("/ Ethernet-Fiber For tcp  : ");
    // SerialUSB.println(digitalRead(FIBERTCP));
    }

    MainLoopTimer = millis();
  }
  CycleTime = micros();
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
    //UpDateLEDs();
    // Manage product and Extension Boards
    // updateInputBuffers();
    // syncModbusBuffers();
    // updateOutputBuffers();

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
