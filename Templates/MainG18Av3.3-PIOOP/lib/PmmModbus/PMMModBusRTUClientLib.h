//#include "../Commons/ProjectDef.h"
#ifdef PMMModbusRTUClient

extern void PmmModBusRTUClientSetup(uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPortNumber = 1);

extern int PmmModBusRTUClientCoilRead(int SlaveID, int address);
extern int PmmModBusRTUClientDiscreteRead(int SlaveID, int address);
extern long PmmModBusRTUClientHoldingRead(int SlaveID, int address);
extern long PmmModBusRTUClientInputRead(int SlaveID, int address);
extern void PmmModBusRTUClientCoilWrite(int SlaveID, int address, uint8_t value);
extern void PmmModBusRTUClientDiscreteWrite(int SlaveID, int address, uint8_t value);
extern void PmmModBusRTUClientHoldingWrite(int SlaveID, int address, uint16_t value);
extern void PmmModBusRTUClientInputWrite(int SlaveID, int address, uint16_t value);

void PmmModBusRTUClientSetup(uint16_t Config, int16_t BaudRate, int16_t TXPin,
                             int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPort)
{
    if (SerialPort == 1)
        RS485.setSerial(&Serial);
    else if (SerialPort == 2)
        RS485.setSerial(&Serial1);

    RS485.setPins(TXPin, SerialSelectionPin, RXPin);
    PmmModbusRTUClient.begin(BaudRate, Config);
}

int PmmModBusRTUClientCoilRead(int SlaveID, int address)
{
    int value;

    value = PmmModbusRTUClient.coilRead(SlaveID, address);
    return value;
}

int PmmModBusRTUClientDiscreteRead(int SlaveID, int address)
{
    int value;

    value = PmmModbusRTUClient.discreteInputRead(SlaveID, address);
    return value;
}

long PmmModBusRTUClientHoldingRead(int SlaveID, int address)
{
    long value;

    value = PmmModbusRTUClient.holdingRegisterRead(SlaveID, address);
    return value;
}

long PmmModBusRTUClientInputRead(int SlaveID, int address)
{
    long value;

    value = PmmModbusRTUClient.inputRegisterRead(SlaveID, address);
    return value;
}

void PmmModBusRTUClientCoilWrite(int SlaveID, int address, uint8_t value)
{

    PmmModbusRTUClient.coilWrite(SlaveID, address, value);
}

// void PmmModBusRTUClientDiscreteWrite(int SlaveID, int address, uint8_t value)
// {

//     ModbusRTUClient.discreteInputWrite(SlaveID, address, value);
// }

void PmmModBusRTUClientHoldingWrite(int SlaveID, int address, uint16_t value)
{

    PmmModbusRTUClient.holdingRegisterWrite(SlaveID, address, value);
}

// void PmmModBusRTUClientInputWrite(int SlaveID, int address, uint16_t value)
// {

//     ModbusRTUClient.inputRegisterWrite(SlaveID, address, value);
// }
#endif