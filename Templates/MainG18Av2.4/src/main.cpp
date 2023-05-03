#include <Arduino.h>
#include <ProjectDef.h>

#include <PmmCommands.h>
#include <PmmGlobalFunctions.h>

// Include your board here
#include <PMM0620.h>



void setup()
{
  //STEP00: Start usb serial for debug
  SerialUSB.begin(9600);
  
  //STEP01: Read Flash ROM and update Settings
   PmmInitializeProjectSettings();
  //STEP02: Initialize Extra needed Modules
  ThisProductSetup();
  
     
  //STEP04: Warmup 1 sec
  delay(1000);
  SerialUSB.println("New Starting ...");
  
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */

int x5 = 0;
int val;


 
void loop()
{
  PmmPowerManagerUpdate();

  if ((millis() - MainLoopTimer) > 100)
  {

   

    if (x5 >= 20)
    {
      
      x5 = 0 ; 
       SerialUSB.println("Running .. ");
      
      
      if (digitalRead(DIPPROG01) != 1)
      {
        SerialUSB.println("Enter Sleep mode");
        PmmWatchDoggy.setup(0x00);
        //PmmRTCInternal.standbyMode();
        //LowPower.sleep();

      } 
    }
    
    PmmIO.Inputs[24]++;// Alive flage PmmIO.Inputs[24]
    // PmmIO.Inputs[1]++;// Alive flage PmmIO.Inputs[24]
    
   
  x5++;
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
  if ((millis() - CommunicationTimer) >100)
  {
    // Manage product and Extension Boards
    ThisProductUpdate();
    AllExtensionBoarsdUpdate();
    // Manage Modbus Servers
    // for (int SerialPort = 0 ;SerialPort < 5 ; SerialPort++ )
    // {
    //   if (ThisProduct.PmmSerial[SerialPort].Enabled & 
    //       ThisProduct.PmmSerial[SerialPort].PmmProtocols.IsRunning &
    //       ThisProduct.PmmSerial[SerialPort].PmmProtocols.ModBusSlave)
    //   ModbusPort[SerialPort].ModbusServersUpdate(SerialPort);
    // }

    ModbusPort[1].ModbusServersUpdate(1);  
    

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
