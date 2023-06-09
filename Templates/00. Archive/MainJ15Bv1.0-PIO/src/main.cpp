#include <Arduino.h>

#include <Wire.h>
#include "PmmDef.h"

void receiveEvent(int Receivedbytes);
void requestEvent();

//int PMM_AI_Pins[17] = {A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16};
int PMM_AI_Pins[17] = {};
int ProductName ;
bool ReadyToUse = false;
uint8_t MyI2CAddress = 0 ;
int sensorValue = 0;
int RUN_LED = 39;
int Fault_LED = 46;
bool BlinkFlag = false;
bool Fault_Flag = false ;
int AnaSwitch00 = 30;
int AnaSwitch01 = 31;
int AnaSwitch02 = 26;
int AnaSwitch03 = 27;

void setup() {
  // Put your setup code here, to run once:
  SerialUSB.begin(9600);
  // Read board address and start wire 
  // Start I2c
  MyI2CAddress = GetMyI2CAddress(2,38,22);
  Wire.begin(MyI2CAddress);     // join i2c bus with address # MyI2CAddress
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event 
  // Warmup
  delay(1000);  
  SerialUSB.println("New Starting ...");

}

 

void loop() 
{
  // put your main code here, to run repeatedly:
   ProductName = BytesToInt(PmmIO.Inputs[48] , PmmIO.Inputs[49]) ;
  
    
   // for (int i =0 ;i < 50; i++)PmmIO.Outputs[i] = PmmIO.Inputs[i] ; // Mirror inputs to outpus
      switch (ProductName)
      {
      case 632:
          /* Procces 0632xp */
      if (!ReadyToUse) 
      {
        // ADC Resolution
        analogReadResolution(12);
        // AI Pins
        PMM_AI_Pins[0] = A1;
        PMM_AI_Pins[1] = A1;PMM_AI_Pins[2] = A2;PMM_AI_Pins[3] = A3;PMM_AI_Pins[4] = A4;PMM_AI_Pins[5] = A5;PMM_AI_Pins[6] = A6;
        PMM_AI_Pins[7] = A7;PMM_AI_Pins[8] = A8;PMM_AI_Pins[9] = A9;PMM_AI_Pins[10] = A10;PMM_AI_Pins[11] = A11;PMM_AI_Pins[12] = A12;
        PMM_AI_Pins[13] = A13;PMM_AI_Pins[14] = A14;PMM_AI_Pins[15] = A15;PMM_AI_Pins[16] = A16;
        // LEDs Pins
        RUN_LED = 39;        pinMode(RUN_LED,OUTPUT);
        Fault_LED = 46;      pinMode(Fault_LED,OUTPUT);
        // Analog Switch Pins
         AnaSwitch00 = 30;  pinMode(AnaSwitch00,OUTPUT);
         AnaSwitch01 = 31;  pinMode(AnaSwitch01,OUTPUT);
         AnaSwitch02 = 26;  pinMode(AnaSwitch02,OUTPUT);
         AnaSwitch03 = 27;  pinMode(AnaSwitch03,OUTPUT);


       
        ReadyToUse = true;

      } else {
       
        for (int x=0 ;x < 25 ;x++)
         {
              //PMM_AI_Pins[]
              if (x == 0)
              {
                  sensorValue = 1948;// when we lost our home land :(
              }
              else if ( x < 15 && x > 0)
              {
                  sensorValue = analogRead(PMM_AI_Pins[x]);
              }else if (x == 15)
              {
                digitalWrite(AnaSwitch03,LOW);
                delay(10);
                sensorValue = analogRead(PMM_AI_Pins[15]); //AIMZ2

              }else if (x == 24)
              {
                digitalWrite(AnaSwitch03,HIGH);
                delay(10);
                sensorValue = analogRead(PMM_AI_Pins[15]); //AIMZ2

              }else
              {
                ////AIMZ1 => 14
                byte tmpAdd = x - 16;
                digitalWrite(AnaSwitch00,tmpAdd & 0x01);
                digitalWrite(AnaSwitch01,(tmpAdd >> 1) & 0x01);
                digitalWrite(AnaSwitch02,(tmpAdd >> 2) & 0x01);
                delay(10);
                sensorValue = analogRead(PMM_AI_Pins[15]); //AIMZ2
              }
              
              //write to SerialUSB
              SerialUSB.print(" =>AI[");
              SerialUSB.print(x+1);
              SerialUSB.print("]=");
              SerialUSB.print(sensorValue) ;
              // write to i2c
              PmmIO.Outputs[x * 2 ] = lowByte(sensorValue);
              PmmIO.Outputs[(x *2) +1] = highByte(sensorValue);
        }
      }
          break;
      case 635:
          /* code */
          break;
      case 638:
          /* code */
          break;
      case 639:
          /* code */
          break;

      default:
        
          break;
      }
      
   

    Fault_Flag = !ReadyToUse ;
     
     SerialUSB.print(" => ProductName : "); SerialUSB.print(ProductName);// print the integer MyI2CAddress
     SerialUSB.print(" => I2C Address : "); SerialUSB.println(MyI2CAddress);// print the integer MyI2CAddress
    delay(1000);

     // blink Run
   
      if (Fault_Flag)
      {
        digitalWrite(Fault_LED,HIGH);
        }
        else
        {
          BlinkFlag = !BlinkFlag ;
        digitalWrite(RUN_LED,BlinkFlag);
        digitalWrite(Fault_LED,LOW);
      }
   

  //SerialUSB.println("New Starting ...");
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int Receivedbytes) 
{
  int i =0;
  while (1 < Wire.available()) { // loop through all but the last
    PmmIO.Inputs[i] = Wire.read(); // receive byte as a character
    //SerialUSB.print(PmmIO.Inputs[i]);         // print the character
    i++;
  }
  // int x = Wire.read();    // receive byte as an integer
    

}

// function that executes whenever data is received from master 
// this function is registered as an event, see setup() 
void requestEvent() 
{
  Wire.write(PmmIO.Outputs , 64);
 
}

