
#include <Arduino.h>
#include <string>

#include <PmmTypes.h>
#include <PmmEthernet.h>
#include <PmmFlashStorage.h>
#include <PmmSPISerialFlash.h>

#include <PmmInternalRTC.h>
#include <PmmDS3231.h>

#include "PmmCommands.h"

/*
 // Common functions
void PmmStringToArray(string input);
void PMMIsAlive();
void PmmConvertDecimalToBinary(int Dic);
//Internal flash ROM functions
void PmmWriteGeneralSettings(string Message);
void PmmWriteRTUSettings(string Message);
void PmmWriteTCPUDPSettings(string Message);
void PmmWriteModbusSettings(string Message);
void PmmWriteTimerSettings(string Message);
void PmmReadGeneralSettings();
void PmmReadRTUSettings();
void PmmReadTCPUDPSettings();
void PmmReadModbusSettings();
void PmmReadTimersSettings();


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
byte ByteArray[4];
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

void LongToBytes(long LongVaue)
{  
ByteArray[0] = LongVaue & 0xFF; // 0x78
ByteArray[1] = (LongVaue >> 8) & 0xFF ;// 0x56
ByteArray[2] = (LongVaue >> 16) & 0xFF ;// 0x34
ByteArray[3] = (LongVaue >> 24) & 0xFF ;// 0x12
}


void LongToBytes(unsigned long uLongVaue)
{
ByteArray[0] = uLongVaue & 0xFF; // 0x78
ByteArray[1] = (uLongVaue >> 8) & 0xFF ; // 0x56
ByteArray[2] = (uLongVaue >> 16) & 0xFF ;// 0x34
ByteArray[3] = (uLongVaue >> 24) & 0xFF ;// 0x12
}
/*****************************************************************
 * Internal flash section
 ******************************************************************/

// storage area definitions each should be max 256 bytes
FlashStorage(Product_flash_store, Product);
FlashStorage(General_flash_store, PMMGeneral);

FlashStorage(TCPUDP_flash_store, PMMTCPUDP);
FlashStorage(TCPUDP_Protocol_store, PMMProtocol);

FlashStorage(Serial_flash_store01, PMMSERIAL);
FlashStorage(Serial_Protoco_store01, PMMProtocol);

FlashStorage(Serial_flash_store02, PMMSERIAL);
FlashStorage(Serial_Protoco_store02, PMMProtocol);

FlashStorage(Serial_flash_store03, PMMSERIAL);
FlashStorage(Serial_Protoco_store03, PMMProtocol);

FlashStorage(Serial_flash_store04, PMMSERIAL);
FlashStorage(Serial_Protoco_store04, PMMProtocol);

FlashStorage(Timers_flash_store, PMMTimer);

FlashStorage(Claibration_flash_Page01, PMMDeviceCalibration);
FlashStorage(Claibration_flash_Page02, PMMDeviceCalibration);

FlashStorage(GerneralPurpose_flash_store, GerneralPurpose);

/*
          Message : comma sperated string with max 32 elements
                i : Serial port number
uint8_t RomTarget : 0 : internal / 1 : ext eeprom / 2 : ext flash
*/
void PmmWriteGeneralSettings(string Message, int RomTarget)
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

    // ThisProduct.PmmGeneral.IsModBus = binaryInt[8];
    // ThisProduct.PmmGeneral.IsCanBus = binaryInt[7];
    // ThisProduct.PmmGeneral.IsProfiBus = binaryInt[6];
    // ThisProduct.PmmGeneral.IsProfiNet = binaryInt[5];
    // ThisProduct.PmmGeneral.IsBACnet = binaryInt[4];
    // ThisProduct.PmmGeneral.IsDLMS = binaryInt[3];
    // ThisProduct.PmmGeneral.IsMBus = binaryInt[2];
    // ThisProduct.PmmGeneral.IsOPC = binaryInt[1];
    // ThisProduct.PmmGeneral.Canprint = binaryInt[0];

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
String PmmReadGeneralSettings(int RomTarget)
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
    // settings = String(settings + String(ThisProduct.PmmGeneral.IsModBus));
    // settings = String(settings + String(ThisProduct.PmmGeneral.IsCanBus));
    // settings = String(settings + String(ThisProduct.PmmGeneral.IsProfiBus));
    // settings = String(settings + String(ThisProduct.PmmGeneral.IsProfiNet));
    // settings = String(settings + String(ThisProduct.PmmGeneral.IsBACnet));
    // settings = String(settings + String(ThisProduct.PmmGeneral.IsDLMS));
    // settings = String(settings + String(ThisProduct.PmmGeneral.IsMBus));
    // settings = String(settings + String(ThisProduct.PmmGeneral.IsOPC));
    // settings = String(settings + String(ThisProduct.PmmGeneral.Canprint));
    // settings = String(settings + ",");
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

