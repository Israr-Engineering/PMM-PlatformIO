
#ifndef PMMDEF_H
#define PMMDEF_H

//#include <PmmWDTZero.h>

//#include <FlashStorage.h>

// Create a structure that is big enough to contain a Settings
//  The "valid" variable is set to "true" once
// the structure is filled with actual data for the first time.
// typedef struct {
//   bool valid;
//  unsigned long BuadRate = 9600;
//  unsigned int SerialConfig = 1023;
//  int options = 0;
// } SettingsROM;

// Reserve a portion of flash memory to store a "Person" and
// call it "SettingsROM_Store".
//FlashStorage(SettingsROM_Store, SettingsROM);



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

