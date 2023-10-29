/*
   Board : PMM0635xp  24x AnalogOutputs (12bit)
   Name On Bus : 1635
   Firmware : 1.1
   Date : 20230420
  Auther : MZ
  Variant : pmm0632xp
  Note : this board needs special care for memory size issue
 */
#ifndef PMM0635XP_H
#define PMM0635XP_H

#include <PmmDef.h>

// int PMM_AI_Pins[17] = {};
int PMM_AO_Pins[25] = {};

int ProductName;
bool ReadyToUse = false;
uint8_t MyI2CAddress = 8;
int sensorValue = 0;
int RUN_LED = 39;
int Fault_LED = 46;
bool BlinkFlag = false;
bool Fault_Flag = false;
int AnaSwitch00 = 30;
int AnaSwitch01 = 31;
int AnaSwitch02 = 26;
int AnaSwitch03 = 27;

ulong BuadRate = 19200;
uint8_t DIPROG = 37;
int slaveID = 1;

void ThisDeviceSetup()
{

      // I2c Address
      MyI2CAddress = GetMyI2CAddress(38, 22, 57);
      MyI2CAddress = MyI2CAddress + 40;
      // ADC Resolution
      analogReadResolution(12);

      if (!ReadyToUse)
      {

            // AO Pins
            PMM_AO_Pins[0] = A1;
            PMM_AO_Pins[1] = A1;
            PMM_AO_Pins[2] = A2;
            PMM_AO_Pins[3] = A3;
            PMM_AO_Pins[4] = A4;
            PMM_AO_Pins[5] = A5;
            PMM_AO_Pins[6] = A6;
            PMM_AO_Pins[7] = A7;
            PMM_AO_Pins[8] = A8;
            PMM_AO_Pins[9] = A9;
            PMM_AO_Pins[10] = A10;
            PMM_AO_Pins[11] = A11;
            PMM_AO_Pins[12] = A12;
            PMM_AO_Pins[13] = A13;
            PMM_AO_Pins[14] = A14;
            PMM_AO_Pins[15] = A15;
            PMM_AO_Pins[16] = A16;
            PMM_AO_Pins[17] = A1;
            PMM_AO_Pins[18] = A2;
            PMM_AO_Pins[19] = A3;
            PMM_AO_Pins[20] = A4;
            PMM_AO_Pins[21] = A5;
            PMM_AO_Pins[22] = A6;
            PMM_AO_Pins[23] = A7;
            PMM_AO_Pins[24] = A8;
            // PMM_AO_Pins[25] = A9;
            //  LEDs Pins
            pinMode(RUN_LED, OUTPUT);
            pinMode(Fault_LED, OUTPUT);
            // Other Switch Pins

            // Setup PWM
            for (uint8_t i = 0; i < 25; i++)
                  pinMode(PMM_AO_Pins[i], OUTPUT);

            analogWriteResolution(16);

            ReadyToUse = true;
      }
}

void ThisDeviceUpDate()
{
      // write My Address to SerialUSB
      SerialUSB.print("My I2C Address : ");
      SerialUSB.println(MyI2CAddress);

      if (ReadyToUse)
      {

            for (int x = 0; x < 25; x++)
            {
                  // PMM_AO_Pins[]

                  int tmpIndex = 2 * x;
                  int tmpInt = BytesToInt(PmmIO.Inputs[tmpIndex], PmmIO.Inputs[tmpIndex + 1]);

                  analogWrite(PMM_AO_Pins[x], tmpInt);

                  // write to SerialUSB
                  SerialUSB.print(" =>A0[");
                  SerialUSB.print(x);
                  SerialUSB.print("]=");
                  SerialUSB.print(PmmIO.Outputs[x]);
                  // write to i2c
                  // PmmIO.Outputs[x * 2 ] = lowByte(sensorValue);
                  // PmmIO.Outputs[(x *2) +1] = highByte(sensorValue);
            }
      }
}

#endif