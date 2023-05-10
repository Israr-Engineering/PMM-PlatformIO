/* 
   Board : PMM0625 8x Digital output
Firmware : 1.1
    Date : 20230429
  Auther : MZ
 */
#ifndef PMM0625_H
#define PMM0625_H

#include "Arduino.h"
#include <PmmTypes.h>
#include <PmmCommands.h>

extern void ThisProductSetup();
extern void ThisProductUpdate();

// Call this once at main setup function
void ThisProductSetup()
{
    // configure all pins  as an input and enable the internal pull-up resistor
    // for (uint8_t i = 0; i < 12; i++)
    //     pinMode(PMM_DI_Pins[i], INPUT_PULLUP);
}

// Manage this in the main loop
void ThisProductUpdate()
{

    // save the digital inputs values into a int16 variable 
    int tmpInputs = 0;
    String str = "";

    // for (uint8_t i = 0; i < 12; i++)
    // {
    //     bool sensorVal = digitalRead(PMM_DI_Pins[i]);

    //     sensorVal = !sensorVal;

        
    //     if(sensorVal) 
    //     {
    //         tmpInputs |= (1 << i); //x |= (1 << n); forces nth bit of x to be 1.  all other bits left alone.
    //     }
    //     else 
    //     {
    //         tmpInputs &= ~(1 << i); //x &= ~(1 << n);  // forces nth bit of x to be 0.
    //     }
        
    // }

    // PmmIO.InputsByte[0] = lowByte(tmpInputs) ;
    // PmmIO.InputsByte[1] = highByte(tmpInputs);

    //SerialUSB.println("");

}



#endif