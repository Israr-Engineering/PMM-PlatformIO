#include <Arduino.h>
 #include <PMM0632XP.h>
// #include <PMM0632X.h>
// #include <PMM0635XP.h>
// #include <PMM0638XP.h>
// #include <PMM0639XP.h>

#include <Wire.h>
#include "PmmDef.h"

void receiveEvent(int Receivedbytes);
void requestEvent();

void setup()
{
  // Put your setup code here, to run once:
  SerialUSB.begin(9600);
  // Setup Watch dog
  // PmmWatchDoggy.setup(WDT_SOFTCYCLE8S);
  // Read board address and start wire
  ThisDeviceSetup();

  // Start I2c
  Wire.begin(MyI2CAddress);     // join i2c bus with address # MyI2CAddress
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  // Warmup
  delay(1000);
  SerialUSB.println("New Starting ...");
}

u_int32_t MainLoopTimer = 0;
void loop()
{
  // put your main code here, to run repeatedly:
  // PmmWatchDoggy.clear();

  //ProductName = BytesToInt(PmmIO.Inputs[48], PmmIO.Inputs[49]);
  // for (int i =0 ;i < 50; i++)PmmIO.Outputs[i] = PmmIO.Inputs[i] ; // Mirror inputs to outpus
  ThisDeviceUpDate();

  Fault_Flag = !ReadyToUse;

  // STEP04 : Do anything you need in 1 second base timer
  // delay(1000);
  if ((millis() - MainLoopTimer) > 1000)
  {

    SerialUSB.print(" => ProductName : ");
    SerialUSB.print(ProductName); // print the integer MyI2CAddress
    SerialUSB.print(" => I2C Address : ");
    SerialUSB.print(MyI2CAddress); // print the integer MyI2CAddress
    SerialUSB.print(" => MB SlaveID : ");
    SerialUSB.print(slaveID); // print the integer MyI2CAddress
    SerialUSB.print(" => Buadrate : ");
    SerialUSB.print(BuadRate); // print the integer MyI2CAddress
    SerialUSB.println(" => Serial settings : SERIAL_8N1 ");
    //SerialUSB.println(MyI2CAddress); // print the integer MyI2CAddress
    // blink Run
    if (Fault_Flag)
    {
      digitalWrite(Fault_LED, HIGH);
    }
    else
    {
      BlinkFlag = !BlinkFlag;
      digitalWrite(RUN_LED, BlinkFlag);
      digitalWrite(Fault_LED, LOW);
    }

    MainLoopTimer = millis();
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int Receivedbytes)
{
  int i = 0;

  while (1 < Wire.available())
  {                                // loop through all but the last
    PmmIO.Inputs[i] = Wire.read(); // receive byte as a character
    // SerialUSB.print(PmmIO.Inputs[i]);         // print the character
    i++;
  }
  // int x = Wire.read();    // receive byte as an integer
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent()
{
  Wire.write(PmmIO.Outputs, 64);
  //SerialUSB.println("REPLAY TO I2C ..");
}
