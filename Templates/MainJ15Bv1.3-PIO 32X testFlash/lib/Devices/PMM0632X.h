/* 
   Board : 1. PMM0632x 16x Analog Inputs (12bit) with modbus RTU
   Name On Bus : 2632
   Firmware : 1.1
   Date : 20230420
  Auther : MZ
  Variant : pmm0632x
  Note : this board needs special care for memory size issue
 */

#ifndef PMM0632X_H
#define PMM0632X_H

#include <ModbusRtu.h>
#include <PmmDef.h>
#include <FlashStorage.h>

// int PMM_AI_Pins[17] = {A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16};
uint16_t PMM_AI_Pins[17] = {};
int ProductName;
bool ReadyToUse = false;
uint8_t MyI2CAddress = 8;
int sensorValue = 0;
int RUN_LED = 39;
int Fault_LED = 46;
bool BlinkFlag = false;
bool Fault_Flag = false;

// int AnaSwitch00 = 30;
// int AnaSwitch01 = 31;
// int AnaSwitch02 = 26;
// int AnaSwitch03 = 27;

// Create a structure that is big enough to contain a name
// and a surname. The "valid" variable is set to "true" once
// the structure is filled with actual data for the first time.
typedef struct {
  boolean valid;
  char name[100];
  char surname[100];
} Person;

// Reserve a portion of flash memory to store a "Person" and
// call it "my_flash_store".
FlashStorage(my_flash_store, Person);

// ModbusRTU

// assign the Arduino pin that must be connected to RE-DE RS485 transceiver
#define TXEN 57

// data array for modbus network sharing
uint16_t au16data[18] = {
    3, 1415, 9265, 4, 2, 7182, 28182, 8, 0, 0, 0, 0, 0, 0, 1, 5};

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  port : serial port
 *  u8txenpin : 0 for RS-232 and USB-FTDI
 *               or any pin number > 1 for RS-485
 */
Modbus slave(1, Serial1, TXEN); // this is slave @1 and RS-485

void ThisDeviceSetup()
{


  Person owner;

  // Read the content of "my_flash_store" into the "owner" variable
  owner = my_flash_store.read();

  // try define the uart here

  // Serial.begin( 9600 ); // baud-rate at 9600
  Serial1.begin(115200, SERIAL_8N1); // 19200 baud, 8-bits, even, 1-bit stop
  slave.start();

  if (!ReadyToUse)
  {
    MyI2CAddress = GetMyI2CAddress(2, 38, 22);
    // ADC Resolution
    analogReadResolution(12);
    // AI Pins
    PMM_AI_Pins[0] = A1;
    PMM_AI_Pins[1] = A1;
    PMM_AI_Pins[2] = A2;
    PMM_AI_Pins[3] = A3;
    PMM_AI_Pins[4] = A4;
    PMM_AI_Pins[5] = A5;
    PMM_AI_Pins[6] = A6;
    PMM_AI_Pins[7] = A7;
    PMM_AI_Pins[8] = A8;
    PMM_AI_Pins[9] = A9;
    PMM_AI_Pins[10] = A10;
    PMM_AI_Pins[11] = A11;
    PMM_AI_Pins[12] = A12;
    PMM_AI_Pins[13] = A13;
    PMM_AI_Pins[14] = A14;
    PMM_AI_Pins[15] = A15;
    PMM_AI_Pins[16] = A16;
    // LEDs Pins
    RUN_LED = 39;
    pinMode(RUN_LED, OUTPUT);
    Fault_LED = 46;
    pinMode(Fault_LED, OUTPUT);

    ReadyToUse = true;
  }
}

void ThisDeviceUpDate()
{

  if (ReadyToUse)
  {
    // modbus
    slave.poll(au16data, 17);

    for (int x = 0; x < 17; x++)
    {
      // PMM_AI_Pins[]
      if (x == 0)
      {
        //sensorValue = 1948; // when we lost our home land :(
      }
      else if (x < 18 )// && x > 0)
      {
        sensorValue = analogRead(PMM_AI_Pins[x]);
      }


      // write to SerialUSB
      SerialUSB.print(" =>AI[");
      SerialUSB.print(x);
      SerialUSB.print("]=");
      SerialUSB.print(sensorValue);
      // write to i2c
      PmmIO.Outputs[x * 2] = lowByte(sensorValue);
      PmmIO.Outputs[(x * 2) + 1] = highByte(sensorValue);

      // write to ModBus RTU
      au16data[x] = sensorValue ;
    }
  }
}


#endif