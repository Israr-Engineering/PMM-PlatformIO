#include "../Commons/ProjectDef.h"
#ifdef PMMModbusLightRTUServer
ModbusSerial mb;

void PMMModbusLightRTUServerSetup(int8_t SerialPortSelection, int16_t BudeRate, int16_t ModBusConfigration, int8_t SlaveID, int8_t SerialSelectionPin,
                                  bool HoldingRegEnable, int16_t StartaAddressHoldingReg, int16_t QuantityHoldingReg,
                                  bool CoilsEnable, int16_t StartaAddressCoils, int16_t QuantityCoils);

void PMMModbusLightRTUServerWriteHoldingReg(int16_t Address, int16_t Value);
int16_t PMMModbusLightRTUServerReadHoldingReg(int16_t Address);

void PMMModbusLightRTUServerWriteCoils(int16_t Address, bool Value);
int16_t PMMModbusLightRTUServerReadCoils(int16_t Address);

void PMMModbusLightRTUServerSetup(int8_t SerialPortSelection, int16_t BudeRate, int16_t ModBusConfigration, int8_t SlaveID, int8_t SerialSelectionPin,
                                  bool HoldingRegEnable, int16_t StartaAddressHoldingReg, int16_t QuantityHoldingReg,
                                  bool CoilsEnable, int16_t StartaAddressCoils, int16_t QuantityCoils)
{
    if (SerialPortSelection == 1)
        mb.config(&Serial, BudeRate, ModBusConfigration, SerialSelectionPin);
    if (SerialPortSelection == 2)
        mb.config(&Serial1, BudeRate, ModBusConfigration, SerialSelectionPin);
    if (SerialPortSelection == 3)
        mb.config(&Serial2, BudeRate, ModBusConfigration, SerialSelectionPin);
    mb.setSlaveId(SlaveID);
    if (HoldingRegEnable)
    {

        for (int i = StartaAddressHoldingReg; i < (StartaAddressHoldingReg + QuantityHoldingReg); i++)
        {
            mb.addHreg(i, 0);
        }
    }
    if (CoilsEnable)

    {
        for (int i = StartaAddressCoils; i < (StartaAddressCoils + QuantityCoils); i++)
        {
            mb.addCoil(i, 0);
        }
    }
}

void PMMModbusLightRTUServerWriteHoldingReg(int16_t Address, int16_t Value)
{
    mb.task();
    mb.Hreg(Address, Value);
}
int16_t PMMModbusLightRTUServerReadHoldingReg(int16_t Address)
{
    mb.task();
    mb.Hreg(Address);
}

void PMMModbusLightRTUServerWriteCoils(int16_t Address, bool Value)
{
    mb.task();
    mb.Coil(Address, Value);
}
int16_t PMMModbusLightRTUServerReadCoils(int16_t Address)
{
    mb.task();
    mb.Coil(Address);
}

#endif