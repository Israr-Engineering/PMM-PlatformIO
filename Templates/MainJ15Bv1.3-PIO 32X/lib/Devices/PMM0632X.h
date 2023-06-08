// Note : this board needs special care for memory size issue

#ifndef PMM0632X_H
#define PMM0632X_H


 //#include <ArduinoRS485.h>
 //#include <ArduinoModbus.h>

//#include <ModbusRTUSlave.h>

#include <ModbusRtu.h>

#include <PmmDef.h>

//int PMM_AI_Pins[17] = {A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16};
int PMM_AI_Pins[17] = {};
int ProductName ;
bool ReadyToUse = false;
uint8_t MyI2CAddress = 8 ;
int sensorValue = 0;
int RUN_LED = 39;
int Fault_LED = 46;
bool BlinkFlag = false;
bool Fault_Flag = false ;
int AnaSwitch00 = 30;
int AnaSwitch01 = 31;
int AnaSwitch02 = 26;
int AnaSwitch03 = 27;

// ModbusRTU 

// assign the Arduino pin that must be connected to RE-DE RS485 transceiver
#define TXEN	57 

// data array for modbus network sharing
uint16_t au16data[16] = {
  3, 1415, 9265, 4, 2, 7182, 28182, 8, 0, 0, 0, 0, 0, 0, 1, 5};

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  port : serial port
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(1,Serial1,TXEN); // this is slave @1 and RS-485



void ThisDeviceSetup()
{

      // try define the uart here 
      

       //Serial.begin( 9600 ); // baud-rate at 9600
      Serial1.begin( 9600, SERIAL_8N1 ); // 19200 baud, 8-bits, even, 1-bit stop
       slave.start();

      if (!ReadyToUse) 
      {
        MyI2CAddress = GetMyI2CAddress(2, 38, 22);
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

      }

}

void ThisDeviceUpDate()
{
   
       if (ReadyToUse)
       {
        //modbus 
         slave.poll( au16data, 16 );
       
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
              SerialUSB.print(x);
              SerialUSB.print("]=");
              SerialUSB.print(sensorValue) ;
              // write to i2c
              PmmIO.Outputs[x * 2 ] = lowByte(sensorValue);
              PmmIO.Outputs[(x *2) +1] = highByte(sensorValue);
        }
      }
          
    
}


// void ModbusRtuServerUpdate()
// {

//    ModbusRTUServer.poll();

// // map the holding register values to the input register values
//   for (int i = 0; i < 32; i++) 
//   {

//      ModbusRTUServer.inputRegisterWrite(i, PmmIO.Inputs[i]);

//     //PmmIO.Outputs[i] = ModbusRTUServer.holdingRegisterRead(i);

   
//   }


// }


void ModbusRtuServerSetup()
{



      
//   RS485.setSerial(&Serial);  
//   RS485.setPins(35, 31, 36);
//   // start the Modbus RTU server, with (slave) id 42
//   if (!ModbusRTUServer.begin(1, 9600 ,SERIAL_8N1)) {
//     //Serial.println("Failed to start Modbus RTU Server!");
//     while (1);
//   }

//  // configure holding registers at address 0x00
//   //ModbusRTUServer.configureHoldingRegisters(0x00, 0x20);
//   // configure input registers at address 0x00
//   ModbusRTUServer.configureInputRegisters(0x00, 0x20);
}



#endif