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

  // STEP04: Warmup 1 sec
  delay(1000);
  SerialUSB.println("New Starting ...");
  // Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
  // Scheduler.startLoop(PMMTimers);
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */

void loop()
{
  // PmmPowerManagerUpdate();
  ModbusTCPServer.PmmModBusTCPServerInputRead(0);

  if ((millis() - MainLoopTimer) > 1000)
  {

    ///////////////////////////////////////////////////////////////
    // SerialUSB.println (tempRead);
    // PMM1003ID
    SerialUSB.println(Pmm1003ID);
    SerialUSB.println(Ethernet.localIP());

    int_output[2]++;

    SerialUSB.println(OutRelayImage);

    SerialUSB.println("");
    // bool_output[2/8][2%8] = 1;
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
  if ((millis() - CommunicationTimer) > 500)
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
