#ifndef PROJECTDEF_H
#define PROJECTDEF_H

//#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include "ArduinoRS485/src/ArduinoRS485.h"
#include <ArduinoModbus.h>


//Global Variables

bool x = false;
long MainLoopTimer = 0;


typedef struct PMMIO
{
    // // represent descret InputStatus in modbus
    //int InputsByte[120];
    // // image for internal use
    //int InputsBytePrev[120];
    // // represent coils bits in modbus
    // bit OutputsByte[1920];
    // // image for internal use
    // bit OutputsBytePrev[1920];

    // represent Input registers in modbus
    int Inputs[128];
    // image for internal use
    int InputsPrev[128];
    // represent hodling registers in modbus
    int Outputs[128];
    // image for internal use
    int OutputsPrev[128];

} PMMIO;

PMMIO PmmIO;


void ModbusRtuServerUpdate()
{

   ModbusRTUServer.poll();

// map the holding register values to the input register values
  for (int i = 0; i < 32; i++) 
  {

     ModbusRTUServer.inputRegisterWrite(i, PmmIO.Inputs[i]);

    PmmIO.Outputs[i] = ModbusRTUServer.holdingRegisterRead(i);

   
  }


}


void ModbusRtuServerSetup()
{
  RS485.setSerial(&Serial);  
  RS485.setPins(35, 31, 36);
  // start the Modbus RTU server, with (slave) id 42
  if (!ModbusRTUServer.begin(1, 9600 ,SERIAL_8N1)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

 // configure holding registers at address 0x00
  ModbusRTUServer.configureHoldingRegisters(0x00, 0x20);
  // configure input registers at address 0x00
  ModbusRTUServer.configureInputRegisters(0x00, 0x20);
}

#endif









