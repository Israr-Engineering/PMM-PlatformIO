// #include <stdlib.h>
//  extern "C" {
//  #include "openplc.h"
//  }

#ifndef PMM1003_H
#define PMM1003_H

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
        PCA9555()};

/******************PINOUT CONFIGURATION*******************
Digital In: 2, 3, 4, 5, 6           (%IX0.0 - %IX0.4)
Digital Out: 7, 8, 12, 13           (%QX0.0 - %QX0.3)
Analog In: A0, A1, A2, A3, A4, A5   (%IW0 - %IW5)
Analog Out: 9, 10, 11               (%QW0 - %QW2)
**********************************************************/

// Define the number of inputs and outputs for this board (mapping for the 1104)
#define NUM_DISCRETE_INPUT 27
#define NUM_ANALOG_INPUT 1
#define NUM_DISCRETE_OUTPUT 56
#define NUM_ANALOG_OUTPUT 4

// Create the I/O pin masks
uint8_t pinMask_DIN[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 24, 43, 44, 19, 23, 7};
uint8_t pinMask_AIN[] = {A0, A1, A2, A3, A4, A5};
uint8_t pinMask_DOUT[] = {7, 8, 12, 13};
uint8_t pinMask_AOUT[] = {9, 10, 11};

// Common Pins If any (Pin Mask)
static const uint8_t ETHERRST = 32ul;
static const uint8_t ETHERINT = 18ul;
static const uint8_t TESTLEDS = 4ul;
static const uint8_t DILOSSPOWER = 19ul;
static const uint8_t INT01 = 24ul;
static const uint8_t INT02 = 25ul;
static const uint8_t INT03 = 13ul;
static const uint8_t INT04 = 2ul;
static const uint8_t INT05 = 3ul;

// Status LEDs
bool LED_Ready = false;
bool LED_COMMHealthy = false;
bool LED_IOMaster = false;
bool LED_ModbusRTU = false;
bool LED_ModbusTCP = false;
bool LED_Alarm = false;

// Dip Switches (0/1)

// Dip[0] // A0
// Dip[1] // A1
// Dip[2] // A2
// Dip[3] // A3
// Dip[4] // A4
uint8_t Pmm1003ID = 1;

uint16_t tempRead = 0;
uint16_t OutRelayImage = 0xaaaa;
uint16_t OutInputsImage = 0xaaaa;

// Dip[5] // (Normal operation / FactoryDefualt )  use (Buadrate = 115200, 8N1 ) or read from internal flash
bool FactoryDefualt = false;
// Dip[6] // (RTU / TCP)
bool ModbusTCPMode = false;
// Dip[7] // (Slave IO / IO mirror master) , in master mode use address 31 by default
bool IOMirrorMode = false;
// Dip[8] // (No GSM / Use GSM backup)
bool GSMBackupMode = false;
// Dip[9] // (Normal opration / Setup mode) ,Save settings to falsh on change to 0
bool SetupMode = false;
// Dip[10] // Rs485 Termination resistor

void PCAExtensionSetup(uint8_t index, uint8_t BoardAddress, uint16_t direction)
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
    PCAExtensionSetup(0, 0x20, 0x0000); // LEDs
    PCAExtensionSetup(1, 0x21, 0x0000); // LEDs
    PCAExtensionSetup(2, 0x22, 0x0000); // LEDs + DIP switches
    PCAExtensionSetup(3, 0x23, 0xFFFF); // Inputs
    PCAExtensionSetup(4, 0x24, 0x0000); // Outputs

    // other pins setup
    pinMode(ETHERRST, OUTPUT);
    pinMode(ETHERINT, INPUT);
    pinMode(TESTLEDS, INPUT_PULLUP);
    pinMode(DILOSSPOWER, INPUT);
    pinMode(INT01, INPUT);
    pinMode(INT02, INPUT);
    pinMode(INT03, INPUT);
    pinMode(INT04, INPUT);
    pinMode(INT05, INPUT);

    // STEP02 : Get device address and load variables and check factory Defaults
    uint16_t tmpint = 0;
    tmpint = IOext[2].read();

    Pmm1003ID = highByte(tmpint) & 0x1f;

    LED_Ready = true;
    LED_Alarm = true;

    // tmpint = PMM1003ID ;

    // STEP03 : check and update Modes

    // STEP04 : Run Modbus Server/Client as per mode

    // STEP05 : Save the status of everthing

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

