//#include <stdlib.h>
// extern "C" {
// #include "openplc.h"
// }

#ifndef PMM0620_H
#define PMM0620_H



#include "Arduino.h"
#include <PmmTypes.h>
#include <PmmPCA95x5.h>

PCA9555 IOext[6] =
    {
        PCA9555(),
        PCA9555(),
        PCA9555(),
        PCA9555(),
        PCA9555(),
        PCA9555()
    };


/******************PINOUT CONFIGURATION*******************
Digital In: 2, 3, 4, 5, 6           (%IX0.0 - %IX0.4)
Digital Out: 7, 8, 12, 13           (%QX0.0 - %QX0.3)
Analog In: A0, A1, A2, A3, A4, A5   (%IW0 - %IW5)
Analog Out: 9, 10, 11               (%QW0 - %QW2)
**********************************************************/

//Define the number of inputs and outputs for this board (mapping for the 1104)
#define NUM_DISCRETE_INPUT          27
#define NUM_ANALOG_INPUT            1
#define NUM_DISCRETE_OUTPUT         56
#define NUM_ANALOG_OUTPUT           4

//Create the I/O pin masks
uint8_t pinMask_DIN[] = {2,3,4,5,6,7,8,9,10,11,12,13,24,43,44,19,23,7};
uint8_t pinMask_AIN[] = {A0, A1, A2, A3, A4, A5};
uint8_t pinMask_DOUT[] = {7, 8, 12, 13};
uint8_t pinMask_AOUT[] = {9, 10, 11};

// Common Pins If any (Pin Mask)
static const uint8_t ETHERRST = 24ul;
static const uint8_t ETHERINT = 46ul ;
static const uint8_t TESTLEDS = 47ul  ;
static const uint8_t DILOSSPOWER = 19ul  ;
static const uint8_t INT01 = 24ul;
static const uint8_t INT02 = 24ul;
static const uint8_t INT03 = 24ul;
static const uint8_t INT04 = 24ul;
static const uint8_t INT05 = 24ul;



// Dip Switches (0/1)

// Dip[0] // A0
// Dip[1] // A1
// Dip[2] // A2
// Dip[3] // A3
// Dip[4] // A4
uint8_t PMM1104ID = 1 ;

// Dip[5] // (Normal operation / FactoryDefualt )  use (Buadrate = 115200, 8N1 ) or read from internal flash
bool FactoryDefualt = false ;
// Dip[6] // (RTU / TCP)
bool ModbusTCPMode = false ;
// Dip[7] // (Slave IO / IO mirror master) , in master mode use address 31 by default
bool IOMirrorMode = false ; 
// Dip[8] // (No GSM / Use GSM backup)
bool GSMBackupMode = false ;
// Dip[9] // (Normal opration / Setup mode) ,Save settings to falsh on change to 0
bool SetupMode = false ;
// Dip[10] // Rs485 Termination resistor 

void PCAExtensionSetup(uint8_t index,uint8_t BoardAddress, uint16_t direction)
{

    Wire.begin();
            IOext[index].attach(Wire, BoardAddress);
            IOext[index].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index].direction(direction);
            IOext[index].write(PCA95x5::Level::L_ALL);


}

void hardwareInit()
{
    // PCA i2c Extension
    PCAExtensionSetup(0,0x20,0x0000); // LEDs
    PCAExtensionSetup(1,0x21,0x0000); // LEDs
    PCAExtensionSetup(2,0x22,0x00FF); // LEDs + DIP switches
    PCAExtensionSetup(3,0x23,0xFFFF); // Inputs
    PCAExtensionSetup(4,0x24,0x0000); // Outputs

    // other pins setup 
    pinMode(ETHERRST, OUTPUT);
    pinMode(ETHERINT, INPUT);
    pinMode(TESTLEDS, INPUT);
    pinMode(DILOSSPOWER, INPUT);
    pinMode(INT01, INPUT);
    pinMode(INT02, INPUT);
    pinMode(INT03, INPUT);
    pinMode(INT04, INPUT);
    pinMode(INT05, INPUT);


    //STEP02 : Get device address and load variables and check factory Defaults
    uint16_t tmpint = 0 ;
    tmpint = IOext[2].read();
    PMM1104ID = lowByte(tmpint) & 0x1f;

    //STEP03 : check and update Modes 


    //STEP04 : Run Modbus Server/Client as per mode 


    //STEP05 : Save the status of everthing 

    // for (int i = 0; i < NUM_DISCRETE_INPUT; i++)
    // {
	// 	uint8_t pin = pinMask_DIN[i];
    //     pinMode(pin, INPUT);
    // }
    
    // for (int i = 0; i < NUM_ANALOG_INPUT; i++)
    // {
	// 	uint8_t pin = pinMask_AIN[i];
    //     pinMode(pin, INPUT);
    // }
    
    // for (int i = 0; i < NUM_DISCRETE_OUTPUT; i++)
    // {
	// 	uint8_t pin = pinMask_DOUT[i];
    //     pinMode(pin, OUTPUT);
    // }

    // for (int i = 0; i < NUM_ANALOG_OUTPUT; i++)
    // {
	// 	uint8_t pin = pinMask_AOUT[i];
    //     pinMode(pin, OUTPUT);
    // }
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
