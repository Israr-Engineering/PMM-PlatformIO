
#include <Wire.h>
#include "PmmDef.h"

int ProductName ;
bool ReadyToUse = false;
uint8_t MyI2CAddress = 0x8 ;

int RnuLED = 39;


void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);
  // read board address and start wire 
  MyI2CAddress = GetMyI2CAddress(1,2,3);

  Wire.begin(MyI2CAddress);          // join i2c bus with address = > GetMyI2cAddress

  pinMode(39, OUTPUT);

  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event 

   delay(2000);
  SerialUSB.print("New Starting => I2C Address = ");SerialUSB.println(MyI2CAddress);


}

//uint8_t sensorValueArr[]={A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19};
 
 bool BlinkFlag = true ;

void loop() 
{
  // blink 
  BlinkFlag = !BlinkFlag;
  digitalWrite(39, BlinkFlag);
  // put your main code here, to run repeatedly:
   ProductName = BytesToInt(PmmIO.Inputs[48] , PmmIO.Inputs[49]) ;
    
   // for (int i =0 ;i < 50; i++)PmmIO.Outputs[i] = PmmIO.Inputs[i] ; // Mirror inputs to outpus
      
      if (ProductName == 632) ReadyToUse = true ;
      SerialUSB.println(" => ProductName : "); SerialUSB.println(ProductName);// print the integer
        
    //pinMode(15, INPUT); 
    uint16_t sensorValue = 0 ;
     sensorValue = analogRead(A6);
     PmmIO.Outputs[0] = lowByte(sensorValue);
     PmmIO.Outputs[1] = highByte(sensorValue);
     sensorValue = analogRead(A2);
     PmmIO.Outputs[2] = lowByte(sensorValue);
     PmmIO.Outputs[3] = highByte(sensorValue);
     sensorValue = analogRead(A1);
     PmmIO.Outputs[4] = lowByte(sensorValue);
     PmmIO.Outputs[5] = highByte(sensorValue);
    //  sensorValue = analogRead(A4);
    //  PmmIO.Outputs[6] = lowByte(sensorValue);
    //  PmmIO.Outputs[7] = highByte(sensorValue);
    //  sensorValue = analogRead(A5);
    //  PmmIO.Outputs[8] = lowByte(sensorValue);
    //  PmmIO.Outputs[9] = highByte(sensorValue);
    //  sensorValue = analogRead(A6);
    //  PmmIO.Outputs[10] = lowByte(sensorValue);
    //  PmmIO.Outputs[11] = highByte(sensorValue);
     //delay(1000);
    //  sensorValue = analogRead(A7);
    //  sensorValue = analogRead(A8);
    //  sensorValue = analogRead(A9);
    //  sensorValue = analogRead(A10);
    //  sensorValue = analogRead(A11);
    //  sensorValue = analogRead(A12);
    

    
    delay(1000);

    
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