void UpDateLEDs()
{

    if (!digitalRead(TESTLEDS))
    {
        IOext[0].write(0xffff);
        IOext[1].write(0xffff);
        IOext[2].write(0x00ff);
    }
    else
    {
        // Updaye out LEDs
        uint16_t LEDsGroup01 = 0;
        uint16_t LEDsGroup02 = 0;
        uint16_t LEDsGroup03 = 0;

        // Inputs
        bitWrite(LEDsGroup02, 8, bitRead(OutInputsImage, 0)); // 
        bitWrite(LEDsGroup02, 9, bitRead(OutInputsImage, 1));
        bitWrite(LEDsGroup02, 10, bitRead(OutInputsImage, 2));
        bitWrite(LEDsGroup02, 11, bitRead(OutInputsImage, 3));
        bitWrite(LEDsGroup02, 12, bitRead(OutInputsImage, 4));
        bitWrite(LEDsGroup02, 13, bitRead(OutInputsImage, 5));
        bitWrite(LEDsGroup02, 14, bitRead(OutInputsImage, 6));
        bitWrite(LEDsGroup02, 15, bitRead(OutInputsImage, 7));

        bitWrite(LEDsGroup02, 6, bitRead(OutInputsImage, 8));
        bitWrite(LEDsGroup02, 5, bitRead(OutInputsImage, 9));
        bitWrite(LEDsGroup02, 4, bitRead(OutInputsImage, 10));
        bitWrite(LEDsGroup02, 3, bitRead(OutInputsImage, 11));
        bitWrite(LEDsGroup02, 2, bitRead(OutInputsImage, 12));
        bitWrite(LEDsGroup02, 1, bitRead(OutInputsImage, 13));
        bitWrite(LEDsGroup02, 0, bitRead(OutInputsImage, 14));
        bitWrite(LEDsGroup03, 0, bitRead(OutInputsImage, 15));

        //outputs
        bitWrite(LEDsGroup01, 1, bitRead(OutRelayImage, 0)); 
        bitWrite(LEDsGroup01, 0, bitRead(OutRelayImage, 1));
        bitWrite(LEDsGroup01, 15, bitRead(OutRelayImage, 2));
        bitWrite(LEDsGroup01, 14, bitRead(OutRelayImage, 3));
        bitWrite(LEDsGroup01, 13, bitRead(OutRelayImage, 4));
        bitWrite(LEDsGroup01, 12, bitRead(OutRelayImage, 5));
        bitWrite(LEDsGroup01, 11, bitRead(OutRelayImage, 6));
        bitWrite(LEDsGroup01, 10, bitRead(OutRelayImage, 7));

        bitWrite(LEDsGroup01, 9, bitRead(OutRelayImage, 8));
        bitWrite(LEDsGroup01, 8, bitRead(OutRelayImage, 9));
        bitWrite(LEDsGroup03, 6, bitRead(OutRelayImage, 10));
        bitWrite(LEDsGroup03, 5, bitRead(OutRelayImage, 11));
        bitWrite(LEDsGroup03, 4, bitRead(OutRelayImage, 12));
        bitWrite(LEDsGroup03, 3, bitRead(OutRelayImage, 13));
        bitWrite(LEDsGroup03, 2, bitRead(OutRelayImage, 14));
        bitWrite(LEDsGroup03, 1, bitRead(OutRelayImage, 15));

        //Status 
        

        bitWrite(LEDsGroup01, 7, LED_Ready); // Ready 
        bitWrite(LEDsGroup01, 6, LED_COMMHealthy); // COMM Healthy 
        bitWrite(LEDsGroup01, 5, LED_IOMaster); // IO Master
        bitWrite(LEDsGroup01, 4, LED_ModbusRTU); // Modbus RTU
        bitWrite(LEDsGroup01, 3, LED_ModbusTCP); // Modbus TCP
        bitWrite(LEDsGroup01, 2, LED_Alarm); // Alarm

        // Do Output
        IOext[0].write(LEDsGroup01);
        IOext[1].write(LEDsGroup02);
        IOext[2].write(LEDsGroup03);
    }
}

void updateInputBuffers()
{
    // tempRead = IOext[2].read();

    // SerialUSB.println (tempRead);
    // bool TESTLEDSAUTUS = digitalRead(TESTLEDS) ;
    // SerialUSB.println (TESTLEDSAUTUS);

    uint16_t RawRead = 0;
    RawRead = IOext[3].read();
    RawRead = RawRead ^ 0xffff;
    OutInputsImage = RawRead;

    // arrange bits
    bitWrite(OutInputsImage, 8, bitRead(RawRead, 15));
    bitWrite(OutInputsImage, 9, bitRead(RawRead, 14));
    bitWrite(OutInputsImage, 10, bitRead(RawRead, 13));
    bitWrite(OutInputsImage, 11, bitRead(RawRead, 12));
    bitWrite(OutInputsImage, 12, bitRead(RawRead, 11));
    bitWrite(OutInputsImage, 13, bitRead(RawRead, 10));
    bitWrite(OutInputsImage, 14, bitRead(RawRead, 9));
    bitWrite(OutInputsImage, 15, bitRead(RawRead, 8));

    // SerialUSB.println(OutInputsImage);
    UpDateLEDs();

    // for (int i = 0; i < NUM_DISCRETE_INPUT; i++)
    // {
    // 	uint8_t pin = pinMask_DIN[i];
    //     if (bool_input[i/8][i%8] != NULL)
    //         *bool_input[i/8][i%8] = digitalRead(pin);
    // }

    // for (int i = 0; i < NUM_ANALOG_INPUT; i++)
    // {
    // 	uint8_t pin = pinMask_AIN[i];
    //     if (int_input[i] != NULL)
    //         *int_input[i] = (analogRead(pin) * 64);
    // }
}

void updateOutputBuffers()
{

    // update relays
    OutRelayImage = OutInputsImage ; // for test only 

    IOext[4].write(OutRelayImage);
    UpDateLEDs();

    // for (int i = 0; i < NUM_DISCRETE_OUTPUT; i++)
    // {
    // 	uint8_t pin = pinMask_DOUT[i];
    //     if (bool_output[i/8][i%8] != NULL)
    //         digitalWrite(pin, *bool_output[i/8][i%8]);
    // }
    // for (int i = 0; i < NUM_ANALOG_OUTPUT; i++)
    // {
    // 	uint8_t pin = pinMask_AOUT[i];
    //     if (int_output[i] != NULL)
    //         analogWrite(pin, (*int_output[i] / 256));
    // }
}

#endif
