#include <Arduino.h>
#include <ProjectDef.h>
#include <PmmTypes.h>

// Include your board here
#include <PMM1003.h>

// #include <PmmGlobalFunctions.h>

void setup()
{

  // STEP02: Initialize Product IOs
  hardwareInit();
  if (MasterIOMode)
   PmmWatchDoggy.setup(WDT_HARDCYCLE2S);

  // STEP04: Warmup 1 sec
  delay(50);
  SerialUSB.println("New Starting ...");
  // Scheduler.startLoop(PMMConfiguration);
   //Scheduler.startLoop(PMMCommunication);
  // Scheduler.startLoop(PMMTimers);
  
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */

void loop()
{
  
  if (MasterIOMode)
   PmmWatchDoggy.clear();
    // NVIC_SystemReset();// Restart
  
 
  GenerateAlarm();
  
  updateInputBuffers();
  syncModbusBuffers();
  updateOutputBuffers();


  if ((millis() - MainLoopTimer) > 1000)
  {

    ModbusTCPServer.PmmModBusTCPServerInputRead(0);
    ///////////////////////////////////////////////////////////////
    // PMM1003ID
    SerialUSB.println(Pmm1003ID);
    SerialUSB.println(Ethernet.localIP());

    //int_output[2]++;
    //SerialUSB.println(OutRelayImage);

    SerialUSB.println("");
    SerialUSB.println(int_input[2]);
    SerialUSB.println(int_output[2]);
    // bool_output[2/8][2%8] = 1;int_input[2]
    // bool_output[6/8][6%8] = 1;

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
