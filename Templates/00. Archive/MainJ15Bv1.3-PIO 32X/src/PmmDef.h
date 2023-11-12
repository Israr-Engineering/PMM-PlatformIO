
#ifndef PMMDEF_H
#define PMMDEF_H

//#include <PmmWDTZero.h>



typedef struct PMMIO
{
    // represent Input registers in modbus
    byte Inputs[64];
    // image for internal use
    //int InputsPrev[64];
    // represent hodling registers in modbus
    byte Outputs[64];
    // image for internal use
    //int OutputsPrev[64];

} PMMIO;

PMMIO PmmIO;


int BytesToInt(byte IntLowByte, byte IntHighByte)
{
    int tmpInt = IntHighByte;
    tmpInt = (tmpInt << 8) | IntLowByte;

    return tmpInt;
}

uint8_t GetMyI2CAddress(uint8_t Pin00 ,uint8_t Pin01 , uint8_t Pin02)
{
  uint8_t tmp =0 ;

  pinMode(Pin00,INPUT);
  pinMode(Pin01,INPUT);
  pinMode(Pin02,INPUT);
  
  tmp = digitalRead(Pin02);
  tmp = (tmp << 1) | digitalRead(Pin01);
  tmp = (tmp << 1) | digitalRead(Pin00);

  return tmp ;
}


#endif

