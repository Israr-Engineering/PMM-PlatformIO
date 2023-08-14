// #include <stdlib.h>
//  extern "C" {
//  #include "openplc.h"
//  }

#ifndef PMM1003_H
#define PMM1003_H

#include "Arduino.h"
#include <PmmTypes.h>
#include <PmmPCA95x5.h>
#include <PmmEthernet.h>

// #include <PmmGlobalFunctions.h>

PCA9555 IOext[6] =
    {
        PCA9555(),
        PCA9555(),
        PCA9555(),
        PCA9555(),
        PCA9555(),
        PCA9555()};


// Define the number of inputs and outputs for this board (mapping for the 1003)
// #define NUM_DISCRETE_INPUT 27
// #define NUM_ANALOG_INPUT 1
// #define NUM_DISCRETE_OUTPUT 56
// #define NUM_ANALOG_OUTPUT 4

// // Create the I/O pin masks
// uint8_t pinMask_DIN[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 24, 43, 44, 19, 23, 7};
// uint8_t pinMask_AIN[] = {A0, A1, A2, A3, A4, A5};
// uint8_t pinMask_DOUT[] = {7, 8, 12, 13};
// uint8_t pinMask_AOUT[] = {9, 10, 11};

// Common Pins If any (Pin Mask)
static const uint8_t SSWIZ = 9ul;
static const uint8_t ETHERRST = 32ul;
static const uint8_t ETHERINT = 18ul;
static const uint8_t TESTLEDS = 4ul;
static const uint8_t DILOSSPOWER = 19ul;
static const uint8_t INT01 = 24ul;
static const uint8_t INT02 = 25ul;
static const uint8_t INT03 = 13ul;
static const uint8_t INT04 = 2ul;
static const uint8_t INT05 = 3ul;

// Etehrnet
// PmmEthernetServer server1003(502);
PmmModBus ModbusTCPServer;
PmmModBus ModbusRTUServer;
byte mac[] = {0x23, 0x22, 0x22, 0x22, 0x48, 0x7E};
byte IPAdd[] = {192, 168, 1, 100};

// Status LEDs
bool LED_Ready = false;
bool LED_COMMHealthy = false;
bool LED_IOMaster = false;
bool LED_ModbusRTU = false;
bool LED_ModbusTCP = false;
bool LED_Alarm = false;
bool AlarmFlag = true ;

// Dip Switches (0/1)

// Dip[0] // A0
// Dip[1] // A1
// Dip[2] // A2
// Dip[3] // A3
// Dip[4] // A4
uint8_t Pmm1003ID = 1;

uint16_t tempRead = 0;
uint16_t OutRelayImage = 0x0;
uint16_t OutInputsImage = 0x0;

// Dip[5] // (Slave IO / IO mirror master) , in master mode use address 31 by default
bool MasterIOMode = false;
// Dip[6] // (RTU / TCP)
bool ModbusTCPMode = false;
// Dip[7] // (Normal operation / FactoryDefualt )  use (Buadrate = 115200, 8N1 ) or read from internal flash
bool FactoryDefualtMode = false;

void PCAExtensionSetup(uint8_t index, uint8_t BoardAddress, uint16_t direction)
{

    Wire.begin();
    IOext[index].attach(Wire, BoardAddress);
    IOext[index].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
    IOext[index].direction(direction);
    IOext[index].write(PCA95x5::Level::L_ALL);
}

