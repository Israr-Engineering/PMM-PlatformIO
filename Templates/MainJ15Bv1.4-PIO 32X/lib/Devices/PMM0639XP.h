/* 
   Board : 1. PMM0639xp 12x DigitalInputs 6x Digitaloutputs 4x AnalogInputs and 2x AnalogOutputs (12bit) 
   Name On Bus : 1639
   Firmware : 1.1
   Date : 20230420
  Auther : MZ
  Variant : pmm0632xp
  Note : this board needs special care for memory size issue
 */

#ifndef PMM0639XP_H
#define PMM0639XP_H

#include <PmmDef.h>

int PMM_DI_Pins[13] = {};
int PMM_DO_Pins[7] = {};
int PMM_AI_Pins[5] = {};
int PMM_AO_Pins[3] = {};

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
        
        // DI Pins
        PMM_DI_Pins[1] = 1 ;PMM_DI_Pins[2] = 2 ;PMM_DI_Pins[3] = 3 ;PMM_DI_Pins[4] = 4 ;PMM_DI_Pins[5] = 5 ;PMM_DI_Pins[6] = 6 ;
        PMM_DI_Pins[7] = 7 ;PMM_DI_Pins[8] = 8 ;PMM_DI_Pins[9] = 9 ;PMM_DI_Pins[10] = 10 ;PMM_DI_Pins[11] = 11 ;PMM_DI_Pins[12] = 12 ;
        // DO Pins
        PMM_DO_Pins[1] = 1; PMM_DO_Pins[2] = 2 ; PMM_DO_Pins[3] = 3; PMM_DO_Pins[4] = 4 ; PMM_DO_Pins[5] = 5 ; PMM_DO_Pins[6] = 6 ;
        // AI Pins
        PMM_AI_Pins[0] = A1;
        PMM_AI_Pins[1] = A1;PMM_AI_Pins[2] = A2;PMM_AI_Pins[3] = A3;PMM_AI_Pins[4] = A4;
        // AO Pins
        PMM_AO_Pins[1] = A13;PMM_AO_Pins[2] = A14;
        
       
             
        // LEDs Pins
        RUN_LED = 39;        pinMode(RUN_LED,OUTPUT);
        Fault_LED = 46;      pinMode(Fault_LED,OUTPUT);
        // Other Switch Pins
         
        // Setup Pins direction
        for (uint8_t i = 0; i < 13; i++) pinMode(PMM_DI_Pins[i], INPUT);
        for ( uint8_t i = 0; i < 7; i++) pinMode(PMM_DO_Pins[i], OUTPUT);

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
              //AO
              if (x < 3)
              {
                int tmpIndex = 2 * x ;
              int tmpInt = BytesToInt(PmmIO.Inputs[tmpIndex],PmmIO.Inputs[tmpIndex+1]);
              analogWrite(PMM_AO_Pins[x], tmpInt);

              //write to SerialUSB
              SerialUSB.print(" =>A0[");
              SerialUSB.print(x);
              SerialUSB.print("]=");
              SerialUSB.print(PmmIO.Outputs[x]) ;

              }

              //AI
              if (x < 5)
              {
                int sensorValueAI = analogRead(PMM_AI_Pins[x]);
             
              SerialUSB.print(" =>AI[");
              SerialUSB.print(x);
              SerialUSB.print("]=");
              SerialUSB.print(sensorValueAI) ;
              // write to i2c
              PmmIO.Outputs[x * 2 ] = lowByte(sensorValueAI);
              PmmIO.Outputs[(x *2) +1] = highByte(sensorValueAI);
              }

              //DO
              if(x < 7)
              {
                  int tmpInt2 = BytesToInt(PmmIO.Inputs[28],PmmIO.Inputs[29]);

                  digitalWrite(PMM_DO_Pins[x],bitRead(tmpInt2,x));

                  //write to SerialUSB
                  SerialUSB.print(" =>DO[");
                  SerialUSB.print(x);
                  SerialUSB.print("]=");
                  SerialUSB.print(bitRead(tmpInt2,x)) ;

              }

              //DI
             bitWrite(sensorValue,x, PMM_DI_Pins[x]);
             
              //write to SerialUSB
              SerialUSB.print(" =>DI[");
              SerialUSB.print(x);
              SerialUSB.print("]=");
              SerialUSB.print(PmmIO.Outputs[x]) ;
              // write to i2c
              PmmIO.Outputs[26 ] = lowByte(sensorValue);
              PmmIO.Outputs[27] = highByte(sensorValue);
        }
      }
          
    
}




#endif