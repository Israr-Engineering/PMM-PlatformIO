#include "PmmConfigrature.h"

char *strings[32]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;
string values[32];

int binaryInt[16];
string binary[16];

/*****************************************************************
 * Common functions for all types of ROM
 ******************************************************************/

void PmmStringToArray(string input)
{
    // declaring character array (+1 for null terminator)
    char *char_array = new char[128];

    // copying the contents of the
    // string to char array
    strcpy(char_array, input.c_str());

    byte index = 0;
    ptr = strtok(char_array, ","); // delimiter
    while (ptr != NULL)
    {
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, ",");
    }

    for (int n = 0; n < (index); n++)
    {
        string s(strings[n]);
        values[n] = s;
    }
}

void PmmConvertDecimalToBinary(int Dic)
{
    for (int i = 0; Dic > 0; i++)
    {
        binaryInt[i] = Dic % 2;
        Dic = Dic / 2;
    }
}

void PMMIsAlive()
{
    SerialUSB.println("PMMAlive");
}

/*****************************************************************
 * Internal flash section
 ******************************************************************/
Product ThisProduct;
// storage area definitions each should be max 256 bytes
FlashStorage(Product_flash_store, Product);
FlashStorage(General_flash_store, PMMGeneral);
FlashStorage(RTU_flash_store, PMMRTU);
FlashStorage(TCPUDP_flash_store, PMMTCPUDP);
FlashStorage(Modbus_flash_store, PMMModBus);
FlashStorage(Timers_flash_store, PMMTimer);

void PmmWriteGeneralSettingsInternalFlash(string Message)
{
    PmmStringToArray(Message);

    ThisProduct.PmmGeneral.DeviceName = stoi(values[0]);
    ThisProduct.PmmGeneral.SerialNumber = stoi(values[1]);
    ThisProduct.PmmGeneral.FirstTimeRun = stoi(values[2]);
    ThisProduct.PmmGeneral.LifeTime = stoi(values[3]);
    ThisProduct.PmmGeneral.NumberOfCycles = stoi(values[4]);
    ThisProduct.PmmGeneral.LastRunningTime = stoi(values[5]);
    ThisProduct.PmmGeneral.NumberOfRunningTimes = stoi(values[6]);
    ThisProduct.PmmGeneral.SoftwareVersion = stoi(values[7]);
    ThisProduct.PmmGeneral.FirmwareVersion = stoi(values[8]);
    ThisProduct.PmmGeneral.HardwareVersion = stoi(values[9]);

    PmmConvertDecimalToBinary(stoi(values[10]));

    ThisProduct.PmmGeneral.WebServer = binaryInt[11];
    ThisProduct.PmmGeneral.CaseType = binaryInt[10];
    ThisProduct.PmmGeneral.FiberOption = binaryInt[9];
    ThisProduct.PmmGeneral.SingleModeFiber = binaryInt[8];
    ThisProduct.PmmGeneral.MultiModeFiber = binaryInt[7];
    ThisProduct.PmmGeneral.GSM = binaryInt[6];
    ThisProduct.PmmGeneral.GPS = binaryInt[5];
    ThisProduct.PmmGeneral.Antenna = binaryInt[4];
    ThisProduct.PmmGeneral.ExternalRTC = binaryInt[3];
    ThisProduct.PmmGeneral.InternalRTC = binaryInt[2];
    ThisProduct.PmmGeneral.UDPOption = binaryInt[1];
    ThisProduct.PmmGeneral.GateWay = binaryInt[0];

    ThisProduct.PmmGeneral.ControlerType = stoi(values[11]);
    ThisProduct.PmmGeneral.MinOprationTemperature = stoi(values[12]);
    ThisProduct.PmmGeneral.MaxOprationTemperature = stoi(values[13]);
    ThisProduct.PmmGeneral.NumberOfInputs = stoi(values[14]);
    ThisProduct.PmmGeneral.NumberOfOutputs = stoi(values[15]);
    ThisProduct.PmmGeneral.NumberOfSerials = stoi(values[16]);
    ThisProduct.PmmGeneral.NumberOfUDPPorts = stoi(values[17]);
    ThisProduct.PmmGeneral.MinReadValue = stoi(values[18]);
    ThisProduct.PmmGeneral.MaxReadValue = stoi(values[19]);
    ThisProduct.PmmGeneral.OprationVoltage = stoi(values[20]);
    ThisProduct.PmmGeneral.GeneralReadingsOffset = stoi(values[21]);
    ThisProduct.PmmGeneral.GeneralReadingsFactor = stoi(values[22]);

    General_flash_store.write(ThisProduct.PmmGeneral);
}

