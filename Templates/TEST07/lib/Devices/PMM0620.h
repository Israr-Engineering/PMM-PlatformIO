#ifndef PMM0620_H
#define PMM0620_H

#include "Arduino.h"
#include <PmmTypes.h>
#include <PmmCommands.h>

extern void ThisProductSetup();
extern void ThisProductUpdate();
extern void ThisProductExt();

// Call this once at main setup function
void ThisProductSetup()
{

    ThisProduct.PmmGeneral.DeviceName = 0620;

    // configure all pins  as an input and enable the internal pull-up resistor
    for (uint8_t i = 0; i < 12; i++)
        pinMode(PMM_DI_Pins[i], INPUT_PULLUP);
}

// Manage this in the main loop
void ThisProductUpdate()
{

    // save the digital inputs values into a variable and print it's status

    String str = "";

    for (uint8_t i = 0; i < 12; i++)
    {
        bool sensorVal = digitalRead(PMM_DI_Pins[i]);

        sensorVal = !sensorVal;

        // print out the value of the pushbutton
        str = "DI" + String(i + 1) + "=" + String(sensorVal) + "   ";
        SerialUSB.print(str);
    }
    SerialUSB.println("");
}

// Manage Extensions boards
void ThisProductExt()
{
}

#endif