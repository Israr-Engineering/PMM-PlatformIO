// #include "PmmCommands.h"
// #include "PmmConfigrature.h"

#include <Arduino.h>
#include <string>

#include <PmmTypes.h>
#include <PmmEthernet.h>
#include <PmmFlashStorage.h>
#include <PmmSPISerialFlash.h>
#include <PmmExternalEEPROMLib.h>

#include <PmmInternalRTC.h>

// #include <PmmExternalRTC.h>

#include <PmmDS3231.h>

#include "PmmCommands.h"

/*
 // Common functions
void PmmStringToArray(string input);
void PMMIsAlive();
void PmmConvertDecimalToBinary(int Dic);
//Internal flash ROM functions
void PmmWriteGeneralSettingsInternalFlash(string Message);
void PmmWriteRTUSettingsInternalFlash(string Message);
void PmmWriteTCPUDPSettingsInternalFlash(string Message);
void PmmWriteModbusSettingsInternalFlash(string Message);
void PmmWriteTimerSettingsInternalFlash(string Message);
void PmmReadGeneralSettingsInternalFlash();
void PmmReadRTUSettingsInternalFlash();
void PmmReadTCPUDPSettingsInternalFlash();
void PmmReadModbusSettingsInternalFlash();
void PmmReadTimersSettingsInternalFlash();


// Command
 void PMMInitializeEthernet(IPAddress ip, byte mac[]);
 string PMMReadCommands();
 string PMMCommnads(string readData);
 string PMMReturnDataFromSerialUSB();
 //string PMMReturnDataFromAPIHTTPHeader();


  */

void PMMReadCommands();
void PMMSendDataHTTPClient(String Data);

String SetInternalRTC(string Message);
String GetInternalRTC();

String SetExternalRTC(string Message);
String GetExternalRTC();

PmmEthernetServer server(80);
PmmEthernetClient client = server.available();

char *strings[32]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;
string values[32];

// rastest = 55 ;

int binaryInt[16];
string binary[16];
Product ThisProduct;
PMMIO PmmIO;
PmmInternalRTC PmmRTCInternal;

PmmDS3231 PmmRTCExternal;
// Extra functions if needed
PmmRTClib PMMDS3231A;

/*****************************************************************
 * Common functions for all types of ROM
 ******************************************************************/

