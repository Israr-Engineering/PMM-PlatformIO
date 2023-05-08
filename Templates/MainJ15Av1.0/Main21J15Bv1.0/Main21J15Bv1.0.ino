
#include <Wire.h>
#include "PmmDef.h"

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);
  // read board address and start wire
  Wire.begin(8);  // join i2c bus with address #8

  Wire.onReceive(receiveEvent);  // register event
  Wire.onRequest(requestEvent);  // register event

  delay(2000);
  SerialUSB.println("New Starting ...");
}

int ProductName;
bool ReadyToUse = false;

void loop() {
  // put your main code here, to run repeatedly:
  ProductName = BytesToInt(PmmIO.Inputs[48], PmmIO.Inputs[49]);

  // for (int i =0 ;i < 50; i++)PmmIO.Outputs[i] = PmmIO.Inputs[i] ; // Mirror inputs to outpus
  switch (ProductName) {
    case 632:
      /* Procces 0632xp */
      if (!ReadyToUse) {
        //DoBoardSetup(ProductName);
        //configer All pins as Analog input
        for (uint8_t i = 0; i < 16; i++)
        pinMode(PMM_AI_Pins[i], INPUT);

        ReadyToUse = true;

      } else {
        for (uint8_t i = 0; i < 16; i++) {
          uint8_t sensorValue = analogRead(PMM_AI_Pins[i]);

          //print out the value of the sensors:
          //String str = "AI" + String(i + 1) + "=" + String(sensorVal) + "   ";
          // SerialUSB.print(str);
          // delay(500);
        }
      }

      break;
    case 635:
      /* Procces 0635xp */
      if (!ReadyToUse) {
        //DoBoardSetup(ProductName);
        // configer All pins as Analog input
        // for (uint8_t i = 0; i < 24; i++)
        //   pinMode(PMM_AI_Pins[i], INPUT);

        ReadyToUse = true;

      } else {
        // for (uint8_t i = 0; i < 24; i++) {
        //   uint8_t sensorValue = analogRead(PMM_AI_Pins[i]);

        //   //print out the value of the sensors:
        //   String str = "AI" + String(i + 1) + "=" + String(sensorVal) + "   ";
        //   // SerialUSB.print(str);
        //   // delay(500);
        // }
      }
      break;
    case 638:
      /* Procces 0638xp */
      if (!ReadyToUse) {
        //DoBoardSetup(ProductName);
        // configer All pins as Analog input
        // for (uint8_t i = 0; i < 24; i++)
        //   pinMode(PMM_AI_Pins[i], INPUT);
        ReadyToUse = true;
      } else {
        // for (uint8_t i = 0; i < 24; i++) {
        //   uint8_t sensorValue = analogRead(PMM_AI_Pins[i]);

        //   //print out the value of the sensors:
        //   String str = "AI" + String(i + 1) + "=" + String(sensorVal) + "   ";
        //   // SerialUSB.print(str);
        //   // delay(500);
        // }
      }
      break;
    case 639:
      /* Procces 0639xp */
      if (!ReadyToUse) {
        //DoBoardSetup(ProductName);
        // configer All pins as Analog input
        // for (uint8_t i = 0; i < 24; i++)
        //   pinMode(PMM_AI_Pins[i], INPUT);

        ReadyToUse = true;

      } else {
        // for (uint8_t i = 0; i < 24; i++) {
        //   uint8_t sensorValue = analogRead(PMM_AI_Pins[i]);

        //   //print out the value of the sensors:
        //   String str = "AI" + String(i + 1) + "=" + String(sensorVal) + "   ";
        //   // SerialUSB.print(str);
        //   // delay(500);
        // }
      }
      break;

    default:
      break;
  }


  SerialUSB.print(" => ProductName : ");
  SerialUSB.println(ProductName);  // print the integer
  delay(500);
  //SerialUSB.println("New Starting ...");
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int Receivedbytes) {
  int i = 0;
  while (1 < Wire.available()) {    // loop through all but the last
    PmmIO.Inputs[i] = Wire.read();  // receive byte as a character
    //SerialUSB.print(PmmIO.Inputs[i]);         // print the character
    i++;
  }
  // int x = Wire.read();    // receive byte as an integer
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(PmmIO.Outputs, 64);
}
