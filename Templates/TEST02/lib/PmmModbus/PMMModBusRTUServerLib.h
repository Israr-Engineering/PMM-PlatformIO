// #include "../Commons/ProjectDef.h"
#ifdef PMMModbusRTUServer

extern void PMMModBUSRTUServerSetup(int16_t SlaveID, uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPortNumber = 1);

extern void PMMModBUSRTUServerconfigure(
    bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
    bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
    bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
    bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity);

extern int *PMMModBUSRTUServercoilRead(int StartingAddress, int quantity);
extern int PMMModBUSRTUServercoilReadOneValue(int Address);

extern int *PMMModBUSRTUServerdiscreteInputRead(int StartingAddress, int quantity);
extern int PMMModBUSRTUServerdiscreteInputReadOneValue(int address);

extern long *PMMModBUSRTUServerholdingRegisterRead(int StartingAddress, int quantity);
extern long PMMModBUSRTUServerholdingRegisterReadOneValue(int address);

extern long *PMMModBUSRTUServerinputRegisterRead(int StartingAddress, int quantity);
extern long PMMModBUSRTUServerinputRegisterReadOneValue(int address);

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
    PmmModbusRTUServer.begin(SlaveID, BaudRate, Config);
}

void PMMModBUSRTUServerconfigure(
    bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
    bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
    bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
    bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity)
{

    if (Coils)
    {
        PmmModbusRTUServer.configureCoils(CoilsStartAddress, CoilsQauntity);
    }
    if (InputRegisters)
    {
        PmmModbusRTUServer.configureInputRegisters(InputRegistersStartAddress, InputRegistersQauntity);
    }
    if (HoldingRegisters)
    {
        PmmModbusRTUServer.configureHoldingRegisters(HoldingRegistersStartAddress, HoldingRegistersQauntity);
    }
    if (DiscreteInputs)
    {
        PmmModbusRTUServer.configureDiscreteInputs(DiscreteInputsStartAddress, DiscreteInputsQauntity);
    }
}

int *PMMModBUSRTUServercoilRead(int StartingAddress, int quantity)
{
    int ReturnArray[quantity];
    for (int i = StartingAddress; i < (StartingAddress + quantity); i++)
    {
        int index = i - StartingAddress;
        int value;
        PmmModbusRTUServer.poll();
        value = PmmModbusRTUServer.coilRead(i);
        ReturnArray[index] = value;
    }

    return ReturnArray;
}
int PMMModBUSRTUServercoilReadOneValue(int Address)
{

    int value;
    PmmModbusRTUServer.poll();
    value = PmmModbusRTUServer.coilRead(Address);

    return value;
}

int *PMMModBUSRTUServerdiscreteInputRead(int StartingAddress, int quantity)
{
    int ReturnArray[quantity];
    for (int i = StartingAddress; i < (StartingAddress + quantity); i++)
    {
        int index = i - StartingAddress;
        int value;
        PmmModbusRTUServer.poll();
        value = PmmModbusRTUServer.discreteInputRead(i);
        ReturnArray[index] = value;
    }

    return ReturnArray;
}
int PMMModBUSRTUServerdiscreteInputReadOneValue(int address)
{
    int value;
    PmmModbusRTUServer.poll();
    value = PmmModbusRTUServer.discreteInputRead(address);
    return value;
}

long *PMMModBUSRTUServerholdingRegisterRead(int StartingAddress, int quantity)
{
    long ReturnArray[quantity];
    for (int i = StartingAddress; i < (StartingAddress + quantity); i++)
    {
        int index = i - StartingAddress;
        long value;
        PmmModbusRTUServer.poll();
        value = PmmModbusRTUServer.holdingRegisterRead(i);
        ReturnArray[index] = value;
    }

    return ReturnArray;
}
long PMMModBUSRTUServerholdingRegisterReadOneValue(int address)
{
    long value;
    PmmModbusRTUServer.poll();
    value = PmmModbusRTUServer.holdingRegisterRead(address);
    return value;
}

long *PMMModBUSRTUServerinputRegisterRead(int StartingAddress, int quantity)
{
    long ReturnArray[quantity];
    for (int i = StartingAddress; i < (StartingAddress + quantity); i++)
    {
        int index = i - StartingAddress;
        long value;
        PmmModbusRTUServer.poll();
        value = PmmModbusRTUServer.inputRegisterRead(i);
        ReturnArray[index] = value;
    }

    return ReturnArray;
}
long PMMModBUSRTUServerinputRegisterReadOneValue(int address)
{
    long value;
    PmmModbusRTUServer.poll();
    value = PmmModbusRTUServer.inputRegisterRead(address);
    return value;
}

void PMMModBUSRTUServercoilWrite(int address, uint8_t value)
{
    PmmModbusRTUServer.poll();
    PmmModbusRTUServer.coilWrite(address, value);
}

void PMMModBUSRTUServerdiscreteInputWrite(int address, uint8_t value)
{
    PmmModbusRTUServer.poll();
    PmmModbusRTUServer.discreteInputWrite(address, value);
}

void PMMModBUSRTUServerholdingRegisterWrite(int address, uint16_t value)
{
    PmmModbusRTUServer.poll();
    PmmModbusRTUServer.holdingRegisterWrite(address, value);
}

void PMMModBUSRTUServerinputRegisterWrite(int address, uint16_t value)
{
    PmmModbusRTUServer.poll();
    PmmModbusRTUServer.inputRegisterWrite(address, value);
}

#endif