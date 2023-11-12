/* 
   Board : 1. PMM0638xp 12x AnalogInputs and 12x AnalogOutputs (12bit) 
   Name On Bus : 1638
   Firmware : 1.1
   Date : 20230420
  Auther : MZ
  Variant : pmm0632xp
  Note : this board needs special care for memory size issue
 */

#ifndef PMM0638XP_H
#define PMM0638XP_H

#include <PmmDef.h>

int PMM_AI_Pins[13] = {};
int PMM_AO_Pins[13] = {};

int ProductName ;
bool ReadyToUse = false;
uint8_t MyI2CAddress = 8 ;
int sensorValue = 0;
int RUN_LED = 39;
int Fault_LED = 46;
bool BlinkFlag = false;
bool Fault_Flag = false ;

void ThisDeviceSetup()
{


      if (!ReadyToUse) 
      {
        MyI2CAddress = GetMyI2CAddress(2, 38, 22);
        
        // AO Pins
        PMM_AI_Pins[0] = A1;
        PMM_AI_Pins[1] = A1;PMM_AI_Pins[2] = A2;PMM_AI_Pins[3] = A3;PMM_AI_Pins[4] = A4;PMM_AI_Pins[5] = A5;PMM_AI_Pins[6] = A6;
        PMM_AI_Pins[7] = A7;PMM_AI_Pins[8] = A8;PMM_AI_Pins[9] = A9;PMM_AI_Pins[10] = A10;PMM_AI_Pins[11] = A11;PMM_AI_Pins[12] = A12;
        
        PMM_AO_Pins[1] = A13;PMM_AO_Pins[2] = A14;PMM_AO_Pins[3] = A15;PMM_AO_Pins[4] = A16;PMM_AO_Pins[5] = A1;PMM_AO_Pins[6] = A2;
        PMM_AO_Pins[7] = A3;PMM_AO_Pins[8] = A4;PMM_AO_Pins[9] = A5;PMM_AO_Pins[10] = A6;PMM_AO_Pins[11] = A7;PMM_AO_Pins[12] = A8;
             
        // LEDs Pins
        RUN_LED = 39;        pinMode(RUN_LED,OUTPUT);
        Fault_LED = 46;      pinMode(Fault_LED,OUTPUT);
        // Other Switch Pins
         
        // Setup PWM
        for (uint8_t i = 0; i < 13; i++) pinMode(PMM_AO_Pins[i], OUTPUT);

        // ADC & DAC Resolution
        analogReadResolution(12);
        analogWriteResolution(16);
       
        ReadyToUse = true;

      }

}

void ThisDeviceUpDate()
{
           //write My Address to SerialUSB
              SerialUSB.print("My I2C Address : ");
              SerialUSB.println(MyI2CAddress);
                  
       if (ReadyToUse)
       {
       
        for (int x=0 ;x < 13 ;x++)
         {
              //PMM_AO_Pins[]
                                            
              int tmpIndex = 2 * x ;
              int tmpInt = BytesToInt(PmmIO.Inputs[tmpIndex],PmmIO.Inputs[tmpIndex+1]);
              analogWrite(PMM_AO_Pins[x], tmpInt);

              sensorValue = analogRead(PMM_AI_Pins[x]);
              
              
              //write to SerialUSB
              SerialUSB.print(" =>A0[");
              SerialUSB.print(x);
              SerialUSB.print("]=");
              SerialUSB.print(PmmIO.Outputs[x]) ;

              SerialUSB.print(" =>AI[");
              SerialUSB.print(x);
              SerialUSB.print("]=");
              SerialUSB.print(sensorValue) ;
              // write to i2c
              PmmIO.Outputs[x * 2 ] = lowByte(sensorValue);
              PmmIO.Outputs[(x *2) +1] = highByte(sensorValue);
        }
      }
          
    
}




#endif