void PmmWriteRTUSettingsInternalFlash(string Message)
{
    PmmStringToArray(Message);

    ThisProduct.PmmRTU.PortOneName = stoi(values[0]);
    ThisProduct.PmmRTU.PortOneBaudRate = stoi(values[1]);
    ThisProduct.PmmRTU.PortOneStopBit = stoi(values[2]);
    ThisProduct.PmmRTU.PortOneDataBit = stoi(values[3]);
    ThisProduct.PmmRTU.PortOneParity = stoi(values[4]);
    ThisProduct.PmmRTU.PortOneConnectionTimeout = stoi(values[5]);
    ThisProduct.PmmRTU.PortOneMaxRetryRTU = stoi(values[6]);
    ThisProduct.PmmRTU.PortOneInterface = stoi(values[7]);
    ThisProduct.PmmRTU.PortTwoName = stoi(values[8]);
    ThisProduct.PmmRTU.PortTwoBaudRate = stoi(values[9]);
    ThisProduct.PmmRTU.PortTwoStopBit = stoi(values[10]);
    ThisProduct.PmmRTU.PortTwoDataBit = stoi(values[11]);
    ThisProduct.PmmRTU.PortTwoParity = stoi(values[12]);
    ThisProduct.PmmRTU.PortTwoConnectionTimeout = stoi(values[13]);
    ThisProduct.PmmRTU.PortTwoMaxRetryRTU = stoi(values[14]);
    ThisProduct.PmmRTU.PortTwoInterface = stoi(values[15]);
    ThisProduct.PmmRTU.PortThreeName = stoi(values[16]);
    ThisProduct.PmmRTU.PortThreeBaudRate = stoi(values[17]);
    ThisProduct.PmmRTU.PortThreeStopBit = stoi(values[18]);
    ThisProduct.PmmRTU.PortThreeDataBit = stoi(values[19]);
    ThisProduct.PmmRTU.PortThreeParity = stoi(values[20]);
    ThisProduct.PmmRTU.PortThreeConnectionTimeout = stoi(values[21]);
    ThisProduct.PmmRTU.PortThreeMaxRetryRTU = stoi(values[22]);
    ThisProduct.PmmRTU.PortThreeInterface = stoi(values[23]);
    ThisProduct.PmmRTU.PortFourName = stoi(values[24]);
    ThisProduct.PmmRTU.PortFourBaudRate = stoi(values[25]);
    ThisProduct.PmmRTU.PortFourStopBit = stoi(values[26]);
    ThisProduct.PmmRTU.PortFourDataBit = stoi(values[27]);
    ThisProduct.PmmRTU.PortFourParity = stoi(values[28]);
    ThisProduct.PmmRTU.PortFourConnectionTimeout = stoi(values[26]);
    ThisProduct.PmmRTU.PortFourMaxRetryRTU = stoi(values[30]);
    ThisProduct.PmmRTU.PortFourInterface = stoi(values[31]);

    RTU_flash_store.write(ThisProduct.PmmRTU);
}