void PmmStringToArray(string input)
{
    // declaring character array (+1 for null terminator)
    char *char_array = new char[128];

    SerialUSB.println(input.c_str());

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
        SerialUSB.println(s.c_str());
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

String PMMIsAlive()
{
    return "PMMAlive";
}

void StartCommandHttpServer()
{
    if (ThisProduct.EthernetRunning)
    {
        server.begin(); // port 80
    }
    else
    {
        SerialUSB.println("Http server fail : no ethernet running .");
    }
}

void StartEEprom()
{
    if (ThisProduct.PmmGeneral.ItHasExtEEPROM == true && ThisProduct.PmmGeneral.SettingsRef == 3)
    {
        EEpromSetup();
    }
}

/*****************************************************************
 * Internal flash section
 ******************************************************************/

// storage area definitions each should be max 256 bytes
FlashStorage(Product_flash_store, Product);
FlashStorage(General_flash_store, PMMGeneral);

FlashStorage(TCPUDP_flash_store, PMMTCPUDP);
FlashStorage(TCPUDP_Protocol_store, PMMModBusPort);

FlashStorage(Serial_flash_store01, PMMSERIAL);
FlashStorage(Serial_Protoco_store01, PMMModBusPort);

FlashStorage(Serial_flash_store02, PMMSERIAL);
FlashStorage(Serial_Protoco_store02, PMMModBusPort);

FlashStorage(Serial_flash_store03, PMMSERIAL);
FlashStorage(Serial_Protoco_store03, PMMModBusPort);

FlashStorage(Serial_flash_store04, PMMSERIAL);
FlashStorage(Serial_Protoco_store04, PMMModBusPort);

FlashStorage(Timers_flash_store, PMMTimer);

FlashStorage(Claibration_flash_Page01, PMMDeviceCalibration);
FlashStorage(Claibration_flash_Page02, PMMDeviceCalibration);

FlashStorage(GerneralPurpose_flash_Page02, GerneralPurpose);


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
    ThisProduct.PmmGeneral.ControlerType = stoi(values[10]);

    PmmConvertDecimalToBinary(stoi(values[11]));

    ThisProduct.PmmGeneral.ItHasEthernet = binaryInt[15];
    ThisProduct.PmmGeneral.ItHasSwitch = binaryInt[14];
    ThisProduct.PmmGeneral.ItHasExtEEPROM = binaryInt[13];
    ThisProduct.PmmGeneral.ItHasExtFlash = binaryInt[12];
    ThisProduct.PmmGeneral.ItHasSerial = binaryInt[11];
    ThisProduct.PmmGeneral.ItHasWebServer = binaryInt[10];
    ThisProduct.PmmGeneral.ItHasFiber = binaryInt[9];
    ThisProduct.PmmGeneral.LoRA = binaryInt[8];
    ThisProduct.PmmGeneral.Zidbee = binaryInt[7];
    ThisProduct.PmmGeneral.GSM = binaryInt[6];
    ThisProduct.PmmGeneral.GPS = binaryInt[5];
    ThisProduct.PmmGeneral.Antenna = binaryInt[4];
    ThisProduct.PmmGeneral.ExternalRTC = binaryInt[3];
    ThisProduct.PmmGeneral.InternalRTC = binaryInt[2];
    ThisProduct.PmmGeneral.UDPOption = binaryInt[1];
    ThisProduct.PmmGeneral.GateWay = binaryInt[0];

    PmmConvertDecimalToBinary(stoi(values[12]));

    ThisProduct.PmmGeneral.IsModBus = binaryInt[8];
    ThisProduct.PmmGeneral.IsCanBus = binaryInt[7];
    ThisProduct.PmmGeneral.IsProfiBus = binaryInt[6];
    ThisProduct.PmmGeneral.IsProfiNet = binaryInt[5];
    ThisProduct.PmmGeneral.IsBACnet = binaryInt[4];
    ThisProduct.PmmGeneral.IsDLMS = binaryInt[3];
    ThisProduct.PmmGeneral.IsMBus = binaryInt[2];
    ThisProduct.PmmGeneral.IsOPC = binaryInt[1];
    ThisProduct.PmmGeneral.Canprint = binaryInt[0];

    ThisProduct.PmmGeneral.NumberOfInputs = stoi(values[13]);
    ThisProduct.PmmGeneral.NumberOfOutputs = stoi(values[14]);
    ThisProduct.PmmGeneral.NumberOfSerials = stoi(values[15]);
    ThisProduct.PmmGeneral.NumberOfUDPPorts = stoi(values[16]);
    ThisProduct.PmmGeneral.MinReadValue = stoi(values[17]);
    ThisProduct.PmmGeneral.MaxReadValue = stoi(values[18]);
    ThisProduct.PmmGeneral.OprationVoltage = stoi(values[19]);
    ThisProduct.PmmGeneral.GeneralReadingsOffset = stoi(values[20]);
    ThisProduct.PmmGeneral.GeneralReadingsFactor = stoi(values[21]);
    ThisProduct.PmmGeneral.SettingsRef = stoi(values[22]);

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
    ThisProduct.PmmTCPUDP.MacAddress06 = stoi(values[5]);

    ThisProduct.PmmTCPUDP.IPAddress01 = stoi(values[6]);
    ThisProduct.PmmTCPUDP.IPAddress02 = stoi(values[7]);
    ThisProduct.PmmTCPUDP.IPAddress03 = stoi(values[8]);
    ThisProduct.PmmTCPUDP.IPAddress04 = stoi(values[9]);

    ThisProduct.PmmTCPUDP.RemoteIPAddress01 = stoi(values[10]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress02 = stoi(values[11]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress03 = stoi(values[12]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress04 = stoi(values[13]);

    ThisProduct.PmmTCPUDP.NetMask01 = stoi(values[14]);
    ThisProduct.PmmTCPUDP.NetMask02 = stoi(values[15]);
    ThisProduct.PmmTCPUDP.NetMask03 = stoi(values[16]);
    ThisProduct.PmmTCPUDP.NetMask04 = stoi(values[17]);

    ThisProduct.PmmTCPUDP.DNSSOneServer01 = stoi(values[18]);
    ThisProduct.PmmTCPUDP.DNSSOneServer02 = stoi(values[19]);
    ThisProduct.PmmTCPUDP.DNSSOneServer03 = stoi(values[20]);
    ThisProduct.PmmTCPUDP.DNSSOneServer04 = stoi(values[21]);

    ThisProduct.PmmTCPUDP.DNSTwoServer01 = stoi(values[22]);
    ThisProduct.PmmTCPUDP.DNSTwoServer02 = stoi(values[23]);
    ThisProduct.PmmTCPUDP.DNSTwoServer03 = stoi(values[24]);
    ThisProduct.PmmTCPUDP.DNSTwoServer04 = stoi(values[25]);

    ThisProduct.PmmTCPUDP.ConnectionTimeoutTCP = stoi(values[26]);
    ThisProduct.PmmTCPUDP.MaxRetryTCP = stoi(values[27]);
    ThisProduct.PmmTCPUDP.UDPPortOne = stoi(values[28]);
    ThisProduct.PmmTCPUDP.UDPPortTwo = stoi(values[29]);
    ThisProduct.PmmTCPUDP.UDPPortThree = stoi(values[30]);
    ThisProduct.PmmTCPUDP.UDPPortFour = stoi(values[31]);

    TCPUDP_flash_store.write(ThisProduct.PmmTCPUDP);
}

// void PmmWriteModbusSettingsInternalFlash(string Message)
// {
//     PmmStringToArray(Message);

//     PmmConvertDecimalToBinary(stoi(values[0]));

//     ThisProduct.PmmModbus.ModBusRTU = binaryInt[15];
//     ThisProduct.PmmModbus.ModBusTCP = binaryInt[14];
//     ThisProduct.PmmModbus.ModBusSlave = binaryInt[13];
//     ThisProduct.PmmModbus.ModBusMaster = binaryInt[12];
//     ThisProduct.PmmModbus.ReadCoilsStatus = binaryInt[11];
//     ThisProduct.PmmModbus.ReadInputStatus = binaryInt[10];
//     ThisProduct.PmmModbus.ReadHoldingRegisters = binaryInt[9];
//     ThisProduct.PmmModbus.ReadInputRegisters = binaryInt[8];
//     ThisProduct.PmmModbus.WriteSingleCoil = binaryInt[7];
//     ThisProduct.PmmModbus.WriteSingleRegister = binaryInt[6];
//     ThisProduct.PmmModbus.WriteMultipleCoils = binaryInt[5];
//     ThisProduct.PmmModbus.WriteMultipleRegisters = binaryInt[4];

//     ThisProduct.PmmModbus.CoilsStatus = binaryInt[3];
//     ThisProduct.PmmModbus.InputStatus = binaryInt[2];
//     ThisProduct.PmmModbus.HoldingRegisters = binaryInt[1];
//     ThisProduct.PmmModbus.InputRegisters = binaryInt[0];

//     ThisProduct.PmmModbus.StartingAddressCoilsStatus = stoi(values[1]);
//     ThisProduct.PmmModbus.StartingAddressInputStatus = stoi(values[2]);
//     ThisProduct.PmmModbus.StartingAddressHoldingRegisters = stoi(values[3]);
//     ThisProduct.PmmModbus.StartingAddressInputRegisters = stoi(values[4]);
//     ThisProduct.PmmModbus.StartingAddressWriteSingleCoil = stoi(values[5]);
//     ThisProduct.PmmModbus.StartingAddressWriteSingleRegister = stoi(values[6]);
//     ThisProduct.PmmModbus.StartingAddressWriteMultipleCoils = stoi(values[7]);
//     ThisProduct.PmmModbus.StartingAddressWriteMultipleRegisters = stoi(values[8]);
//     ThisProduct.PmmModbus.QuantityCoilsStatus = stoi(values[9]);
//     ThisProduct.PmmModbus.QuantityInputStatus = stoi(values[10]);
//     ThisProduct.PmmModbus.QuantityHoldingRegisters = stoi(values[11]);
//     ThisProduct.PmmModbus.QuantityInputRegisters = stoi(values[12]);
//     ThisProduct.PmmModbus.QuantityWriteMultipleCoils = stoi(values[13]);
//     ThisProduct.PmmModbus.QuantityWriteMultipleRegisters = stoi(values[14]);
//     ThisProduct.PmmModbus.FunctionCode = stoi(values[15]);
//     ThisProduct.PmmModbus.IODataOrder = stoi(values[16]);
//     ThisProduct.PmmModbus.IODataType = stoi(values[17]);
//     ThisProduct.PmmModbus.PollInterval = stoi(values[18]);
//     ThisProduct.PmmModbus.SlaveID = stoi(values[19]);
//     ThisProduct.PmmModbus.StartingAddress = stoi(values[20]);
//     ThisProduct.PmmModbus.Quantity = stoi(values[21]);

//     ThisProduct.PmmModbus.DataBitConfig = stoi(values[22]);
//     ThisProduct.PmmModbus.ParityConfig = stoi(values[23]); // 1=>None,2=>Even,3=>Odd,4=>Mark,5=>Space
//     ThisProduct.PmmModbus.StopBitConfig = stoi(values[24]);
//     ThisProduct.PmmModbus.BaudRate = stoi(values[25]);
//     ThisProduct.PmmModbus.TXPin = stoi(values[26]);
//     ThisProduct.PmmModbus.RXPin = stoi(values[27]);
//     ThisProduct.PmmModbus.SerialSelectionPin = stoi(values[28]);
//     ThisProduct.PmmModbus.SerialPort = stoi(values[29]);

//     Modbus_flash_store.write(ThisProduct.PmmModbus);
// }

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

String PmmReadGeneralSettingsInternalFlash()
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
    settings = String(settings + String(ThisProduct.PmmGeneral.ControlerType));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.ItHasEthernet));
    settings = String(settings + String(ThisProduct.PmmGeneral.ItHasSwitch));
    settings = String(settings + String(ThisProduct.PmmGeneral.ItHasExtEEPROM));
    settings = String(settings + String(ThisProduct.PmmGeneral.ItHasExtFlash));
    settings = String(settings + String(ThisProduct.PmmGeneral.ItHasSerial));
    settings = String(settings + String(ThisProduct.PmmGeneral.ItHasWebServer));
    settings = String(settings + String(ThisProduct.PmmGeneral.ItHasFiber));
    settings = String(settings + String(ThisProduct.PmmGeneral.LoRA));
    settings = String(settings + String(ThisProduct.PmmGeneral.Zidbee));
    settings = String(settings + String(ThisProduct.PmmGeneral.GSM));
    settings = String(settings + String(ThisProduct.PmmGeneral.GPS));
    settings = String(settings + String(ThisProduct.PmmGeneral.Antenna));
    settings = String(settings + String(ThisProduct.PmmGeneral.ExternalRTC));
    settings = String(settings + String(ThisProduct.PmmGeneral.InternalRTC));
    settings = String(settings + String(ThisProduct.PmmGeneral.UDPOption));
    settings = String(settings + String(ThisProduct.PmmGeneral.GateWay));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.IsModBus));
    settings = String(settings + String(ThisProduct.PmmGeneral.IsCanBus));
    settings = String(settings + String(ThisProduct.PmmGeneral.IsProfiBus));
    settings = String(settings + String(ThisProduct.PmmGeneral.IsProfiNet));
    settings = String(settings + String(ThisProduct.PmmGeneral.IsBACnet));
    settings = String(settings + String(ThisProduct.PmmGeneral.IsDLMS));
    settings = String(settings + String(ThisProduct.PmmGeneral.IsMBus));
    settings = String(settings + String(ThisProduct.PmmGeneral.IsOPC));
    settings = String(settings + String(ThisProduct.PmmGeneral.Canprint));
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
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.SettingsRef));

    settings = String(settings + String(",End"));

    // SerialUSB.println(settings);

    return settings;
}

