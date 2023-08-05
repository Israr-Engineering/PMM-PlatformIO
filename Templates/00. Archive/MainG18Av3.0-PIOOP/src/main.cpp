#include <Arduino.h>
#include <ProjectDef.h>

// Include your board here
//#include <PMM0628.h> // Tested
//#include <PMM0620.h> // Tested
//#include <PMM0625.h> // Tested
//#include <PMM0636.h> // Tested need some review 
//#include <PMM0620X.h> // Tested
//#include <PMM0626X.h> // Tested need some review 
#include <PMM1104.h>

// #include <PmmCommands.h>
// #include <PmmGlobalFunctions.h>

void setup()
{
  
  //STEP02: Initialize Product IOs
  hardwareInit();

  
  //STEP04: Warmup 1 sec
  delay(1000);
  SerialUSB.println("New Starting ...");
  
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */




 
void loop()
{
  //PmmPowerManagerUpdate();

  if ((millis() - MainLoopTimer) > 1000)
  {
   
   ///////////////////////////////////////////////////////////////

    
     

    MainLoopTimer = millis();
  }
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 02 : Configuration and commands updating    //
/////////////////////////////////////////////////// */

void PMMConfiguration()
{
  if ((millis() - ConfigurationTimer) > 200)
  {
    // StartCommandHttpServer(); // this Should be here always
    // PMMReadCommands();
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
    updateOutputBuffers();

    //ThisExtensionBoarsUpdate();


    // Manage Protocol Servers
    //ThisProtocolUpdate();
    
    

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
