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
//#include <microsmooth.h>

// int PMM_AI_Pins[17] = {A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16};
uint16_t PMM_AI_Pins[17] = {};
int ProductName = 1632;
bool ReadyToUse = false;
uint8_t MyI2CAddress = 8;
int sensorValue = 0;
int RUN_LED = 39;
int Fault_LED = 46;
bool BlinkFlag = false;
bool Fault_Flag = false;

ulong BuadRate = 19200;
uint8_t DIPROG = 37;
int slaveID = 1;

// Templates
uint32_t ptrFilter[17];
uint8_t FilterCounter = 0 ;

// ModbusRTU
// assign the Arduino pin that must be connected to RE-DE RS485 transceiver
#define TXEN 3
// data array for modbus network sharing
uint16_t au16data[64] = {
    3, 1415, 9265, 4, 2, 7182, 28182, 8, 0, 0, 0, 0, 0, 0, 1, 5};
// uint16_t Filterdata[32] = {0};
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

  //for (int i = 0 ;i < 18;i++) ptr01[i] = ms_init(EMA); // FILTERS SETUP
  // SettingsROM ThisSettingsROM;
  //  Read the content of "my_flash_store" into the "owner" variable
  // ThisSettingsROM = SettingsROM_Store.read();
  // try define the uart here
  // Serial.begin( 9600 ); // baud-rate at 9600

  pinMode(DIPROG, INPUT);
  MyI2CAddress = GetMyI2CAddress(38, 22, 57);
  slaveID = MyI2CAddress ;
  if (digitalRead(DIPROG)) slaveID = MyI2CAddress +  8 ;
  // slaveID = 1 ;
  slave.setID(slaveID); // MODBUS SLAVE ID
  MyI2CAddress = MyI2CAddress + 40;

  Serial1.begin(BuadRate, SERIAL_8N1); // 19200 baud, 8-bits, even, 1-bit stop
  slave.start();

  if (!ReadyToUse)
  {
    // ADC Resolution
    analogReadResolution(12);
    // AI Pins mapping 
    PMM_AI_Pins[0] = A1;
    PMM_AI_Pins[1] = 54;//56
    PMM_AI_Pins[2] = 56;//58
    PMM_AI_Pins[3] = 58;//55
    PMM_AI_Pins[4] = 55;//54
    PMM_AI_Pins[5] = 47;
    PMM_AI_Pins[6] = 43;
    PMM_AI_Pins[7] = 48;
    PMM_AI_Pins[8] = 49;
    PMM_AI_Pins[9] = A9;
    PMM_AI_Pins[10] = A10;
    PMM_AI_Pins[11] = A11;
    PMM_AI_Pins[12] = A12;
    PMM_AI_Pins[13] = A13;
    PMM_AI_Pins[14] = A14;
    PMM_AI_Pins[15] = A15;
    PMM_AI_Pins[16] = A16;
    // LEDs Pins
    RUN_LED = 35;
    pinMode(RUN_LED, OUTPUT);
    Fault_LED = 5;
    pinMode(Fault_LED, OUTPUT);
    ReadyToUse = true;

    // Reset Modbus
    for (int i = 0 ; i < 64 ; i++)
    {
      PmmIO.Outputs[i] = 0 ;
      au16data[i] = 0 ; 
    } 
  }
}

void ThisDeviceUpDate()
{
  

  if (ReadyToUse)
  {
    // Check filter counter 
    FilterCounter++ ; 
    // modbus
    slave.poll(au16data, 63);

    for (int x = 1; x < 17; x++)
    {
      // PMM_AI_Pins[]
      if (x == 0)
      {
        // sensorValue = 1948; // when we lost our home land :(
      }
      else if (x < 18) // && x > 0)
      {
        sensorValue = analogRead(PMM_AI_Pins[x]);
      }
      
      // Apply Filter
      //sensorValue = ema_filter(sensorValue, ptr01[x]);

      // write to SerialUSB for test only 
      // SerialUSB.print(" =>AI[");
      // SerialUSB.print(x);
      // SerialUSB.print("]=");
      // SerialUSB.print(sensorValue);

       if (FilterCounter > 50)
       {
        au16data[x-1] =(uint16_t)(ptrFilter[x] / 50) ;
        ptrFilter[x] = 0 ;
       } else
       {
        ptrFilter[x] = ptrFilter[x] + sensorValue ; 
       }

      //  write to i2c
      // PmmIO.Outputs[x * 2] = lowByte(sensorValue);
      // PmmIO.Outputs[(x * 2) + 1] = highByte(sensorValue);
      // write to ModBus RTU
      au16data[x + 23] = sensorValue;
      
    }

    // Reset filter counter 
    if (FilterCounter > 50)
       {
        FilterCounter = 0 ;
       }
    //PmmIO.Outputs[62] = 32; // board id 
  }
}

#endif