String PmmReadRTUSettingsInternalFlash()
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

    // SerialUSB.println(settings);

    return settings;
}

String PmmReadTCPUDPSettingsInternalFlash()
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
    settings = String(settings + String(ThisProduct.PmmTCPUDP.MacAddress06));
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

    // SerialUSB.println(settings);
    return settings;
}

// String PmmReadModbusSettingsInternalFlash()
// {
//     ThisProduct.PmmModbus = Modbus_flash_store.read();
//     String settings = "";

//     settings = String(settings + String(ThisProduct.PmmModbus.ModBusRTU));
//     settings = String(settings + String(ThisProduct.PmmModbus.ModBusTCP));
//     settings = String(settings + String(ThisProduct.PmmModbus.ModBusSlave));
//     settings = String(settings + String(ThisProduct.PmmModbus.ModBusMaster));
//     settings = String(settings + String(ThisProduct.PmmModbus.ReadCoilsStatus));
//     settings = String(settings + String(ThisProduct.PmmModbus.ReadInputStatus));
//     settings = String(settings + String(ThisProduct.PmmModbus.ReadHoldingRegisters));
//     settings = String(settings + String(ThisProduct.PmmModbus.ReadInputRegisters));
//     settings = String(settings + String(ThisProduct.PmmModbus.WriteSingleCoil));
//     settings = String(settings + String(ThisProduct.PmmModbus.WriteSingleRegister));
//     settings = String(settings + String(ThisProduct.PmmModbus.WriteMultipleCoils));
//     settings = String(settings + String(ThisProduct.PmmModbus.WriteMultipleRegisters));
//     settings = String(settings + String(ThisProduct.PmmModbus.CoilsStatus));
//     settings = String(settings + String(ThisProduct.PmmModbus.InputStatus));
//     settings = String(settings + String(ThisProduct.PmmModbus.HoldingRegisters));
//     settings = String(settings + String(ThisProduct.PmmModbus.InputRegisters));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressCoilsStatus));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressInputStatus));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressHoldingRegisters));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressInputRegisters));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressWriteSingleCoil));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressWriteSingleRegister));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressWriteMultipleCoils));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddressWriteMultipleRegisters));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.QuantityCoilsStatus));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.QuantityInputStatus));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.QuantityHoldingRegisters));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.QuantityInputRegisters));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.QuantityWriteMultipleCoils));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.QuantityWriteMultipleRegisters));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.FunctionCode));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.IODataOrder));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.IODataType));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.PollInterval));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.SlaveID));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StartingAddress));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.Quantity));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.DataBitConfig));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.ParityConfig));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.StopBitConfig));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.BaudRate));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.TXPin));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.RXPin));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.SerialSelectionPin));
//     settings = String(settings + ",");
//     settings = String(settings + String(ThisProduct.PmmModbus.SerialPort));
//     settings = String(settings + ",End");

//     // SerialUSB.println(settings);

//     return settings;
// }

String PmmReadTimersSettingsInternalFlash()
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

    // SerialUSB.println(settings);

    return settings;
}

void PmmReadAllSettingsInternalFlash()
{
    PmmReadGeneralSettingsInternalFlash();
    PmmReadRTUSettingsInternalFlash();
    PmmReadTCPUDPSettingsInternalFlash();
    //PmmReadModbusSettingsInternalFlash();
    PmmReadTimersSettingsInternalFlash();
}

/*****************************************************************
 * External flash section
 ******************************************************************/
byte PIN_FLASH_CS = 8;
PMM_SPI_FLASH myFlash;

/*****************************************************************
 * External EEPROM flash section
 ******************************************************************/

void PmmWriteGeneralSettingsEEPROM(string Message)
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
    ThisProduct.PmmGeneral.ControlerType = stoi(values[10]);

    PmmConvertDecimalToBinary(stoi(values[11]));

    ThisProduct.PmmGeneral.ItHasEthernet = binaryInt[15];
    ThisProduct.PmmGeneral.ItHasSwitch = binaryInt[14];
    ThisProduct.PmmGeneral.ItHasExtEEPROM = binaryInt[13];
    ThisProduct.PmmGeneral.ItHasExtFlash = binaryInt[12];
    ThisProduct.PmmGeneral.ItHasSerial = binaryInt[11];
    ThisProduct.PmmGeneral.ItHasWebServer = binaryInt[10];
    ThisProduct.PmmGeneral.ItHasFiber = binaryInt[9];
    ThisProduct.PmmGeneral.LoRA = binaryInt[8];

    ThisProduct.PmmGeneral.Zidbee = binaryInt[7];
    ThisProduct.PmmGeneral.GSM = binaryInt[6];
    ThisProduct.PmmGeneral.GPS = binaryInt[5];
    ThisProduct.PmmGeneral.Antenna = binaryInt[4];
    ThisProduct.PmmGeneral.ExternalRTC = binaryInt[3];
    ThisProduct.PmmGeneral.InternalRTC = binaryInt[2];
    ThisProduct.PmmGeneral.UDPOption = binaryInt[1];
    ThisProduct.PmmGeneral.GateWay = binaryInt[0];

    PmmConvertDecimalToBinary(stoi(values[12]));

    ThisProduct.PmmGeneral.IsModBus = binaryInt[8];
    ThisProduct.PmmGeneral.IsCanBus = binaryInt[7];
    ThisProduct.PmmGeneral.IsProfiBus = binaryInt[6];
    ThisProduct.PmmGeneral.IsProfiNet = binaryInt[5];
    ThisProduct.PmmGeneral.IsBACnet = binaryInt[4];
    ThisProduct.PmmGeneral.IsDLMS = binaryInt[3];
    ThisProduct.PmmGeneral.IsMBus = binaryInt[2];
    ThisProduct.PmmGeneral.IsOPC = binaryInt[1];
    ThisProduct.PmmGeneral.IsOPC = binaryInt[0];

    ThisProduct.PmmGeneral.NumberOfInputs = stoi(values[13]);
    ThisProduct.PmmGeneral.NumberOfOutputs = stoi(values[14]);
    ThisProduct.PmmGeneral.NumberOfSerials = stoi(values[15]);
    ThisProduct.PmmGeneral.NumberOfUDPPorts = stoi(values[16]);
    ThisProduct.PmmGeneral.MinReadValue = stoi(values[17]);
    ThisProduct.PmmGeneral.MaxReadValue = stoi(values[18]);
    ThisProduct.PmmGeneral.OprationVoltage = stoi(values[19]);
    ThisProduct.PmmGeneral.GeneralReadingsOffset = stoi(values[20]);
    ThisProduct.PmmGeneral.GeneralReadingsFactor = stoi(values[21]);
    ThisProduct.PmmGeneral.SettingsRef = stoi(values[22]);

    for (int index = 0; index < 23; index++)
    {
        PutIntDataToEEprom(index, stoi(values[index]));
    }
}