void PmmWriteTCPUDPSettingsInternalFlash(string Message)
{
    PmmStringToArray(Message);

    // TCP Settings
    ThisProduct.PmmTCPUDP.MacAddress01 = stoi(values[0]);
    ThisProduct.PmmTCPUDP.MacAddress02 = stoi(values[1]);
    ThisProduct.PmmTCPUDP.MacAddress03 = stoi(values[2]);
    ThisProduct.PmmTCPUDP.MacAddress04 = stoi(values[3]);
    ThisProduct.PmmTCPUDP.MacAddress05 = stoi(values[4]);

    ThisProduct.PmmTCPUDP.IPAddress01 = stoi(values[5]);
    ThisProduct.PmmTCPUDP.IPAddress02 = stoi(values[6]);
    ThisProduct.PmmTCPUDP.IPAddress03 = stoi(values[7]);
    ThisProduct.PmmTCPUDP.IPAddress04 = stoi(values[8]);

    ThisProduct.PmmTCPUDP.RemoteIPAddress01 = stoi(values[9]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress02 = stoi(values[10]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress03 = stoi(values[11]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress04 = stoi(values[12]);

    ThisProduct.PmmTCPUDP.NetMask01 = stoi(values[13]);
    ThisProduct.PmmTCPUDP.NetMask02 = stoi(values[14]);
    ThisProduct.PmmTCPUDP.NetMask03 = stoi(values[15]);
    ThisProduct.PmmTCPUDP.NetMask04 = stoi(values[16]);

    ThisProduct.PmmTCPUDP.DNSSOneServer01 = stoi(values[17]);
    ThisProduct.PmmTCPUDP.DNSSOneServer02 = stoi(values[18]);
    ThisProduct.PmmTCPUDP.DNSSOneServer03 = stoi(values[19]);
    ThisProduct.PmmTCPUDP.DNSSOneServer04 = stoi(values[20]);

    ThisProduct.PmmTCPUDP.DNSTwoServer01 = stoi(values[21]);
    ThisProduct.PmmTCPUDP.DNSTwoServer02 = stoi(values[22]);
    ThisProduct.PmmTCPUDP.DNSTwoServer03 = stoi(values[23]);
    ThisProduct.PmmTCPUDP.DNSTwoServer04 = stoi(values[24]);

    ThisProduct.PmmTCPUDP.ConnectionTimeoutTCP = stoi(values[25]);
    ThisProduct.PmmTCPUDP.MaxRetryTCP = stoi(values[26]);
    ThisProduct.PmmTCPUDP.UDPPortOne = stoi(values[27]);
    ThisProduct.PmmTCPUDP.UDPPortTwo = stoi(values[28]);
    ThisProduct.PmmTCPUDP.UDPPortThree = stoi(values[29]);
    ThisProduct.PmmTCPUDP.UDPPortFour = stoi(values[30]);

    TCPUDP_flash_store.write(ThisProduct.PmmTCPUDP);
}

void PmmWriteModbusSettingsInternalFlash(string Message)
{
    PmmStringToArray(Message);

    PmmConvertDecimalToBinary(stoi(values[0]));

    ThisProduct.PmmModbus.ModBusRTU = binaryInt[12];
    ThisProduct.PmmModbus.ModBusTCP = binaryInt[11];
    ThisProduct.PmmModbus.ModBusUDP = binaryInt[10];
    ThisProduct.PmmModbus.ModBusSlave = binaryInt[9];
    ThisProduct.PmmModbus.ModBusMaster = binaryInt[8];
    ThisProduct.PmmModbus.ReadCoilsStatus = binaryInt[7];
    ThisProduct.PmmModbus.ReadInputStatus = binaryInt[6];
    ThisProduct.PmmModbus.ReadHoldingRegisters = binaryInt[5];
    ThisProduct.PmmModbus.ReadInputRegisters = binaryInt[4];
    ThisProduct.PmmModbus.WriteSingleCoil = binaryInt[3];
    ThisProduct.PmmModbus.WriteSingleRegister = binaryInt[2];
    ThisProduct.PmmModbus.WriteMultipleCoils = binaryInt[1];
    ThisProduct.PmmModbus.WriteMultipleRegisters = binaryInt[0];
    ThisProduct.PmmModbus.StartingAddressCoilsStatus = stoi(values[1]);
    ThisProduct.PmmModbus.StartingAddressInputStatus = stoi(values[2]);
    ThisProduct.PmmModbus.StartingAddressHoldingRegisters = stoi(values[3]);
    ThisProduct.PmmModbus.StartingAddressInputRegisters = stoi(values[4]);
    ThisProduct.PmmModbus.StartingAddressWriteSingleCoil = stoi(values[5]);
    ThisProduct.PmmModbus.StartingAddressWriteSingleRegister = stoi(values[6]);
    ThisProduct.PmmModbus.StartingAddressWriteMultipleCoils = stoi(values[7]);
    ThisProduct.PmmModbus.StartingAddressWriteMultipleRegisters = stoi(values[8]);
    ThisProduct.PmmModbus.QuantityCoilsStatus = stoi(values[9]);
    ThisProduct.PmmModbus.QuantityInputStatus = stoi(values[10]);
    ThisProduct.PmmModbus.QuantityHoldingRegisters = stoi(values[11]);
    ThisProduct.PmmModbus.QuantityInputRegisters = stoi(values[12]);
    ThisProduct.PmmModbus.QuantityWriteMultipleCoils = stoi(values[13]);
    ThisProduct.PmmModbus.QuantityWriteMultipleRegisters = stoi(values[14]);
    ThisProduct.PmmModbus.FunctionCode = stoi(values[15]);
    ThisProduct.PmmModbus.IODataOrder = stoi(values[16]);
    ThisProduct.PmmModbus.IODataType = stoi(values[17]);
    ThisProduct.PmmModbus.PollInterval = stoi(values[18]);
    ThisProduct.PmmModbus.SlaveID = stoi(values[19]);
    ThisProduct.PmmModbus.StartingAddress = stoi(values[20]);
    ThisProduct.PmmModbus.Quantity = stoi(values[21]);

    Modbus_flash_store.write(ThisProduct.PmmModbus);
}

void PmmWriteTimerSettingsInternalFlash(string Message)
{
    PmmStringToArray(Message);

    PmmConvertDecimalToBinary(stoi(values[0]));

    ThisProduct.PmmTimers.CycleTimer = binaryInt[7];
    ThisProduct.PmmTimers.OneMSTimer = binaryInt[6];
    ThisProduct.PmmTimers.TenMSTimer = binaryInt[5];
    ThisProduct.PmmTimers.OneSecTimer = binaryInt[4];
    ThisProduct.PmmTimers.OneMinTimer = binaryInt[3];
    ThisProduct.PmmTimers.OneHouTimer = binaryInt[2];
    ThisProduct.PmmTimers.OneMonTimer = binaryInt[1];
    ThisProduct.PmmTimers.OneYearTimer = binaryInt[0];

    Timers_flash_store.write(ThisProduct.PmmTimers);
}

void PmmReadGeneralSettingsInternalFlash()
{

    ThisProduct.PmmGeneral = General_flash_store.read();
    String settings = "";

    settings = String(settings + String(ThisProduct.PmmGeneral.DeviceName));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.SerialNumber));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.FirstTimeRun));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.LifeTime));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.NumberOfCycles));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.LastRunningTime));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.NumberOfRunningTimes));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.SoftwareVersion));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.FirmwareVersion));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.HardwareVersion));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.WebServer));
    settings = String(settings + String(ThisProduct.PmmGeneral.CaseType));
    settings = String(settings + String(ThisProduct.PmmGeneral.FiberOption));
    settings = String(settings + String(ThisProduct.PmmGeneral.SingleModeFiber));
    settings = String(settings + String(ThisProduct.PmmGeneral.MultiModeFiber));
    settings = String(settings + String(ThisProduct.PmmGeneral.GSM));
    settings = String(settings + String(ThisProduct.PmmGeneral.GPS));
    settings = String(settings + String(ThisProduct.PmmGeneral.Antenna));
    settings = String(settings + String(ThisProduct.PmmGeneral.ExternalRTC));
    settings = String(settings + String(ThisProduct.PmmGeneral.InternalRTC));
    settings = String(settings + String(ThisProduct.PmmGeneral.UDPOption));
    settings = String(settings + String(ThisProduct.PmmGeneral.GateWay));
    settings = String(settings + String(ThisProduct.PmmGeneral.ControlerType));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.MinOprationTemperature));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.MaxOprationTemperature));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.NumberOfInputs));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.NumberOfOutputs));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.NumberOfSerials));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.NumberOfUDPPorts));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.MinReadValue));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.MaxReadValue));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.OprationVoltage));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.GeneralReadingsOffset));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.GeneralReadingsFactor));
    settings = String(settings + String(",End"));

    SerialUSB.println(settings);
}