void PmmWriteTCPUDPSettings(string Message, int RomTarget)
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
String PmmReadTCPUDPSettings(int RomTarget)
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

void PmmWriteSerialSettings(string Message, int Portnumber, int RomTarget)
{
}
String PmmReadSerialSettings( int Portnumber, int RomTarget)
{

    return "Settings";
}

void PmmWriteProtocol(string Message, int PortNumer, int RomTarget)
{
    PmmStringToArray(Message);

    PmmConvertDecimalToBinary(stoi(values[0]));

    // int Spare00 ; (00)
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsModBus =binaryInt[15];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsCanBus =binaryInt[14];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsProfiBus =binaryInt[13];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsProfiNet =binaryInt[12];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsBACnet =binaryInt[11];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsDLMS =binaryInt[10];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsMBus =binaryInt[9];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsOPC =binaryInt[8];
    
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.Spare08 =binaryInt[7];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.Spare09 =binaryInt[6];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.Spare10 =binaryInt[5];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.Spare11 =binaryInt[4];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.Spare12 =binaryInt[3];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.Spare13 =binaryInt[2];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.Spare14 =binaryInt[1];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.IsRunning =binaryInt[0];
    
    // ModBus Settings (01)
    PmmConvertDecimalToBinary(stoi(values[1]));
    // int Spare00 ; (01)
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.ModBusRTU =binaryInt[15];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.ModBusTCP =binaryInt[14];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.ModBusSlave =binaryInt[13];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.ModBusMaster =binaryInt[12];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.ReadCoilsStatus =binaryInt[11];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.ReadInputStatus =binaryInt[10];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.ReadHoldingRegisters =binaryInt[9];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.ReadInputRegisters =binaryInt[8];
    
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.WriteSingleCoil =binaryInt[7];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.WriteSingleRegister =binaryInt[6];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.WriteMultipleCoils =binaryInt[5];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.WriteMultipleRegisters =binaryInt[4];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.CoilsStatus =binaryInt[3];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.InputStatus =binaryInt[2];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.HoldingRegisters =binaryInt[1];
    ThisProduct.PmmSerial[PortNumer].PmmProtocols.InputRegisters =binaryInt[0];
   
    // int StartingAddressCoilsStatus = 0; // 02
    // int StartingAddressInputStatus = 0; // 03
    // int StartingAddressHoldingRegisters = 0; //04
    // int StartingAddressInputRegisters = 0; //05
    // int StartingAddressWriteSingleCoil = 0; // 06
    // int StartingAddressWriteSingleRegister = 0; // 07
    // int StartingAddressWriteMultipleCoils = 0; // 08
    // int StartingAddressWriteMultipleRegisters = 0; //09
    // int QuantityCoilsStatus = 0; // 10
    // int QuantityInputStatus = 0; // 11
    // int QuantityHoldingRegisters = 0; // 12
    // int QuantityInputRegisters = 0; // 13
    // int QuantityWriteMultipleCoils = 0; // 14
    // int QuantityWriteMultipleRegisters = 0; // 15
    // int FunctionCode = 01;// 16
    // int IODataOrder = 1; // 17
    // int IODataType = 1; // 18
    // int PollInterval = 0; // 19
    // int SlaveID = 1; // 20
    // int ConnectionTimeout = 5000; // 21
    // int MaxRetry = 5; // 22

    // int Spare23 = 0;
    // int Spare24 = 0;
    // int Spare25 = 0;
    // int Spare26 = 0;
    // int Spare27 = 0;
    // int Spare28 = 0;
    // int Spare29 = 0;
    // int Spare30 = 0;
    // int Spare31 = 0;
    

    // For Serial
    if (PortNumer == 0)
        Serial_Protoco_store01.write(ThisProduct.PmmSerial[PortNumer].PmmProtocols);
    if (PortNumer == 1)
        Serial_Protoco_store02.write(ThisProduct.PmmSerial[PortNumer].PmmProtocols);
    if (PortNumer == 2)
        Serial_Protoco_store03.write(ThisProduct.PmmSerial[PortNumer].PmmProtocols);
    if (PortNumer == 3)
        Serial_Protoco_store04.write(ThisProduct.PmmSerial[PortNumer].PmmProtocols);
    // For Ethernet
    if (PortNumer == 9)
        TCPUDP_Protocol_store.write(ThisProduct.PmmTCPUDP.PmmProtocols);
}
String PmmReadProtocol(string Message, int Portnumber, int RomTarget)
{

    return "Settings";
}

void PmmWriteTimerSettings(string Message, int RomTarget)
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
String PmmReadTimersSettings(int RomTarget)
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

 void PmmWriteGerneralPurpose(string Message, int RomTarget ){}
 String PmmReadGerneralPurpose(int RomTarget){ return "Settings" ;}

 void PmmWriteDeviceCalibration(string Message,int PageNumber, int RomTarget ){}
 String PmmReadDeviceCalibration(int PageNumber,int RomTarget ){return "Settings" ;}



