#include "../Commons/ProjectDef.h"
#ifdef PMMModbusRTUServer

extern void PMMModBUSRTUServerSetup(int16_t SlaveID, uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPortNumber = 1);

extern void PMMModBUSRTUServerconfigure(
    bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
    bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
    bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
    bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity
);

extern int PMMModBUSRTUServercoilRead(int address);
extern int PMMModBUSRTUServerdiscreteInputRead(int address);
extern long PMMModBUSRTUServerholdingRegisterRead(int address);
extern long PMMModBUSRTUServerinputRegisterRead(int address);
extern void PMMModBUSRTUServercoilWrite(int address, uint8_t value);
extern void PMMModBUSRTUServerdiscreteInputWrite(int address, uint8_t value);
extern void PMMModBUSRTUServerholdingRegisterWrite(int address, uint16_t value);
extern void PMMModBUSRTUServerinputRegisterWrite(int address, uint16_t value);

void PMMModBUSRTUServerSetup(int16_t SlaveID, uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPort)
{
    if (SerialPort == 1)
        RS485.setSerial(&Serial);
    else if (SerialPort == 2)
        RS485.setSerial(&Serial1);

    RS485.setPins(TXPin, SerialSelectionPin, RXPin);
    ModbusRTUServer.begin(SlaveID, BaudRate, Config);
}

void PMMModBUSRTUServerconfigure(
    bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
    bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
    bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
    bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity)
{

    if (Coils)
    {
        ModbusRTUServer.configureCoils(CoilsStartAddress, CoilsQauntity);
    }
    if (InputRegisters)
    {
        ModbusRTUServer.configureInputRegisters(InputRegistersStartAddress, InputRegistersQauntity);
    }
    if (HoldingRegisters)
    {
        ModbusRTUServer.configureHoldingRegisters(HoldingRegistersStartAddress, HoldingRegistersQauntity);
    }
    if (DiscreteInputs)
    {
        ModbusRTUServer.configureDiscreteInputs(DiscreteInputsStartAddress, DiscreteInputsQauntity);
    }
}

int PMMModBUSRTUServercoilRead(int address)
{
    int value;
    ModbusRTUServer.poll();
    value = ModbusRTUServer.coilRead(address);
    return value;
}

int PMMModBUSRTUServerdiscreteInputRead(int address)
{
    int value;
    ModbusRTUServer.poll();
    value = ModbusRTUServer.discreteInputRead(address);
    return value;
}

long PMMModBUSRTUServerholdingRegisterRead(int address)
{
    long value;
    ModbusRTUServer.poll();
    value = ModbusRTUServer.holdingRegisterRead(address);
    return value;
}

long PMMModBUSRTUServerinputRegisterRead(int address)
{
    long value;
    ModbusRTUServer.poll();
    value = ModbusRTUServer.inputRegisterRead(address);
    return value;
}

void PMMModBUSRTUServercoilWrite(int address, uint8_t value)
{
    ModbusRTUServer.poll();
    ModbusRTUServer.coilWrite(address, value);
}

void PMMModBUSRTUServerdiscreteInputWrite(int address, uint8_t value)
{
    ModbusRTUServer.poll();
    ModbusRTUServer.discreteInputWrite(address, value);
}

void PMMModBUSRTUServerholdingRegisterWrite(int address, uint16_t value)
{
    ModbusRTUServer.poll();
    ModbusRTUServer.holdingRegisterWrite(address, value);
}

void PMMModBUSRTUServerinputRegisterWrite(int address, uint16_t value)
{
    ModbusRTUServer.poll();
    ModbusRTUServer.inputRegisterWrite(address, value);
}
#endif