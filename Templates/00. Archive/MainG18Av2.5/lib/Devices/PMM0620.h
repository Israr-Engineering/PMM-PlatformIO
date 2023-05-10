/* 
   Board : PMM0620 12x Digital Inputs
Firmware : 1.1
    Date : 20230420
  Auther : MZ
 */
#ifndef PMM0620_H
#define PMM0620_H

#include "Arduino.h"
#include <PmmTypes.h>
#include <PmmCommands.h>

extern void ThisProductSetup();
extern void ThisProductUpdate();

// Call this once at main setup function
void ThisProductSetup()
{
    // configure all pins  as an input and enable the internal pull-up resistor
    for (uint8_t i = 0; i < 12; i++)
        pinMode(PMM_DI_Pins[i], INPUT_PULLUP);
    //STEP03: configure options pin
    pinMode(DIPPROG01,INPUT);
    pinMode(DIPPROG02,INPUT);
    pinMode(DILOSSPOWER,INPUT); 
    pinMode(FACTORYRESET,INPUT);    

}

// Manage this in the main loop
void ThisProductUpdate()
{

    // save the digital inputs values into a int16 variable 
    int tmpInputs = 0;
    String str = "";

    for (uint8_t i = 0; i < 12; i++)
    {
        bool sensorVal = digitalRead(PMM_DI_Pins[i]);

        sensorVal = !sensorVal;

        
        if(sensorVal) 
        {
            tmpInputs |= (1 << i); //x |= (1 << n); forces nth bit of x to be 1.  all other bits left alone.
        }
        else 
        {
            tmpInputs &= ~(1 << i); //x &= ~(1 << n);  // forces nth bit of x to be 0.
        }
        
    }
     
    // update modbus input register
    PmmIO.Inputs[0] = tmpInputs; 
    tmpInputs = 0 ;
    byte tmpByte = 0;

    bitWrite(tmpByte ,0,digitalRead(DIPPROG01));
    bitWrite(tmpByte ,1,digitalRead(DIPPROG02));
    bitWrite(tmpByte ,2,digitalRead(DILOSSPOWER));
    bitWrite(tmpByte ,3,digitalRead(FACTORYRESET));

    tmpInputs = tmpByte;
    PmmIO.Inputs[1] = tmpInputs; 


    // PmmIO.InputsByte[0] = lowByte(tmpInputs) ;
    // PmmIO.InputsByte[1] = highByte(tmpInputs);

    // // update control signals
    //  bitWrite(PmmIO.InputsByte[2] ,0,digitalRead(DIPPROG01));
    //  bitWrite(PmmIO.InputsByte[2] ,1,digitalRead(DIPPROG02));
    //  bitWrite(PmmIO.InputsByte[2] ,2,digitalRead(DILOSSPOWER));
    //  bitWrite(PmmIO.InputsByte[2] ,3,digitalRead(FACTORYRESET));

}



#endif