void PmmReadAllSettings(int RomTarget)
{
    // PmmReadGeneralSettings();
    // PmmReadRTUSettings();
    // PmmReadTCPUDPSettings();
    // PmmReadModbusSettings();
    // PmmReadTimersSettings();
}

/*****************************************************************
 * External flash section
 ******************************************************************/
byte PIN_FLASH_CS = 8;
PMM_SPI_FLASH myFlash;

/*****************************************************************
 * External EEPROM flash section
 ******************************************************************/

/*****************************************************************
 * Command Reader Functions
 ******************************************************************/

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

String PMMCommnads(string readData)
{
    String result = "Wrong command";
    if (readData.length() > 9)
    {       
    // rowData = 33 int starting from 0 => { int CommandCode, int ROMSelect ,int Data01 , int Data02 , ...
    // rowData = 001,001,000,
    std::string commandtype = readData.substr(0, 3); // obtain commandtype
    int CommandCode = stoi(commandtype) ; // obtain command code
    int RomSelect = stoi(readData.substr(5, 3));// obtain ROM Select=> 0:Internal flash ,1:Ext EEPROM ,Ext Flash
    readData.erase(0, 8); // Clean readData 
   
   switch (CommandCode)
   {
   case 100 : PmmWriteGeneralSettings(readData, RomSelect); break;
   case 101 : result = PmmReadGeneralSettings(RomSelect); break;
   case 102 : PmmWriteTCPUDPSettings(readData, RomSelect); break;
   case 103 : result = PmmReadTCPUDPSettings(RomSelect); break;
   case 104 : PmmWriteTimerSettings(readData, RomSelect); break;
   case 105 : result = PmmReadTimersSettings(RomSelect); break;
   
   case 110 : PmmWriteSerialSettings(readData,1,RomSelect); break;
   case 111 : result = PmmReadSerialSettings(1,RomSelect); break;

   case 112 : PmmWriteSerialSettings(readData,2,RomSelect); break;
   case 113 : result = PmmReadSerialSettings(2,RomSelect); break;

   case 114 : PmmWriteSerialSettings(readData,3,RomSelect); break;
   case 115 : result = PmmReadSerialSettings(3,RomSelect); break;

   case 116 : PmmWriteSerialSettings(readData,4,RomSelect); break;
   case 117 : result = PmmReadSerialSettings(4,RomSelect); break;


   case 900 :  result =  result = PMMIsAlive(); break; // 948,001,000
   case 948 :  NVIC_SystemReset(); break; // 948,001,000
   case 910 :  result = SetInternalRTC(readData); break; // 910,001,23,04,22,21,48,00
   case 911 :  result = GetInternalRTC(); break; // 911,001,000
   case 920 :  result = SetExternalRTC(readData); break;// 920,001,23,04,22,22,00,00
   case 921 :  result = GetExternalRTC(); break;// 921,001,000
 
   
   default: break;
   }

    }
    return result;
}

String SetInternalRTC(string Message)
{
   
    PmmStringToArray(Message);
    PmmRTCInternal.setDate(lowByte(stoi(values[2])), lowByte(stoi(values[1])), lowByte(stoi(values[0])));
    PmmRTCInternal.setTime(lowByte(stoi(values[3])), lowByte(stoi(values[4])), lowByte(stoi(values[5])));
    
    return "Done";
}

String GetInternalRTC()
{

    String result = "";

    result = String(PmmRTCInternal.getYear() );
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
    //result = result + ",";

    return result;
}
// External RTC DS3231
String SetExternalRTC(string Message)
{
    PmmStringToArray(Message);

    PmmRTCExternal.setYear(lowByte(stoi(values[0])) - 48 + 2000 ); // 48 correction for unix time
    PmmRTCExternal.setMonth(lowByte(stoi(values[1])));
    PmmRTCExternal.setDate(lowByte(stoi(values[2])));

    PmmRTCExternal.setClockMode(false); // 24h mode
    PmmRTCExternal.setHour(lowByte(stoi(values[3])));
    PmmRTCExternal.setMinute(lowByte(stoi(values[4])));
    PmmRTCExternal.setSecond(lowByte(stoi(values[5])));

    return "Done";
}
// External RTC DS3231
String GetExternalRTC()
{

    DateTime now = PmmRTCExternal.now();
    String result = "";

    result = String(now.year() - 2000);
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
    //result = result + ",";

    return result;
}
// Rturn Data from USB or HTTP
String PMMReturnDataFromSerialUSB()
{
    String Command = "";
    Command = SerialUSB.readStringUntil('\n');
    return Command;
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
