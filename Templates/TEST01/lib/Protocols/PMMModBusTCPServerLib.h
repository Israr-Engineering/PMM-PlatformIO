#include "../Commons/ProjectDef.h"
#ifdef PMMModbusTCPServer
EthernetServer ethServer(502);
ModbusTCPServer modbusTCPServer;

extern void PMMmodbusTCPServerSetup(uint8_t *MACAddress, IPAddress IpAddress, int16_t ETHPORT, int16_t SlaveID);

extern void PMMmodbusTCPServerconfigure(bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
                                        bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
                                        bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
                                        bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity);

extern int PMMmodbusTCPServercoilRead(int address);
extern int PMMmodbusTCPServerdiscreteInputRead(int address);
extern long PMMmodbusTCPServerholdingRegisterRead(int address);
extern long PMMmodbusTCPServerinputRegisterRead(int address);
extern void PMMmodbusTCPServercoilWrite(int address, uint8_t value);
extern void PMMmodbusTCPServerdiscreteInputWrite(int address, uint8_t value);
extern void PMMmodbusTCPServerholdingRegisterWrite(int address, uint16_t value);
extern void PMMmodbusTCPServerinputRegisterWrite(int address, uint16_t value);

void PMMmodbusTCPServerSetup(uint8_t *MACAddress, IPAddress IpAddress, int16_t ETHPORT, int16_t SlaveID)
{
    Ethernet.begin(MACAddress, IpAddress);
    ethServer.begin();
    modbusTCPServer.begin(SlaveID); 
}

void PMMmodbusTCPServerconfigure(bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
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

int PMMmodbusTCPServercoilRead(int address)
{
    int value;
    EthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    value = modbusTCPServer.coilRead(address);
       
    return value;
}

int PMMmodbusTCPServerdiscreteInputRead(int address)
{
    int value;
    EthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    value = modbusTCPServer.discreteInputRead(address);
        
    return value;
}

long PMMmodbusTCPServerholdingRegisterRead(int address)
{
    long value;
    EthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    value =modbusTCPServer.holdingRegisterRead(address);
       
    return value;
}

long PMMmodbusTCPServerinputRegisterRead(int address)
{
    long value;
    EthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    value = modbusTCPServer.inputRegisterRead(address);
        
    return value;
}

void PMMmodbusTCPServercoilWrite(int address, uint8_t value)
{
    EthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    modbusTCPServer.coilWrite(address, value);
        
}

void PMMmodbusTCPServerdiscreteInputWrite(int address, uint8_t value)
{
    EthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    modbusTCPServer.discreteInputWrite(address, value);
        
}

void PMMmodbusTCPServerholdingRegisterWrite(int address, uint16_t value)
{
    
    EthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    modbusTCPServer.holdingRegisterWrite(address, value);
        
}

void PMMmodbusTCPServerinputRegisterWrite(int address, uint16_t value)
{
    EthernetClient client = ethServer.available();
    modbusTCPServer.accept(client);
    modbusTCPServer.poll();
    modbusTCPServer.inputRegisterWrite(address, value);
        
}
#endif