void ResetEtherNetController()
{
    // Reset Ethernet controller
    pinMode(ETHERRST, OUTPUT);
    digitalWrite(ETHERRST, LOW);
    delay(10);
    digitalWrite(ETHERRST, HIGH);
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
    pinMode(SSWIZ, OUTPUT);
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
    // STEP03 : check and update Modes
    Pmm1003ID = highByte(tmpint) & 0x1f;
    MasterIOMode = bitRead(highByte(tmpint), 5);
    ModbusTCPMode = bitRead(highByte(tmpint), 6);      // Dip[6] // (RTU / TCP)
    FactoryDefualtMode = bitRead(highByte(tmpint), 7); // (Normal operation / FactoryDefualt )
                                                       // use (Buadrate = 115200, 8N1 ) or read from internal flash
    // STEP04 : Setup Ethernet Controller
    ResetEtherNetController();
    uint8_t tmpID = 0;
    if (FactoryDefualtMode)
    {
        tmpID = Pmm1003ID;
        if (MasterIOMode)
            tmpID++;
    }
    else
    {
        // Read from EEprom
    }
    // Now Start Ethernet controller
    mac[0] = tmpID;
    IPAddress ip(IPAdd[0], IPAdd[1], IPAdd[2], tmpID);
    Ethernet.init(SSWIZ); // for WIZ5100 sspin
    Ethernet.begin(mac, ip);
    // server.begin(); // port 80

    // STEP05 : Run Modbus Server/Client as per mode
    if (MasterIOMode) // IO Mirror mode => I am the master
    {
        if (ModbusTCPMode) // TCP
        {
            IPAddress ipServer(IPAdd[0], IPAdd[1], IPAdd[2], Pmm1003ID);

            ModbusTCPServer.PmmModBusTCPClientSetup(IPAddress(192, 168, 1, 42), 502);
        }
        else // RTU
        {
            ModbusRTUServer.PmmModBusRTUClientSetup(SERIAL_8N1, 115200, 35, 36, 31, 1);
        }
    }
    else // Just Run Modbus Servers
    {
        if (ModbusTCPMode) // TCP
        {
            ModbusTCPServer.PmmModBusTCPServerSetup(502, Pmm1003ID);
            ModbusTCPServer.PmmModBusTCPServerConfigure(true, 0, 24, true, 0, 6, true, 0, 6, true, 0, 24);
        }
        else // RTU
        {
            ModbusRTUServer.PmmModBusRTUServerSetup(Pmm1003ID, SERIAL_8N1, 115200, 35, 36, 31, 1);
            ModbusTCPServer.PmmModBusRTUServerconfigure(true, 0, 24, true, 0, 6, true, 0, 6, true, 0, 24);
        }
    }

    // STEP06 : Save the status of everthing

    // STEP06 : Block Output and Update LEDs
    LED_Ready = true;
    LED_Alarm = AlarmFlag;
}

void UpDateLEDs()
{

    if (!digitalRead(TESTLEDS))
    {
        IOext[0].write(0xffff);
        IOext[1].write(0xffff);
        IOext[2].write(0x007f);
    }
    else
    {
        // Update out LEDs
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

        // outputs
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

        // Status

        bitWrite(LEDsGroup01, 7, LED_Ready);       // Ready
        bitWrite(LEDsGroup01, 6, LED_COMMHealthy); // COMM Healthy = (Fiber)
        bitWrite(LEDsGroup01, 5, LED_IOMaster);    // IO Master    = (Ext Ethernet)
        bitWrite(LEDsGroup01, 4, LED_ModbusRTU);   // Modbus RTU   = (RS485 Rx)
        bitWrite(LEDsGroup01, 3, LED_ModbusTCP);   // Modbus TCP   = (RS485 Tx)
        bitWrite(LEDsGroup01, 2, LED_Alarm);       // Alarm

        bitWrite(LEDsGroup03, 7, AlarmFlag); // ALarm relay

        // Do Output
        IOext[0].write(LEDsGroup01);
        IOext[1].write(LEDsGroup02);
        IOext[2].write(LEDsGroup03);
    }
}

void updateInputBuffers()
{

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

    // update modbus buffer
    if (MasterIOMode) // Master Mode
    {
        for (int i = 0; i < 16; i++)
        {

            bool_output[i / 8][i % 8] = bitRead(OutInputsImage, i);
        }
    }
    else
    {
        for (int i = 0; i < 16; i++)
        {

            bool_input[i / 8][i % 8] = bitRead(OutInputsImage, i);
        }
    }

    int_input[0] = OutInputsImage;

    UpDateLEDs();
}

