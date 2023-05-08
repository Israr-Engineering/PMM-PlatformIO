
#include <Wire.h>
#include "PmmDef.h"

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);
  // read board address and start wire 
  Wire.begin(8);                // join i2c bus with address #8

  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event 

   delay(2000);
  SerialUSB.println("New Starting ...");

}

 byte sad = 10 ;
 int ProductName ;

void loop() 
{
  // put your main code here, to run repeatedly:
   ProductName = BytesToInt(PmmIO.Inputs[48] , PmmIO.Inputs[49]) ;
    
   // for (int i =0 ;i < 50; i++)PmmIO.Outputs[i] = PmmIO.Inputs[i] ; // Mirror inputs to outpus
      switch (ProductName)
      {
      case 632:
          /* code */
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
        

    SerialUSB.print(" => ProductName : "); SerialUSB.println(ProductName);         // print the integer
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