void PmmReadRTUSettingsInternalFlash()
{
    ThisProduct.PmmRTU = RTU_flash_store.read();
    String settings = "";

    settings = String(settings + String(ThisProduct.PmmRTU.PortOneName));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortOneBaudRate));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortOneStopBit));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortOneDataBit));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortOneParity));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortOneConnectionTimeout));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortOneMaxRetryRTU));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortOneInterface));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortTwoName));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortTwoBaudRate));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortTwoStopBit));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortTwoDataBit));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortTwoParity));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortTwoConnectionTimeout));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortTwoMaxRetryRTU));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortTwoInterface));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortThreeName));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortThreeBaudRate));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortThreeStopBit));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortThreeDataBit));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortThreeParity));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortThreeConnectionTimeout));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortThreeMaxRetryRTU));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortThreeInterface));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortFourName));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortFourBaudRate));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortFourStopBit));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortFourDataBit));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortFourParity));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortFourConnectionTimeout));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortFourMaxRetryRTU));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmRTU.PortFourInterface));
    settings = String(settings + ",End");

    SerialUSB.println(settings);
}

void PmmReadTCPUDPSettingsInternalFlash()
{

    ThisProduct.PmmTCPUDP = TCPUDP_flash_store.read();
    String settings = "";

    settings = String(settings + String(ThisProduct.PmmTCPUDP.MacAddress01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.MacAddress02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.MacAddress03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.MacAddress04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.MacAddress05));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.IPAddress01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.IPAddress02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.IPAddress03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.IPAddress04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.RemoteIPAddress01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.RemoteIPAddress02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.RemoteIPAddress03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.RemoteIPAddress04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.NetMask01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.NetMask02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.NetMask03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.NetMask04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.DNSSOneServer01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.DNSSOneServer02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.DNSSOneServer03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.DNSSOneServer04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.DNSTwoServer01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.DNSTwoServer02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.DNSTwoServer03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.DNSTwoServer04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.ConnectionTimeoutTCP));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.MaxRetryTCP));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.UDPPortOne));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.UDPPortTwo));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.UDPPortThree));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTCPUDP.UDPPortFour));
    settings = String(settings + ",End");

    SerialUSB.println(settings);
}

