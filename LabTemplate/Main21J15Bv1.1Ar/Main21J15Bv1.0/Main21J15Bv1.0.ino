
#include <Wire.h>
#include "PmmDef.h"

int ProductName ;
bool ReadyToUse = false;
uint8_t MyI2CAddress = 0x8 ;




void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);
  // read board address and start wire 
  MyI2CAddress = GetMyI2CAddress(1,2,3);

  Wire.begin(MyI2CAddress);          // join i2c bus with address = > GetMyI2cAddress

  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event 

   delay(2000);
  SerialUSB.print("New Starting => I2C Address = ");SerialUSB.println(MyI2CAddress);


}

 

void loop() 
{
  // put your main code here, to run repeatedly:
   ProductName = BytesToInt(PmmIO.Inputs[48] , PmmIO.Inputs[49]) ;
    
   // for (int i =0 ;i < 50; i++)PmmIO.Outputs[i] = PmmIO.Inputs[i] ; // Mirror inputs to outpus
      
      if (ProductName == 632) ReadyToUse = true ;
      SerialUSB.println(" => ProductName : "); SerialUSB.println(ProductName);// print the integer
        
    // pinMode(14, INPUT); 
    // uint16_t sensorValue = analogRead(14);
    // SerialUSB.print("AI = ");
    // SerialUSB.println(sensorValue) ;

    
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

