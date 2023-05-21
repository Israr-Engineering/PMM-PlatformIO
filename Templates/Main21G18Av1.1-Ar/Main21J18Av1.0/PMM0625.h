/* 
   Board : PMM0620 12x Digital Inputs
Firmware : 1.1
    Date : 20230420
  Auther : MZ
 */
#ifndef PMM0625_H
#define PMM0625_H

// #include "Arduino.h"
// #include <PmmTypes.h>
// #include <PmmCommands.h>

//#include "Arduino.h"
#include "ProjectDef.h" // Change to types for platformIO
//#include <PmmCommands.h>

extern void ThisProductSetup();
extern void ThisProductUpdate();

// Start Pin Configration for this board 
//static const uint8_t PMM_DI_Pins[8] = {2ul,3ul,4ul,5ul,6ul,7ul,8ul,9ul};
static const uint8_t PMM_DO_Pins[8] = {23ul,24ul,25ul,2ul,3ul,4ul,5ul,6ul};
static const uint8_t ETHERRST = 20ul;
static const uint8_t DIPPROG01 = 0ul ;
static const uint8_t DIPPROG02 = 1ul  ;
static const uint8_t DILOSSPOWER = 9ul  ;
static const uint8_t DIFACTORYRESET = 22ul  ;
// End Pin Configration for this board 


// Call this once at main setup function
void ThisProductSetup()
{
    // configure all pins  as an input and enable the internal pull-up resistor
    //for (uint8_t i = 0; i < 8; i++) pinMode(PMM_DI_Pins[i], INPUT_PULLUP);
    //STEP03: configure options pin
    pinMode(DIPPROG01,INPUT);
    pinMode(DIPPROG02,INPUT);
    pinMode(DILOSSPOWER,INPUT); 
    pinMode(DIFACTORYRESET,INPUT);  
    // Output Pins
    for (uint8_t i = 0; i < 8; i++) pinMode(PMM_DO_Pins[i], OUTPUT);
    for (uint8_t i = 0; i < 8; i++) digitalWrite(PMM_DO_Pins[i], LOW);
  

}

// Manage this in the main loop
void ThisProductUpdate()
{

    // Inputs
    int tmpInputs = 0;
      
    byte tmpByte = 0;

    bitWrite(tmpByte ,0,digitalRead(DIPPROG01));
    bitWrite(tmpByte ,1,digitalRead(DIPPROG02));
    bitWrite(tmpByte ,2,digitalRead(DILOSSPOWER));
    bitWrite(tmpByte ,3,digitalRead(DIFACTORYRESET));

    tmpInputs = tmpByte;
    PmmIO.Inputs[1] = tmpInputs; 

    // outputs

    int tmpOutput = PmmIO.Outputs[0] ;
    //SerialUSB.println(tmpOutput);

    for (uint8_t i = 0; i < 8; i++)
    {
       
        if (tmpOutput & 0x0001) 
        {
            digitalWrite(PMM_DO_Pins[i] , HIGH);
        }else
        {
            digitalWrite(PMM_DO_Pins[i] , LOW);
        }
         tmpOutput =tmpOutput >> 1 ;
        
    }



}



#endif