void PmmReadModbusSettingsInternalFlash()
{
    ThisProduct.PmmModbus = Modbus_flash_store.read();
    String settings = "";

    settings = String(settings + String(ThisProduct.PmmModbus.ModBusRTU));
    settings = String(settings + String(ThisProduct.PmmModbus.ModBusTCP));
    settings = String(settings + String(ThisProduct.PmmModbus.ModBusUDP));
    settings = String(settings + String(ThisProduct.PmmModbus.ModBusSlave));
    settings = String(settings + String(ThisProduct.PmmModbus.ModBusMaster));
    settings = String(settings + String(ThisProduct.PmmModbus.ReadCoilsStatus));
    settings = String(settings + String(ThisProduct.PmmModbus.ReadInputStatus));
    settings = String(settings + String(ThisProduct.PmmModbus.ReadHoldingRegisters));
    settings = String(settings + String(ThisProduct.PmmModbus.ReadInputRegisters));
    settings = String(settings + String(ThisProduct.PmmModbus.WriteSingleCoil));
    settings = String(settings + String(ThisProduct.PmmModbus.WriteSingleRegister));
    settings = String(settings + String(ThisProduct.PmmModbus.WriteMultipleCoils));
    settings = String(settings + String(ThisProduct.PmmModbus.WriteMultipleRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressCoilsStatus));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressInputStatus));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressHoldingRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressInputRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressWriteSingleCoil));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressWriteSingleRegister));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressWriteMultipleCoils));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressWriteMultipleRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.QuantityCoilsStatus));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.QuantityInputStatus));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.QuantityHoldingRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.QuantityInputRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.QuantityWriteMultipleCoils));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.QuantityWriteMultipleRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.FunctionCode));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.IODataOrder));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.IODataType));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.PollInterval));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.SlaveID));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.StartingAddress));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmModbus.Quantity));
    settings = String(settings + ",End");

    SerialUSB.println(settings);
}

