//#include <stdlib.h>
// extern "C" {
// #include "openplc.h"
// }

#ifndef PMM0620_H
#define PMM0620_H



#include "Arduino.h"
#include <PmmTypes.h>

//OpenPLC HAL for Arduino Uno and Arduino Nano (old) form factor (Uno, Leonardo, Nano, Micro and Zero)

/******************PINOUT CONFIGURATION*******************
Digital In: 2, 3, 4, 5, 6           (%IX0.0 - %IX0.4)
Digital Out: 7, 8, 12, 13           (%QX0.0 - %QX0.3)
Analog In: A0, A1, A2, A3, A4, A5   (%IW0 - %IW5)
Analog Out: 9, 10, 11               (%QW0 - %QW2)
**********************************************************/

//Define the number of inputs and outputs for this board (mapping for the Arduino Uno)
#define NUM_DISCRETE_INPUT          17
#define NUM_ANALOG_INPUT            0
#define NUM_DISCRETE_OUTPUT         0
#define NUM_ANALOG_OUTPUT           0

//Create the I/O pin masks
uint8_t pinMask_DIN[] = {2,3,4,5,6,7,8,9,10,11,12,13,24,43,44,19,23,7};
uint8_t pinMask_AIN[] = {A0, A1, A2, A3, A4, A5};
uint8_t pinMask_DOUT[] = {7, 8, 12, 13};
uint8_t pinMask_AOUT[] = {9, 10, 11};

// Common Pins If any
static const uint8_t ETHERRST = 24ul;
static const uint8_t DIPPROG01 = 46ul ;
static const uint8_t DIPPROG02 = 47ul  ;
static const uint8_t DILOSSPOWER = 19ul  ;
static const uint8_t DIFACTORYRESET = 23ul  ; // FACTORYRESET



void hardwareInit()
{
    for (int i = 0; i < NUM_DISCRETE_INPUT; i++)
    {
		uint8_t pin = pinMask_DIN[i];
        pinMode(pin, INPUT);
    }
    
    for (int i = 0; i < NUM_ANALOG_INPUT; i++)
    {
		uint8_t pin = pinMask_AIN[i];
        pinMode(pin, INPUT);
    }
    
    for (int i = 0; i < NUM_DISCRETE_OUTPUT; i++)
    {
		uint8_t pin = pinMask_DOUT[i];
        pinMode(pin, OUTPUT);
    }

    for (int i = 0; i < NUM_ANALOG_OUTPUT; i++)
    {
		uint8_t pin = pinMask_AOUT[i];
        pinMode(pin, OUTPUT);
    }
}

void updateInputBuffers()
{
    for (int i = 0; i < NUM_DISCRETE_INPUT; i++)
    {
		uint8_t pin = pinMask_DIN[i];
        if (bool_input[i/8][i%8] != NULL) 
            *bool_input[i/8][i%8] = digitalRead(pin);
    }
    
    for (int i = 0; i < NUM_ANALOG_INPUT; i++)
    {
		uint8_t pin = pinMask_AIN[i];
        if (int_input[i] != NULL)
            *int_input[i] = (analogRead(pin) * 64);
    }
}

void updateOutputBuffers()
{
    for (int i = 0; i < NUM_DISCRETE_OUTPUT; i++)
    {
		uint8_t pin = pinMask_DOUT[i];
        if (bool_output[i/8][i%8] != NULL) 
            digitalWrite(pin, *bool_output[i/8][i%8]);
    }
    for (int i = 0; i < NUM_ANALOG_OUTPUT; i++)
    {
		uint8_t pin = pinMask_AOUT[i];
        if (int_output[i] != NULL) 
            analogWrite(pin, (*int_output[i] / 256));
    }
}

#endif