void PmmWriteRTUSettingsEEPROM(string Message)
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

    for (int index = 0; index < 32; index++)
    {
        PutIntDataToEEprom((index + 100), stoi(values[index]));
    }
}

void PmmWriteTCPUDPSettingsEEPROM(string Message)
{
    PmmStringToArray(Message);

    // TCP Settings
    ThisProduct.PmmTCPUDP.MacAddress01 = stoi(values[0]);
    ThisProduct.PmmTCPUDP.MacAddress02 = stoi(values[1]);
    ThisProduct.PmmTCPUDP.MacAddress03 = stoi(values[2]);
    ThisProduct.PmmTCPUDP.MacAddress04 = stoi(values[3]);
    ThisProduct.PmmTCPUDP.MacAddress05 = stoi(values[4]);
    ThisProduct.PmmTCPUDP.MacAddress06 = stoi(values[5]);

    ThisProduct.PmmTCPUDP.IPAddress01 = stoi(values[6]);
    ThisProduct.PmmTCPUDP.IPAddress02 = stoi(values[7]);
    ThisProduct.PmmTCPUDP.IPAddress03 = stoi(values[8]);
    ThisProduct.PmmTCPUDP.IPAddress04 = stoi(values[9]);

    ThisProduct.PmmTCPUDP.RemoteIPAddress01 = stoi(values[10]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress02 = stoi(values[11]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress03 = stoi(values[12]);
    ThisProduct.PmmTCPUDP.RemoteIPAddress04 = stoi(values[13]);

    ThisProduct.PmmTCPUDP.NetMask01 = stoi(values[14]);
    ThisProduct.PmmTCPUDP.NetMask02 = stoi(values[15]);
    ThisProduct.PmmTCPUDP.NetMask03 = stoi(values[16]);
    ThisProduct.PmmTCPUDP.NetMask04 = stoi(values[17]);

    ThisProduct.PmmTCPUDP.DNSSOneServer01 = stoi(values[18]);
    ThisProduct.PmmTCPUDP.DNSSOneServer02 = stoi(values[19]);
    ThisProduct.PmmTCPUDP.DNSSOneServer03 = stoi(values[20]);
    ThisProduct.PmmTCPUDP.DNSSOneServer04 = stoi(values[21]);

    ThisProduct.PmmTCPUDP.DNSTwoServer01 = stoi(values[22]);
    ThisProduct.PmmTCPUDP.DNSTwoServer02 = stoi(values[23]);
    ThisProduct.PmmTCPUDP.DNSTwoServer03 = stoi(values[24]);
    ThisProduct.PmmTCPUDP.DNSTwoServer04 = stoi(values[25]);

    ThisProduct.PmmTCPUDP.ConnectionTimeoutTCP = stoi(values[26]);
    ThisProduct.PmmTCPUDP.MaxRetryTCP = stoi(values[27]);
    ThisProduct.PmmTCPUDP.UDPPortOne = stoi(values[28]);
    ThisProduct.PmmTCPUDP.UDPPortTwo = stoi(values[29]);
    ThisProduct.PmmTCPUDP.UDPPortThree = stoi(values[30]);
    ThisProduct.PmmTCPUDP.UDPPortFour = stoi(values[31]);

    for (int index = 0; index < 32; index++)
    {
        PutIntDataToEEprom((index + 200), stoi(values[index]));
    }
}

// void PmmWriteModbusSettingsEEPROM(string Message)
// {
//     PmmStringToArray(Message);

//     PmmConvertDecimalToBinary(stoi(values[0]));

//     ThisProduct.PmmModbus.ModBusRTU = binaryInt[15];
//     ThisProduct.PmmModbus.ModBusTCP = binaryInt[14];
//     ThisProduct.PmmModbus.ModBusSlave = binaryInt[13];
//     ThisProduct.PmmModbus.ModBusMaster = binaryInt[12];
//     ThisProduct.PmmModbus.ReadCoilsStatus = binaryInt[11];
//     ThisProduct.PmmModbus.ReadInputStatus = binaryInt[10];
//     ThisProduct.PmmModbus.ReadHoldingRegisters = binaryInt[9];
//     ThisProduct.PmmModbus.ReadInputRegisters = binaryInt[8];
//     ThisProduct.PmmModbus.WriteSingleCoil = binaryInt[7];
//     ThisProduct.PmmModbus.WriteSingleRegister = binaryInt[6];
//     ThisProduct.PmmModbus.WriteMultipleCoils = binaryInt[5];
//     ThisProduct.PmmModbus.WriteMultipleRegisters = binaryInt[4];

//     ThisProduct.PmmModbus.CoilsStatus = binaryInt[3];
//     ThisProduct.PmmModbus.InputStatus = binaryInt[2];
//     ThisProduct.PmmModbus.HoldingRegisters = binaryInt[1];
//     ThisProduct.PmmModbus.InputRegisters = binaryInt[0];

//     ThisProduct.PmmModbus.StartingAddressCoilsStatus = stoi(values[1]);
//     ThisProduct.PmmModbus.StartingAddressInputStatus = stoi(values[2]);
//     ThisProduct.PmmModbus.StartingAddressHoldingRegisters = stoi(values[3]);
//     ThisProduct.PmmModbus.StartingAddressInputRegisters = stoi(values[4]);
//     ThisProduct.PmmModbus.StartingAddressWriteSingleCoil = stoi(values[5]);
//     ThisProduct.PmmModbus.StartingAddressWriteSingleRegister = stoi(values[6]);
//     ThisProduct.PmmModbus.StartingAddressWriteMultipleCoils = stoi(values[7]);
//     ThisProduct.PmmModbus.StartingAddressWriteMultipleRegisters = stoi(values[8]);
//     ThisProduct.PmmModbus.QuantityCoilsStatus = stoi(values[9]);
//     ThisProduct.PmmModbus.QuantityInputStatus = stoi(values[10]);
//     ThisProduct.PmmModbus.QuantityHoldingRegisters = stoi(values[11]);
//     ThisProduct.PmmModbus.QuantityInputRegisters = stoi(values[12]);
//     ThisProduct.PmmModbus.QuantityWriteMultipleCoils = stoi(values[13]);
//     ThisProduct.PmmModbus.QuantityWriteMultipleRegisters = stoi(values[14]);
//     ThisProduct.PmmModbus.FunctionCode = stoi(values[15]);
//     ThisProduct.PmmModbus.IODataOrder = stoi(values[16]);
//     ThisProduct.PmmModbus.IODataType = stoi(values[17]);
//     ThisProduct.PmmModbus.PollInterval = stoi(values[18]);
//     ThisProduct.PmmModbus.SlaveID = stoi(values[19]);
//     ThisProduct.PmmModbus.StartingAddress = stoi(values[20]);
//     ThisProduct.PmmModbus.Quantity = stoi(values[21]);

//     ThisProduct.PmmModbus.DataBitConfig = stoi(values[22]);
//     ThisProduct.PmmModbus.ParityConfig = stoi(values[23]); // 1=>None,2=>Even,3=>Odd,4=>Mark,5=>Space
//     ThisProduct.PmmModbus.StopBitConfig = stoi(values[24]);
//     ThisProduct.PmmModbus.BaudRate = stoi(values[25]);
//     ThisProduct.PmmModbus.TXPin = stoi(values[26]);
//     ThisProduct.PmmModbus.RXPin = stoi(values[27]);
//     ThisProduct.PmmModbus.SerialSelectionPin = stoi(values[28]);
//     ThisProduct.PmmModbus.SerialPort = stoi(values[29]);

//     for (int index = 0; index < 30; index++)
//     {
//         PutIntDataToEEprom((index + 300), stoi(values[index]));
//     }
// }

void PmmWriteTimerSettingsEEPROM(string Message)
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

    PutIntDataToEEprom(500, stoi(values[0]));
}

String PmmReadGeneralSettingsEEPROM()
{
    String settings = "";

    for (int index = 0; index < 26; index++)
    {
        if (index == 10)
        {
            PmmConvertDecimalToBinary(GetIntDataFromEEprom(10));
            for (int i = 7; i >= 0; i--)
            {
                settings = String(settings + String(binaryInt[i]));
            }
            settings = String(settings + ",");
        }
        else if (index == 11)
        {
            PmmConvertDecimalToBinary(GetIntDataFromEEprom(11));
            for (int i = 7; i >= 0; i--)
            {
                settings = String(settings + String(binaryInt[i]));
            }
            settings = String(settings + ",");
        }
        else if (index == 12)
        {
            PmmConvertDecimalToBinary(GetIntDataFromEEprom(12));
            for (int i = 8; i >= 0; i--)
            {
                settings = String(settings + String(binaryInt[i]));
            }
            settings = String(settings + ",");
        }
        else
        {
            settings = String(settings + String(GetIntDataFromEEprom(index)));
            settings = String(settings + ",");
        }
    }

    settings = String(settings + String("End"));

    // SerialUSB.println(settings);

    ThisProduct.PmmGeneral.DeviceName = GetIntDataFromEEprom(0);
    ThisProduct.PmmGeneral.SerialNumber = GetIntDataFromEEprom(1);
    ThisProduct.PmmGeneral.FirstTimeRun = GetIntDataFromEEprom(2);
    ThisProduct.PmmGeneral.LifeTime = GetIntDataFromEEprom(3);
    ThisProduct.PmmGeneral.NumberOfCycles = GetIntDataFromEEprom(4);
    ThisProduct.PmmGeneral.LastRunningTime = GetIntDataFromEEprom(5);
    ThisProduct.PmmGeneral.NumberOfRunningTimes = GetIntDataFromEEprom(6);
    ThisProduct.PmmGeneral.SoftwareVersion = GetIntDataFromEEprom(7);
    ThisProduct.PmmGeneral.FirmwareVersion = GetIntDataFromEEprom(8);
    ThisProduct.PmmGeneral.HardwareVersion = GetIntDataFromEEprom(9);
    ThisProduct.PmmGeneral.ControlerType = GetIntDataFromEEprom(10);

    PmmConvertDecimalToBinary(GetIntDataFromEEprom(11));

    ThisProduct.PmmGeneral.ItHasEthernet = binaryInt[15];
    ThisProduct.PmmGeneral.ItHasSwitch = binaryInt[14];
    ThisProduct.PmmGeneral.ItHasExtEEPROM = binaryInt[13];
    ThisProduct.PmmGeneral.ItHasExtFlash = binaryInt[12];
    ThisProduct.PmmGeneral.ItHasSerial = binaryInt[11];
    ThisProduct.PmmGeneral.ItHasWebServer = binaryInt[10];
    ThisProduct.PmmGeneral.ItHasFiber = binaryInt[9];
    ThisProduct.PmmGeneral.LoRA = binaryInt[8];

    ThisProduct.PmmGeneral.Zidbee = binaryInt[7];
    ThisProduct.PmmGeneral.GSM = binaryInt[6];
    ThisProduct.PmmGeneral.GPS = binaryInt[5];
    ThisProduct.PmmGeneral.Antenna = binaryInt[4];
    ThisProduct.PmmGeneral.ExternalRTC = binaryInt[3];
    ThisProduct.PmmGeneral.InternalRTC = binaryInt[2];
    ThisProduct.PmmGeneral.UDPOption = binaryInt[1];
    ThisProduct.PmmGeneral.GateWay = binaryInt[0];

    PmmConvertDecimalToBinary(GetIntDataFromEEprom(12));

    ThisProduct.PmmGeneral.IsModBus = binaryInt[8];
    ThisProduct.PmmGeneral.IsCanBus = binaryInt[7];
    ThisProduct.PmmGeneral.IsProfiBus = binaryInt[6];
    ThisProduct.PmmGeneral.IsProfiNet = binaryInt[5];
    ThisProduct.PmmGeneral.IsBACnet = binaryInt[4];
    ThisProduct.PmmGeneral.IsDLMS = binaryInt[3];
    ThisProduct.PmmGeneral.IsMBus = binaryInt[2];
    ThisProduct.PmmGeneral.IsOPC = binaryInt[1];
    ThisProduct.PmmGeneral.Canprint = binaryInt[0];

    ThisProduct.PmmGeneral.NumberOfInputs = GetIntDataFromEEprom(13);
    ThisProduct.PmmGeneral.NumberOfOutputs = GetIntDataFromEEprom(14);
    ThisProduct.PmmGeneral.NumberOfSerials = GetIntDataFromEEprom(15);
    ThisProduct.PmmGeneral.NumberOfUDPPorts = GetIntDataFromEEprom(16);
    ThisProduct.PmmGeneral.MinReadValue = GetIntDataFromEEprom(17);
    ThisProduct.PmmGeneral.MaxReadValue = GetIntDataFromEEprom(18);
    ThisProduct.PmmGeneral.OprationVoltage = GetIntDataFromEEprom(19);
    ThisProduct.PmmGeneral.GeneralReadingsOffset = GetIntDataFromEEprom(20);
    ThisProduct.PmmGeneral.GeneralReadingsFactor = GetIntDataFromEEprom(21);
    ThisProduct.PmmGeneral.SettingsRef = GetIntDataFromEEprom(22);

    return settings;
}

String PmmReadRTUSettingsEEPROM()
{
    String settings = "";

    for (int index = 0; index < 32; index++)
    {
        settings = String(settings + String(GetIntDataFromEEprom((index + 100))));
        settings = String(settings + ",");
    }

    settings = String(settings + String("End"));

    // SerialUSB.println(settings);

    ThisProduct.PmmRTU.PortOneName = GetIntDataFromEEprom(100);
    ThisProduct.PmmRTU.PortOneBaudRate = GetIntDataFromEEprom(101);
    ThisProduct.PmmRTU.PortOneStopBit = GetIntDataFromEEprom(102);
    ThisProduct.PmmRTU.PortOneDataBit = GetIntDataFromEEprom(103);
    ThisProduct.PmmRTU.PortOneParity = GetIntDataFromEEprom(104);
    ThisProduct.PmmRTU.PortOneConnectionTimeout = GetIntDataFromEEprom(105);
    ThisProduct.PmmRTU.PortOneMaxRetryRTU = GetIntDataFromEEprom(106);
    ThisProduct.PmmRTU.PortOneInterface = GetIntDataFromEEprom(107);
    ThisProduct.PmmRTU.PortTwoName = GetIntDataFromEEprom(108);
    ThisProduct.PmmRTU.PortTwoBaudRate = GetIntDataFromEEprom(109);
    ThisProduct.PmmRTU.PortTwoStopBit = GetIntDataFromEEprom(110);
    ThisProduct.PmmRTU.PortTwoDataBit = GetIntDataFromEEprom(111);
    ThisProduct.PmmRTU.PortTwoParity = GetIntDataFromEEprom(112);
    ThisProduct.PmmRTU.PortTwoConnectionTimeout = GetIntDataFromEEprom(113);
    ThisProduct.PmmRTU.PortTwoMaxRetryRTU = GetIntDataFromEEprom(114);
    ThisProduct.PmmRTU.PortTwoInterface = GetIntDataFromEEprom(115);
    ThisProduct.PmmRTU.PortThreeName = GetIntDataFromEEprom(116);
    ThisProduct.PmmRTU.PortThreeBaudRate = GetIntDataFromEEprom(117);
    ThisProduct.PmmRTU.PortThreeStopBit = GetIntDataFromEEprom(118);
    ThisProduct.PmmRTU.PortThreeDataBit = GetIntDataFromEEprom(119);
    ThisProduct.PmmRTU.PortThreeParity = GetIntDataFromEEprom(120);
    ThisProduct.PmmRTU.PortThreeConnectionTimeout = GetIntDataFromEEprom(121);
    ThisProduct.PmmRTU.PortThreeMaxRetryRTU = GetIntDataFromEEprom(122);
    ThisProduct.PmmRTU.PortThreeInterface = GetIntDataFromEEprom(123);
    ThisProduct.PmmRTU.PortFourName = GetIntDataFromEEprom(124);
    ThisProduct.PmmRTU.PortFourBaudRate = GetIntDataFromEEprom(125);
    ThisProduct.PmmRTU.PortFourStopBit = GetIntDataFromEEprom(126);
    ThisProduct.PmmRTU.PortFourDataBit = GetIntDataFromEEprom(127);
    ThisProduct.PmmRTU.PortFourParity = GetIntDataFromEEprom(128);
    ThisProduct.PmmRTU.PortFourConnectionTimeout = GetIntDataFromEEprom(129);
    ThisProduct.PmmRTU.PortFourMaxRetryRTU = GetIntDataFromEEprom(130);
    ThisProduct.PmmRTU.PortFourInterface = GetIntDataFromEEprom(131);

    return settings;
}

String PmmReadTCPUDPSettingsEEPROM()
{

    String settings = "";

    for (int index = 0; index < 32; index++)
    {
        settings = String(settings + String(GetIntDataFromEEprom((index + 200))));
        settings = String(settings + ",");
    }

    settings = String(settings + String("End"));

    // SerialUSB.println(settings);

    ThisProduct.PmmTCPUDP.MacAddress01 = GetIntDataFromEEprom(200);
    ThisProduct.PmmTCPUDP.MacAddress02 = GetIntDataFromEEprom(201);
    ThisProduct.PmmTCPUDP.MacAddress03 = GetIntDataFromEEprom(202);
    ThisProduct.PmmTCPUDP.MacAddress04 = GetIntDataFromEEprom(203);
    ThisProduct.PmmTCPUDP.MacAddress05 = GetIntDataFromEEprom(204);
    ThisProduct.PmmTCPUDP.MacAddress06 = GetIntDataFromEEprom(205);

    ThisProduct.PmmTCPUDP.IPAddress01 = GetIntDataFromEEprom(206);
    ThisProduct.PmmTCPUDP.IPAddress02 = GetIntDataFromEEprom(207);
    ThisProduct.PmmTCPUDP.IPAddress03 = GetIntDataFromEEprom(208);
    ThisProduct.PmmTCPUDP.IPAddress04 = GetIntDataFromEEprom(209);

    ThisProduct.PmmTCPUDP.RemoteIPAddress01 = GetIntDataFromEEprom(210);
    ThisProduct.PmmTCPUDP.RemoteIPAddress02 = GetIntDataFromEEprom(211);
    ThisProduct.PmmTCPUDP.RemoteIPAddress03 = GetIntDataFromEEprom(212);
    ThisProduct.PmmTCPUDP.RemoteIPAddress04 = GetIntDataFromEEprom(213);

    ThisProduct.PmmTCPUDP.NetMask01 = GetIntDataFromEEprom(214);
    ThisProduct.PmmTCPUDP.NetMask02 = GetIntDataFromEEprom(215);
    ThisProduct.PmmTCPUDP.NetMask03 = GetIntDataFromEEprom(216);
    ThisProduct.PmmTCPUDP.NetMask04 = GetIntDataFromEEprom(217);

    ThisProduct.PmmTCPUDP.DNSSOneServer01 = GetIntDataFromEEprom(218);
    ThisProduct.PmmTCPUDP.DNSSOneServer02 = GetIntDataFromEEprom(219);
    ThisProduct.PmmTCPUDP.DNSSOneServer03 = GetIntDataFromEEprom(220);
    ThisProduct.PmmTCPUDP.DNSSOneServer04 = GetIntDataFromEEprom(221);

    ThisProduct.PmmTCPUDP.DNSTwoServer01 = GetIntDataFromEEprom(222);
    ThisProduct.PmmTCPUDP.DNSTwoServer02 = GetIntDataFromEEprom(223);
    ThisProduct.PmmTCPUDP.DNSTwoServer03 = GetIntDataFromEEprom(224);
    ThisProduct.PmmTCPUDP.DNSTwoServer04 = GetIntDataFromEEprom(225);

    ThisProduct.PmmTCPUDP.ConnectionTimeoutTCP = GetIntDataFromEEprom(226);
    ThisProduct.PmmTCPUDP.MaxRetryTCP = GetIntDataFromEEprom(227);
    ThisProduct.PmmTCPUDP.UDPPortOne = GetIntDataFromEEprom(228);
    ThisProduct.PmmTCPUDP.UDPPortTwo = GetIntDataFromEEprom(229);
    ThisProduct.PmmTCPUDP.UDPPortThree = GetIntDataFromEEprom(230);
    ThisProduct.PmmTCPUDP.UDPPortFour = GetIntDataFromEEprom(231);

    return settings;
}

// String PmmReadModbusSettingsEEPROM()
// {
//     String settings = "";

//     PmmConvertDecimalToBinary(GetIntDataFromEEprom(300));
//     for (int i = 15; i >= 0; i--)
//     {
//         settings = String(settings + String(binaryInt[i]));
//     }
//     settings = String(settings + ",");

//     for (int index = 1; index < 30; index++)
//     {

//         settings = String(settings + String(GetIntDataFromEEprom((index + 300))));
//         settings = String(settings + ",");
//     }

//     settings = String(settings + String("End"));

//     // SerialUSB.println(settings);

//     PmmConvertDecimalToBinary(GetIntDataFromEEprom(300));

//     ThisProduct.PmmModbus.ModBusRTU = binaryInt[15];
//     ThisProduct.PmmModbus.ModBusTCP = binaryInt[14];
//     ThisProduct.PmmModbus.ModBusSlave = binaryInt[13];
//     ThisProduct.PmmModbus.ModBusMaster = binaryInt[12];
//     ThisProduct.PmmModbus.ReadCoilsStatus = binaryInt[11];
//     ThisProduct.PmmModbus.ReadInputStatus = binaryInt[10];
//     ThisProduct.PmmModbus.ReadHoldingRegisters = binaryInt[9];
//     ThisProduct.PmmModbus.ReadInputRegisters = binaryInt[8];
//     ThisProduct.PmmModbus.WriteSingleCoil = binaryInt[7];
//     ThisProduct.PmmModbus.WriteSingleRegister = binaryInt[6];
//     ThisProduct.PmmModbus.WriteMultipleCoils = binaryInt[5];
//     ThisProduct.PmmModbus.WriteMultipleRegisters = binaryInt[4];

//     ThisProduct.PmmModbus.CoilsStatus = binaryInt[3];
//     ThisProduct.PmmModbus.InputStatus = binaryInt[2];
//     ThisProduct.PmmModbus.HoldingRegisters = binaryInt[1];
//     ThisProduct.PmmModbus.InputRegisters = binaryInt[0];

//     ThisProduct.PmmModbus.StartingAddressCoilsStatus = GetIntDataFromEEprom(301);
//     ThisProduct.PmmModbus.StartingAddressInputStatus = GetIntDataFromEEprom(302);
//     ThisProduct.PmmModbus.StartingAddressHoldingRegisters = GetIntDataFromEEprom(303);
//     ThisProduct.PmmModbus.StartingAddressInputRegisters = GetIntDataFromEEprom(304);
//     ThisProduct.PmmModbus.StartingAddressWriteSingleCoil = GetIntDataFromEEprom(305);
//     ThisProduct.PmmModbus.StartingAddressWriteSingleRegister = GetIntDataFromEEprom(306);
//     ThisProduct.PmmModbus.StartingAddressWriteMultipleCoils = GetIntDataFromEEprom(307);
//     ThisProduct.PmmModbus.StartingAddressWriteMultipleRegisters = GetIntDataFromEEprom(308);
//     ThisProduct.PmmModbus.QuantityCoilsStatus = GetIntDataFromEEprom(309);
//     ThisProduct.PmmModbus.QuantityInputStatus = GetIntDataFromEEprom(310);
//     ThisProduct.PmmModbus.QuantityHoldingRegisters = GetIntDataFromEEprom(311);
//     ThisProduct.PmmModbus.QuantityInputRegisters = GetIntDataFromEEprom(312);
//     ThisProduct.PmmModbus.QuantityWriteMultipleCoils = GetIntDataFromEEprom(313);
//     ThisProduct.PmmModbus.QuantityWriteMultipleRegisters = GetIntDataFromEEprom(314);
//     ThisProduct.PmmModbus.FunctionCode = GetIntDataFromEEprom(315);
//     ThisProduct.PmmModbus.IODataOrder = GetIntDataFromEEprom(316);
//     ThisProduct.PmmModbus.IODataType = GetIntDataFromEEprom(317);
//     ThisProduct.PmmModbus.PollInterval = GetIntDataFromEEprom(318);
//     ThisProduct.PmmModbus.SlaveID = GetIntDataFromEEprom(319);
//     ThisProduct.PmmModbus.StartingAddress = GetIntDataFromEEprom(320);
//     ThisProduct.PmmModbus.Quantity = GetIntDataFromEEprom(321);

//     ThisProduct.PmmModbus.DataBitConfig = GetIntDataFromEEprom(322);
//     ThisProduct.PmmModbus.ParityConfig = GetIntDataFromEEprom(323); // 1=>None,2=>Even,3=>Odd,4=>Mark,5=>Space
//     ThisProduct.PmmModbus.StopBitConfig = GetIntDataFromEEprom(324);
//     ThisProduct.PmmModbus.BaudRate = GetIntDataFromEEprom(325);
//     ThisProduct.PmmModbus.TXPin = GetIntDataFromEEprom(326);
//     ThisProduct.PmmModbus.RXPin = GetIntDataFromEEprom(327);
//     ThisProduct.PmmModbus.SerialSelectionPin = GetIntDataFromEEprom(328);
//     ThisProduct.PmmModbus.SerialPort = GetIntDataFromEEprom(329);

//     return settings;
// }

String PmmReadTimersSettingsEEPROM()
{
    String settings = "";

    PmmConvertDecimalToBinary(GetIntDataFromEEprom(500));
    for (int i = 7; i >= 0; i--)
    {
        settings = String(settings + String(binaryInt[i]));
    }
    settings = String(settings + ",END");

    // SerialUSB.println(settings);

    PmmConvertDecimalToBinary(GetIntDataFromEEprom(500));

    ThisProduct.PmmTimers.CycleTimer = binaryInt[7];
    ThisProduct.PmmTimers.OneMSTimer = binaryInt[6];
    ThisProduct.PmmTimers.TenMSTimer = binaryInt[5];
    ThisProduct.PmmTimers.OneSecTimer = binaryInt[4];
    ThisProduct.PmmTimers.OneMinTimer = binaryInt[3];
    ThisProduct.PmmTimers.OneHouTimer = binaryInt[2];
    ThisProduct.PmmTimers.OneMonTimer = binaryInt[1];
    ThisProduct.PmmTimers.OneYearTimer = binaryInt[0];

    return settings;
}

void PmmReadAllSettingsEEPROM()
{
    PmmReadGeneralSettingsEEPROM();
    PmmReadRTUSettingsEEPROM();
    PmmReadTCPUDPSettingsEEPROM();
    //PmmReadModbusSettingsEEPROM();
    PmmReadTimersSettingsEEPROM();
}

/*****************************************************************
 * Command Reader Functions
 ******************************************************************/

String PMMReturnDataFromSerialUSB()
{
    String Command = "";
    Command = SerialUSB.readStringUntil('\n');
    return Command;
}

String PMMCommnads(string readData)
{
    String result = "";
    std::string commandtype = readData.substr(0, 10);

    //---------------- Internal Flash ----------------------------------------------------------------------------------------
    // PMMSet,0,0,0620,5000,0,1000,55,200,0,0,1,0,50115,1,2,3,4,5,6,100,200,300,400,500,600,1,644
    // PMMSet,0,1,1,9600,1,8,0,5000,3,485,2,9600,2,7,1,6000,4,322,3,9600,1,8,2,7000,5,485,4,9600,2,7,1,7000,5,322
    // PMMSet,0,2,171,205,173,205,171,205,192,186,1,100,192,168,1,200,255,255,255,255,8,8,8,8,8,8,0,0,5000,3,90,91,92,93
    // PMMSet,0,3,4384,1,1001,2001,3001,1,1001,2001,3001,32,64,128,256,100,100,03,1,1,1000,1,1,100,8,1,1,9600,35,36,1,1
    // PMMSet,0,4,195,
    // PMMGet,0,0
    // PMMGet,0,1
    // PMMGet,0,2
    // PMMGet,0,3
    // PMMGet,0,4
    //---------------- End Internal Flash -------------------------------------------------------------------------------------

    //---------------- EEPROM -------------------------------------------------------------------------------------------------
    // PMMSet,2,0,0620,5000,0,1000,55,200,0,0,1,0,50115,1,2,3,4,5,6,100,200,300,400,500,600,1,644
    // PMMSet,2,1,1,9600,1,8,0,5000,3,485,2,9600,2,7,1,6000,4,322,3,9600,1,8,2,7000,5,485,4,9600,2,7,1,7000,5,322
    // PMMSet,2,2,171,205,173,205,171,205,192,186,1,100,192,168,1,200,255,255,255,255,8,8,8,8,8,8,0,0,5000,3,90,91,92,93
    // PMMSet,2,3,4384,1,1001,2001,3001,1,1001,2001,3001,32,64,128,256,100,100,03,1,1,1000,1,1,100,8,1,1,9600,35,36,1,1
    // PMMSet,2,4,195,
    // PMMGet,2,0
    // PMMGet,2,1
    // PMMGet,2,2
    // PMMGet,2,3
    // PMMGet,2,4
    //---------------- End EEPROM ----------------------------------------------------------------------------------------------

    if (commandtype == "PMMSet,0,0")
    {
        string substring = "PMMSet,0,0,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteGeneralSettingsInternalFlash(readData);
        result = "Done";
    }

    if (commandtype == "PMMSet,0,1")
    {
        string substring = "PMMSet,0,1,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteRTUSettingsInternalFlash(readData);
        result = "Done";
    }

    if (commandtype == "PMMSet,0,2")
    {
        string substring = "PMMSet,0,2,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteTCPUDPSettingsInternalFlash(readData);
        result = "Done";
    }

    if (commandtype == "PMMSet,0,3")
    {
        string substring = "PMMSet,0,3,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        //PmmWriteModbusSettingsInternalFlash(readData);
        result = "Done";
    }

    if (commandtype == "PMMSet,0,4")
    {
        string substring = "PMMSet,0,4,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteTimerSettingsInternalFlash(readData);
        result = "Done";
    }

    if (commandtype == "PMMGet,0,0") // GET General TO STRING
    {
        result = PmmReadGeneralSettingsInternalFlash();
    }

    if (commandtype == "PMMGet,0,1") // GET RTU TO STRING
    {
        result = PmmReadRTUSettingsInternalFlash();
    }

    if (commandtype == "PMMGet,0,2") // GET TCP TO STRING
    {
        result = PmmReadTCPUDPSettingsInternalFlash();
    }

    if (commandtype == "PMMGet,0,3") // GET Modbus TO STRING
    {
        //result = PmmReadModbusSettingsInternalFlash();
    }

    if (commandtype == "PMMGet,0,4") // GET Timers TO STRING
    {
        result = PmmReadTimersSettingsInternalFlash();
    }

    if (commandtype == "PMMSet,2,0")
    {
        string substring = "PMMSet,2,0,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteGeneralSettingsEEPROM(readData);
        result = "Done";
    }

    if (commandtype == "PMMSet,2,1")
    {
        string substring = "PMMSet,2,1,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteRTUSettingsEEPROM(readData);
        result = "Done";
    }

    if (commandtype == "PMMSet,2,2")
    {
        string substring = "PMMSet,2,2,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteTCPUDPSettingsEEPROM(readData);
        result = "Done";
    }

    if (commandtype == "PMMSet,2,3")
    {
        string substring = "PMMSet,2,3,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteTimerSettingsEEPROM(readData);
        result = "Done";
    }

    if (commandtype == "PMMSet,2,4")
    {
        string substring = "PMMSet,2,4,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteTimerSettingsEEPROM(readData);
        result = "Done";
    }

    if (commandtype == "PMMGet,2,0") // GET General TO STRING
    {
        result = PmmReadGeneralSettingsEEPROM();
    }

    if (commandtype == "PMMGet,2,1") // GET RTU TO STRING
    {
        result = PmmReadRTUSettingsEEPROM();
    }

    if (commandtype == "PMMGet,2,2") // GET TCP TO STRING
    {
        result = PmmReadTCPUDPSettingsEEPROM();
    }

    if (commandtype == "PMMGet,2,3") // GET Modbus TO STRING
    {
        //result = PmmReadModbusSettingsEEPROM();
    }

    if (commandtype == "PMMGet,2,4") // GET Timers TO STRING
    {
        result = PmmReadTimersSettingsEEPROM();
    }

    if (readData == "PMMTestConfiguration")
    {
        result = PMMIsAlive();
    }

    // Actions
    // Reset MCU =>NVIC_SystemReset();
    if (readData == "PMMResetMCU1948")
    {
        NVIC_SystemReset();
        result = "Reset..";
    }

    // Set and get clock from PC
    //  SETRTC,0,1,23,4,20,0,15,00,00,
    if (commandtype == "SETRTC,0,1") // internal RTC
    {
        string substring = "SETRTC,0,1,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        result = SetInternalRTC(readData);
        // result = "Done";
    }

    // SETRTC,1,1,23,4,20,10,19,00,00,
    if (commandtype == "SETRTC,1,1") // External RTC
    {
        string substring = "SETRTC,1,1,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        result = SetExternalRTC(readData);
        // result = "Done";
    }

    if (readData == "PMMGetInternalRTC")
    {
        result = GetInternalRTC();
    }

    if (readData == "PMMGetExternalRTC")
    {
        result = GetExternalRTC();
    }

    return result;
}

String SetInternalRTC(string Message)
{
    PmmStringToArray(Message);

    PmmRTCInternal.setDate(lowByte(stoi(values[2])), lowByte(stoi(values[1])), lowByte(stoi(values[0])));
    PmmRTCInternal.setTime(lowByte(stoi(values[3])), lowByte(stoi(values[4])), lowByte(stoi(values[5])));

    // PmmRTCInternal.setDate(20,4,23);
    // PmmRTCInternal.setTime(13,15,0);

    return "Internal RTC Updated";
}

String GetInternalRTC()
{

    String result = "";

    result = String(PmmRTCInternal.getYear());
    result = result + ",";
    result = result + String(PmmRTCInternal.getMonth());
    result = result + ",";
    result = result + String(PmmRTCInternal.getDay());
    result = result + ",";
    result = result + String(PmmRTCInternal.getHours());
    result = result + ",";
    result = result + String(PmmRTCInternal.getMinutes());
    result = result + ",";
    result = result + String(PmmRTCInternal.getSeconds());
    result = result + ",";
    
    return result;
}

// External RTC DS3231
String SetExternalRTC(string Message)
{
    PmmStringToArray(Message);

    PmmRTCExternal.setYear(lowByte(stoi(values[0])));
    PmmRTCExternal.setMonth(lowByte(stoi(values[1])));
    PmmRTCExternal.setDate(lowByte(stoi(values[2])));

    PmmRTCExternal.setClockMode(false); // 24h mode
    PmmRTCExternal.setHour(lowByte(stoi(values[3])));
    PmmRTCExternal.setMinute(lowByte(stoi(values[4])));
    PmmRTCExternal.setSecond(lowByte(stoi(values[5])));

    return "External RTC Updated";
}

// External RTC DS3231
String GetExternalRTC()
{

    DateTime now = PmmRTCExternal.now();
    String result = "";

    result = String(now.year());
    result = result + ",";
    result = result + String(now.month());
    result = result + ",";
    result = result + String(now.day());
    result = result + ",";
    result = result + String(now.hour());
    result = result + ",";
    result = result + String(now.minute());
    result = result + ",";
    result = result + String(now.second());
    result = result + ",";
    
    return result;
}

void PMMReadCommands()
{
    String result = "";
    if (SerialUSB.available())
    {
        string cmd = PMMReturnDataFromSerialUSB().c_str();

        if (!cmd.empty())
        {
            result = PMMCommnads(cmd.c_str());
            SerialUSB.println(result);
        }
    }

    if (client)
    {
        result = PMMCommnads((PMMReturnDataFromAPIHTTPHeader()).c_str());
        PMMSendDataHTTPClient(result);
    }
}

String PMMReturnDataFromAPIHTTPHeader()
{
    String APIData = "";
    String readString = " ";

    while (client.connected())
    {
        if (client.available())
        {
            char c = client.read();

            if (readString.length() < 120)
            {
                readString += c;
            }
            if (c == '\n')
            {
                APIData = readString.substring(readString.indexOf("api?message=") + 12, readString.indexOf("HTTP/1.1"));
                client.stop();
            }
        }
    }

    return APIData;
}

void PMMSendDataHTTPClient(String Data)
{

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");

    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.print(Data);
    client.println("</head>");
    client.println("</html>");
    client.stop();
}