void PmmReadTimersSettingsInternalFlash()
{
    ThisProduct.PmmTimers = Timers_flash_store.read();
    String settings = "";

    settings = String(settings + String(ThisProduct.PmmTimers.CycleTimer));
    settings = String(settings + String(ThisProduct.PmmTimers.OneMSTimer));
    settings = String(settings + String(ThisProduct.PmmTimers.TenMSTimer));
    settings = String(settings + String(ThisProduct.PmmTimers.OneSecTimer));
    settings = String(settings + String(ThisProduct.PmmTimers.OneMinTimer));
    settings = String(settings + String(ThisProduct.PmmTimers.OneHouTimer));
    settings = String(settings + String(ThisProduct.PmmTimers.OneMonTimer));
    settings = String(settings + String(ThisProduct.PmmTimers.OneYearTimer));
    settings = String(settings + ",End");

    SerialUSB.println(settings);
}
/*
void SetTCPSettings(string Message) // Save TCP Settings to internal flash
{
    PmmStringToArray(Message);

    // TCP Settings
    // ThisProduct.tcpudp
    ThisProduct.tcpudp.ip_address01 = stoi(values[0]);
    ThisProduct.tcpudp.ip_address02 = stoi(values[1]);
    ThisProduct.tcpudp.ip_address03 = stoi(values[2]);
    ThisProduct.tcpudp.ip_address04 = stoi(values[3]);

    ThisProduct.tcpudp.net_mask01 = stoi(values[4]);
    ThisProduct.tcpudp.net_mask02 = stoi(values[5]);
    ThisProduct.tcpudp.net_mask03 = stoi(values[6]);
    ThisProduct.tcpudp.net_mask04 = stoi(values[7]);

    ThisProduct.tcpudp.preferred_dns_server01 = stoi(values[8]);
    ThisProduct.tcpudp.preferred_dns_server02 = stoi(values[9]);
    ThisProduct.tcpudp.preferred_dns_server03 = stoi(values[10]);
    ThisProduct.tcpudp.preferred_dns_server04 = stoi(values[11]);

    ThisProduct.tcpudp.alternate_dns_server01 = stoi(values[12]);
    ThisProduct.tcpudp.alternate_dns_server02 = stoi(values[13]);
    ThisProduct.tcpudp.alternate_dns_server03 = stoi(values[14]);
    ThisProduct.tcpudp.alternate_dns_server04 = stoi(values[15]);

    ThisProduct.tcpudp.default_gateway01 = stoi(values[16]);
    ThisProduct.tcpudp.default_gateway02 = stoi(values[17]);
    ThisProduct.tcpudp.default_gateway03 = stoi(values[18]);
    ThisProduct.tcpudp.default_gateway04 = stoi(values[20]);

    ThisProduct.tcpudp.mac_address01 = stoi(values[21]);
    ThisProduct.tcpudp.mac_address02 = stoi(values[22]);
    ThisProduct.tcpudp.mac_address03 = stoi(values[23]);
    ThisProduct.tcpudp.mac_address04 = stoi(values[24]);
    ThisProduct.tcpudp.mac_address05 = stoi(values[25]);

    ThisProduct.tcpudp.connection_timeout_tcp = stoi(values[26]);
    ThisProduct.tcpudp.max_retry_tcp = stoi(values[27]);
    ThisProduct.tcpudp.udp_port_one = stoi(values[28]);
    ThisProduct.tcpudp.udp_port_two = stoi(values[29]);
    ThisProduct.tcpudp.udp_port_three = stoi(values[30]);
    ThisProduct.tcpudp.udp_port_four = stoi(values[31]);

    // // ...and finally save everything into "my_flash_store"
    // my_flash_store.write(ThisProduct.tcpudp);
    TCP_flash_store.write(ThisProduct.tcpudp);
}

String GetTCPSettings() // Get TCP Settings From internal flash
{
    // PmmTCPUDP ThisProduct.tcpudp;

    ThisProduct.tcpudp = TCP_flash_store.read();

    String settings = "";

    settings = String(settings + String(ThisProduct.tcpudp.ip_address01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.ip_address02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.ip_address03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.ip_address04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.net_mask01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.net_mask02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.net_mask03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.net_mask04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.preferred_dns_server01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.preferred_dns_server02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.preferred_dns_server03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.preferred_dns_server04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.default_gateway01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.default_gateway02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.default_gateway03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.default_gateway04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.mac_address01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.mac_address02));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.mac_address03));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.mac_address04));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.mac_address05));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.connection_timeout_tcp));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.max_retry_tcp));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.udp_port_one));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.udp_port_two));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.udp_port_three));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.tcpudp.udp_port_four));

    SerialUSB.println(settings);
    return settings;
}

void SetProductSettings(string Message) // Save product Settings to internal flash
{

    PmmStringToArray(Message);

    // Product ThisProduct;

    // Start writing the procedure
    ThisProduct.serial_number = stol(values[0]);
    ThisProduct.ProductName = stoi(values[1]);
    ThisProduct.firmware_version01 = stoi(values[2]);
    ThisProduct.hardware_version01 = stoi(values[3]);
    ThisProduct.software_version01 = stoi(values[4]);
    ThisProduct.cpu_type = stoi(values[5]);
    ThisProduct.connection_type = stoi(values[6]);
    ThisProduct.slave_i2c_address = stoi(values[7]);

    // End procedure

    Product_flash_store.write(ThisProduct);
}

String GetProductSettings() // Get product Settings From internal flash
{

    ThisProduct = Product_flash_store.read();

    String settings = "";

    // Start writing the procedure
    settings = String(settings + String(ThisProduct.serial_number));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.ProductName));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.firmware_version01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.hardware_version01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.software_version01));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.cpu_type));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.connection_type));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.slave_i2c_address));
    settings = String(settings + ",");

    // End procedure

    SerialUSB.println(settings);
    return settings;
}

*/

/*****************************************************************
 * External flash section
 ******************************************************************/

/*****************************************************************
 * External EEPROM flash section
 ******************************************************************/
