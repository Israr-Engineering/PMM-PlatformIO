#include <Arduino.h>
#include "ProjectDef.h"
// Include your board here
//#include <PMM0628.h> // Tested
#include "PMM0620.h"// Tested
//#include <PMM0625.h> // Tested



void setup()
{
  //STEP00: Start usb serial for debug
  SerialUSB.begin(9600);
  //STEP02: Initialize Extra needed Modules
  ThisProductSetup();
  //STEP03 : Modbus Rtu Server
  ModbusRtuServerSetup();

  //for test only 
  
  
  // end for test only 
      
  //STEP04: Warmup 1 sec
  delay(1000);
  SerialUSB.println("New Starting ...");
  
}


/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */

 
void loop()
{
  

  if ((millis() - MainLoopTimer) > 500)
  {
   
   ///////////////////////////////////////////////////////////////

   // Manage product and Extension Boards
    ThisProductUpdate();
    ModbusRtuServerUpdate();
      
    
    PmmIO.Inputs[30]++;// Alive flage PmmIO.Inputs[24]
    // PmmIO.Inputs[1]++;// Alive flage PmmIO.Inputs[24]
    // SerialUSB.println( PmmIO.Outputs[29]);  

      MainLoopTimer = millis();
  }
 
}


