
#include <PmmModbus.h>


  PmmEthernetServer ethServer(502);
  PmmModbusTCPServer modbusTCPServer;


PmmModBus::PmmModBus() {
}

PmmModBus::~PmmModBus() {
}

//RTUServer

void PmmModBus::RtuServerStart()
{
    
    
    
//   PMMModBUSRTUServerSetup(1, SERIAL_8N1, 9600, 30, 9, 31, 1);
//   PMMModBUSRTUServerconfigure(
//       false, 0, 1024,
//       false, 0, 1024,
//       true, 0, 128,
//       false, 0, 128);

}


void PmmModBus::PMMModBUSRTUServerSetup(int16_t SlaveID, uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPort)
{
    if (SerialPort == 1)
        RS485.setSerial(&Serial);
    else if (SerialPort == 2)
        RS485.setSerial(&Serial1);

    RS485.setPins(TXPin, SerialSelectionPin, RXPin);
    PmmModbusRTUServer.begin(SlaveID, BaudRate, Config);
     
    
}

void PmmModBus::PMMModBUSRTUServerconfigure(
    bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
    bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
    bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
    bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity)
{

    RCoils[1] = 0 ;

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


int PmmModBus:: PMMModBUSRTUServercoilReadOneValue(int Address)
{

    int value;
    PmmModbusRTUServer.poll();
    value = PmmModbusRTUServer.coilRead(Address);

    return value;
}

int PmmModBus:: PMMModBUSRTUServerdiscreteInputReadOneValue(int address)
{
    int value;
    PmmModbusRTUServer.poll();
    value = PmmModbusRTUServer.discreteInputRead(address);
    return value;
}

long PmmModBus:: PMMModBUSRTUServerholdingRegisterReadOneValue(int address)
{
    long value;
    PmmModbusRTUServer.poll();
    value = PmmModbusRTUServer.holdingRegisterRead(address);
    return value;
}

long PmmModBus:: PMMModBUSRTUServerinputRegisterReadOneValue(int address)
{
    long value;
    PmmModbusRTUServer.poll();
    value = PmmModbusRTUServer.inputRegisterRead(address);
    return value;
}
void PmmModBus:: PMMModBUSRTUServercoilWrite(int address, uint8_t value)
{
    PmmModbusRTUServer.poll();
    PmmModbusRTUServer.coilWrite(address, value);
}
void PmmModBus:: PMMModBUSRTUServerdiscreteInputWrite(int address, uint8_t value)
{
    PmmModbusRTUServer.poll();
    PmmModbusRTUServer.discreteInputWrite(address, value);
}
void PmmModBus:: PMMModBUSRTUServerholdingRegisterWrite(int address, uint16_t value)
{
    PmmModbusRTUServer.poll();
    PmmModbusRTUServer.holdingRegisterWrite(address, value);
}
void PmmModBus:: PMMModBUSRTUServerinputRegisterWrite(int address, uint16_t value)
{
    PmmModbusRTUServer.poll();
    PmmModbusRTUServer.inputRegisterWrite(address, value);
}

// RTU Client


void PmmModBus:: PMMModBUSRTUClientSetup(uint16_t Config, int16_t BaudRate, int16_t TXPin,
                             int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPort)
{
    if (SerialPort == 1)
        RS485.setSerial(&Serial);
    else if (SerialPort == 2)
        RS485.setSerial(&Serial1);

    RS485.setPins(TXPin, SerialSelectionPin, RXPin);
    PmmModbusRTUClient.begin(BaudRate, Config);
}

int PmmModBus:: PMMModBUSRTUClientcoilRead(int SlaveID, int address)
{
    int value;

    value = PmmModbusRTUClient.coilRead(SlaveID, address);
    return value;
}

int PmmModBus:: PMMModBUSRTUClientdiscreteInputRead(int SlaveID, int address)
{
    int value;

    value = PmmModbusRTUClient.discreteInputRead(SlaveID, address);
    return value;
}

long PmmModBus:: PMMModBUSRTUClientholdingRegisterRead(int SlaveID, int address)
{
    long value;

    value = PmmModbusRTUClient.holdingRegisterRead(SlaveID, address);
    return value;
}

long PmmModBus:: PMMModBUSRTUClientinputRegisterRead(int SlaveID, int address)
{
    long value;

    value = PmmModbusRTUClient.inputRegisterRead(SlaveID, address);
    return value;
}

void PmmModBus:: PMMModBUSRTUClientcoilWrite(int SlaveID, int address, uint8_t value)
{

    PmmModbusRTUClient.coilWrite(SlaveID, address, value);
}

// void PMMModBUSRTUClientdiscreteInputWrite(int SlaveID, int address, uint8_t value)
// {

//     ModbusRTUClient.discreteInputWrite(SlaveID, address, value);
// }

void PmmModBus:: PMMModBUSRTUClientholdingRegisterWrite(int SlaveID, int address, uint16_t value)
{

    PmmModbusRTUClient.holdingRegisterWrite(SlaveID, address, value);
}


// TCP Server



void PmmModBus:: PMMmodbusTCPServerSetup(uint8_t *MACAddress, IPAddress IpAddress, int16_t ETHPORT, int16_t SlaveID)
{
    Ethernet.begin(MACAddress, IpAddress);
    ethServer.begin();
    modbusTCPServer.begin(SlaveID); 
}

void PmmModBus:: PMMmodbusTCPServerconfigure(bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
                                 bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
                                 bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
                                 bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity)
{

    if (Coils)
    {
        modbusTCPServer.configureCoils(CoilsStartAddress, CoilsQauntity);
    }
    if (InputRegisters)
    {
        modbusTCPServer.configureInputRegisters(InputRegistersStartAddress, InputRegistersQauntity);
    }
    if (HoldingRegisters)
    {
        modbusTCPServer.configureHoldingRegisters(HoldingRegistersStartAddress, HoldingRegistersQauntity);
    }
    if (DiscreteInputs)
    {
        modbusTCPServer.configureDiscreteInputs(DiscreteInputsStartAddress, DiscreteInputsQauntity);
    }
    
}

int PmmModBus:: PMMmodbusTCPServercoilRead(int address)
{
    int value;
    PmmEthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    value = modbusTCPServer.coilRead(address);
       
    return value;
}

int PmmModBus:: PMMmodbusTCPServerdiscreteInputRead(int address)
{
    int value;
    PmmEthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    value = modbusTCPServer.discreteInputRead(address);
        
    return value;
}

long PmmModBus:: PMMmodbusTCPServerholdingRegisterRead(int address)
{
    long value;
    PmmEthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    value =modbusTCPServer.holdingRegisterRead(address);
       
    return value;
}

long PmmModBus:: PMMmodbusTCPServerinputRegisterRead(int address)
{
    long value;
    PmmEthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    value = modbusTCPServer.inputRegisterRead(address);
        
    return value;
}

void PmmModBus:: PMMmodbusTCPServercoilWrite(int address, uint8_t value)
{
    PmmEthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    modbusTCPServer.coilWrite(address, value);
        
}

void PmmModBus:: PMMmodbusTCPServerdiscreteInputWrite(int address, uint8_t value)
{
    PmmEthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    modbusTCPServer.discreteInputWrite(address, value);
        
}

void PmmModBus:: PMMmodbusTCPServerholdingRegisterWrite(int address, uint16_t value)
{
    
    PmmEthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    modbusTCPServer.holdingRegisterWrite(address, value);
        
}

void PmmModBus:: PMMmodbusTCPServerinputRegisterWrite(int address, uint16_t value)
{
    PmmEthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    modbusTCPServer.inputRegisterWrite(address, value);
        
}