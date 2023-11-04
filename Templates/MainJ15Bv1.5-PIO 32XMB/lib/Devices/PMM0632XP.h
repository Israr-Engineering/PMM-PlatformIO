/*
   Board : 1. PMM0632xp 24x AnalogInputs  (12bit)
   Name On Bus : 1632
   Firmware : 1.1
   Date : 20230420
  Auther : MZ
  Variant : pmm0632xp
  Note : this board needs special care for memory size issue
 */

#ifndef PMM0632XP_H
#define PMM0632XP_H

#include <PmmDef.h>
#include <microsmooth.h>

// int PMM_AI_Pins[17] = {A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16};
int PMM_AI_Pins[17] = {};
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

// Templates
uint16_t *ptr01[25];

void ThisDeviceSetup()
{
  for (int i = 0; i < 25; i++)
    ptr01[i] = ms_init(EMA);

  // I2c Address
  MyI2CAddress = GetMyI2CAddress(38, 22, 57);
  MyI2CAddress = MyI2CAddress + 40;
  // ADC Resolution
  analogReadResolution(12);

  if (!ReadyToUse)
  {

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
    PMM_AI_Pins[13] = A14;
    PMM_AI_Pins[14] = A13;
    PMM_AI_Pins[15] = A15;
    PMM_AI_Pins[16] = A16;
    // LEDs Pins
    RUN_LED = 35;
    pinMode(RUN_LED, OUTPUT);
    Fault_LED = 5;
    pinMode(Fault_LED, OUTPUT);
    // Analog Switch Pins
    AnaSwitch00 = 30;
    pinMode(AnaSwitch00, OUTPUT);
    AnaSwitch01 = 31;
    pinMode(AnaSwitch01, OUTPUT);
    AnaSwitch02 = 26;
    pinMode(AnaSwitch02, OUTPUT);
    AnaSwitch03 = 27;
    pinMode(AnaSwitch03, OUTPUT);

    ReadyToUse = true;
  }
}

void ThisDeviceUpDate()
{

  if (ReadyToUse)
  {

    for (int x = 1; x < 25; x++)
    {
      // PMM_AI_Pins[]
      if (x == 0)
      {
        sensorValue = 1948; // when we lost our home land :(
      }
      else if (x < 15 && x > 0)
      {
        sensorValue = analogRead(PMM_AI_Pins[x]);
      }
      else if (x == 23)
      {
        digitalWrite(AnaSwitch03, LOW);
        delay(10);
        sensorValue = analogRead(PMM_AI_Pins[16]); // AIMZ2
      }
      else if (x == 16)
      {
        digitalWrite(AnaSwitch03, HIGH);
        delay(10);
        sensorValue = analogRead(PMM_AI_Pins[16]); // AIMZ2
      }
      else if (x == 15)
      {
        digitalWrite(AnaSwitch00, LOW);
        digitalWrite(AnaSwitch01, HIGH);
        digitalWrite(AnaSwitch02, LOW);
        sensorValue = analogRead(PMM_AI_Pins[15]); // AIMZ2
      }
      else if (x == 18)
      {
        digitalWrite(AnaSwitch00, HIGH);
        digitalWrite(AnaSwitch01, LOW);
        digitalWrite(AnaSwitch02, LOW);
        sensorValue = analogRead(PMM_AI_Pins[15]); // AIMZ2
      }
      else if (x == 17)
      {
        digitalWrite(AnaSwitch00, LOW);
        digitalWrite(AnaSwitch01, LOW);
        digitalWrite(AnaSwitch02, LOW);
        sensorValue = analogRead(PMM_AI_Pins[15]); // AIMZ2
      }
      else if (x == 20)
      {
        digitalWrite(AnaSwitch00, LOW);
        digitalWrite(AnaSwitch01, LOW);
        digitalWrite(AnaSwitch02, HIGH);
        sensorValue = analogRead(PMM_AI_Pins[15]); // AIMZ2
      }
      else if (x == 19)
      {
        digitalWrite(AnaSwitch00, HIGH);
        digitalWrite(AnaSwitch01, HIGH);
        digitalWrite(AnaSwitch02, LOW);
        sensorValue = analogRead(PMM_AI_Pins[15]); // AIMZ2
      }
      else if (x == 22)
      {
        digitalWrite(AnaSwitch00, LOW);
        digitalWrite(AnaSwitch01, HIGH);
        digitalWrite(AnaSwitch02, HIGH);
        sensorValue = analogRead(PMM_AI_Pins[15]); // AIMZ2
      }
      else if (x == 21)
      {
        digitalWrite(AnaSwitch00, HIGH);
        digitalWrite(AnaSwitch01, HIGH);
        digitalWrite(AnaSwitch02, HIGH);
        sensorValue = analogRead(PMM_AI_Pins[15]); // AIMZ2
      }
      else // if (x == 24)
      {
        digitalWrite(AnaSwitch00, HIGH);
        digitalWrite(AnaSwitch01, LOW);
        digitalWrite(AnaSwitch02, HIGH);
        sensorValue = analogRead(PMM_AI_Pins[15]); // AIMZ2
      }

      // write to SerialUSB
      // SerialUSB.print(" =>AI[");
      // SerialUSB.print(x);
      // SerialUSB.print("]=");
      // SerialUSB.print(sensorValue);
      // int i = pulseIn(3, HIGH, 25000);

      sensorValue = ema_filter(sensorValue, ptr01[x]);

      // write to i2c
      PmmIO.Outputs[(x - 1) * 2] = lowByte(sensorValue);
      PmmIO.Outputs[((x - 1) * 2) + 1] = highByte(sensorValue);
    }

    PmmIO.Outputs[62] = 32;
  }
}

#endif