void updateOutputBuffers()
{

    OutRelayImage = 0;
    // update relays
    if (MasterIOMode) // Master Mode
    {
        for (int i = 0; i < 16; i++)
        {

            bitWrite(OutRelayImage, i, bool_input[i / 8][i % 8]);
        }
    }
    else
    {

        for (int i = 0; i < 16; i++)
        {

            bitWrite(OutRelayImage, i, bool_output[i / 8][i % 8]);
        }
    }

    // update modbus buffer
    int_output[0] = OutRelayImage;

    // Do real output
    if(!AlarmFlag) IOext[4].write(OutRelayImage);
    UpDateLEDs();
}

void GenerateAlarm()
{
    // Set AlarmFlag if ....

    AlarmFlag = false ;
    LED_Alarm = AlarmFlag ;

}

void syncModbusBuffers()
{
   GenerateAlarm();

    IPAddress SlaveIP(IPAdd[0], IPAdd[1], IPAdd[2], Pmm1003ID);
    //IPAddress SlaveIP(192,168,1,7); // for test only
    uint16_t port = 502 ;
    if (MasterIOMode) // Master Mode
    {
        // Sync Buffers with Modbus Buffers
        // bool_output[0][0] = 1 ;
        for (int i = 0; i < MAX_DIGITAL_OUTPUT; i++)
        {
            ModbusTCPServer.PmmModBusTCPClientCoilWrite(SlaveIP, port,Pmm1003ID, i, bool_output[i / 8][i % 8]);
        }
        for (int i = 0; i < MAX_ANALOG_OUTPUT; i++)
        {

            ModbusTCPServer.PmmModBusTCPClientHoldingWrite(SlaveIP, port,Pmm1003ID, i, int_output[i]);
        }
        
        // Read changes from clients

        for (int i = 0; i < MAX_DIGITAL_INPUT; i++)
        {
            bool_input[i / 8][i % 8] = ModbusTCPServer.PmmModBusTCPClientDiscreteRead(SlaveIP, port,Pmm1003ID, i);
        }

        for (int i = 0; i < MAX_ANALOG_INPUT; i++)
        {

            int_input[i] = ModbusTCPServer.PmmModBusTCPClientInputRead(SlaveIP, port,Pmm1003ID, i);
        }
    }
    else // Slave Mode
    {

        // Sync OpenPLC Buffers with Modbus Buffers
        for (int i = 0; i < MAX_DIGITAL_OUTPUT; i++)
        {
            // ModbusTCPServer.PmmModBusTCPServerCoilWrite(i, bool_output[i / 8][i % 8]);
        }
        for (int i = 0; i < MAX_ANALOG_OUTPUT; i++)
        {

            ModbusTCPServer.PmmModBusTCPServerHoldingWrite(i, int_output[i]);
        }
        for (int i = 0; i < MAX_DIGITAL_INPUT; i++)
        {

            ModbusTCPServer.PmmModBusTCPServerDiscreteWrite(i, bool_input[i / 8][i % 8]);
        }
        for (int i = 0; i < MAX_ANALOG_INPUT; i++)
        {

            ModbusTCPServer.PmmModBusTCPServerInputWrite(i, int_input[i]);
        }

        // Read changes from clients

        for (int i = 0; i < MAX_DIGITAL_OUTPUT; i++)
        {
            bool_output[i / 8][i % 8] = ModbusTCPServer.PmmModBusTCPServerCoilRead(i);
            
        }

        for (int i = 0; i < MAX_ANALOG_OUTPUT; i++)
        {

            int_output[i] = ModbusTCPServer.PmmModBusTCPServerHoldingRead(i);
        }
    }
}

#endif
