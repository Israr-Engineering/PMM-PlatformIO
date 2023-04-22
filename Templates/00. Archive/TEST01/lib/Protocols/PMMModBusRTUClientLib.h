#include "../Commons/ProjectDef.h"
#ifdef PMMModbusRTUClient

extern void PMMModBUSRTUClientSetup(uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPortNumber = 1);

extern int PMMModBUSRTUClientcoilRead(int SlaveID, int address);
extern int PMMModBUSRTUClientdiscreteInputRead(int SlaveID, int address);
extern long PMMModBUSRTUClientholdingRegisterRead(int SlaveID, int address);
extern long PMMModBUSRTUClientinputRegisterRead(int SlaveID, int address);
extern void PMMModBUSRTUClientcoilWrite(int SlaveID, int address, uint8_t value);
extern void PMMModBUSRTUClientdiscreteInputWrite(int SlaveID, int address, uint8_t value);
extern void PMMModBUSRTUClientholdingRegisterWrite(int SlaveID, int address, uint16_t value);
extern void PMMModBUSRTUClientinputRegisterWrite(int SlaveID, int address, uint16_t value);

void PMMModBUSRTUClientSetup(uint16_t Config, int16_t BaudRate, int16_t TXPin,
                             int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPort)
{
    if (SerialPort == 1)
        RS485.setSerial(&Serial);
    else if (SerialPort == 2)
        RS485.setSerial(&Serial1);

    RS485.setPins(TXPin, SerialSelectionPin, RXPin);
    ModbusRTUClient.begin(BaudRate, Config)
}

int PMMModBUSRTUClientcoilRead(int SlaveID, int address)
{
    int value;

    value = ModbusRTUClient.coilRead(SlaveID, address);
    return value;
}

int PMMModBUSRTUClientdiscreteInputRead(int SlaveID, int address)
{
    int value;

    value = ModbusRTUClient.discreteInputRead(SlaveID, address);
    return value;
}

long PMMModBUSRTUClientholdingRegisterRead(int SlaveID, int address)
{
    long value;

    value = ModbusRTUClient.holdingRegisterRead(SlaveID, address);
    return value;
}

long PMMModBUSRTUClientinputRegisterRead(int SlaveID, int address)
{
    long value;

    value = ModbusRTUClient.inputRegisterRead(SlaveID, address);
    return value;
}

void PMMModBUSRTUClientcoilWrite(int SlaveID, int address, uint8_t value)
{

    ModbusRTUClient.coilWrite(SlaveID, address, value);
}

// void PMMModBUSRTUClientdiscreteInputWrite(int SlaveID, int address, uint8_t value)
// {

//     ModbusRTUClient.discreteInputWrite(SlaveID, address, value);
// }

void PMMModBUSRTUClientholdingRegisterWrite(int SlaveID, int address, uint16_t value)
{

    ModbusRTUClient.holdingRegisterWrite(SlaveID, address, value);
}

// void PMMModBUSRTUClientinputRegisterWrite(int SlaveID, int address, uint16_t value)
// {

//     ModbusRTUClient.inputRegisterWrite(SlaveID, address, value);
// }
#endif