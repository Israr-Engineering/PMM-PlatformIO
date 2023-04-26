
#include <Arduino.h>
#include <string>

#include <PmmTypes.h>
#include <PmmEthernet.h>
//#include <PmmFlashStorage.h>
#include <FlashStorage_SAMD.h>
#include <PmmSPISerialFlash.h>

#include <PmmExternalEEPROM.h>

#include <PmmInternalRTC.h>
#include <PmmDS3231.h>

#include "PmmCommands.h"

/*
 // Common functions
void PmmStringToArray(string input);
void PMMIsAlive();
void IntToBits(int Dic);
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

string binary[16];
byte ByteArray[4];
uint16_t UInt16Array[2];
uint8_t UInt8Array[4];
Product ThisProduct;
PMMIO PmmIO;
PmmInternalRTC PmmRTCInternal;

PmmDS3231 PmmRTCExternal;
// Extra functions if needed
PmmRTClib PMMDS3231A;

PmmExternalEEPROM PmmEEporom;

/*****************************************************************
 * Common functions for all types of ROM
 ******************************************************************/

void PmmStringToArray(string input)
{
    // declaring character array (+1 for null terminator)
    char *char_array = new char[128];

    // SerialUSB.println(input.c_str());

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
        // SerialUSB.print(n);
        // SerialUSB.print(" : ");
        //  SerialUSB.println(s.c_str());
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

long BytesToLong()
{
    long LongVlaue = 0;
    LongVlaue = ByteArray[3] << 24 | ByteArray[2] << 16 | ByteArray[1] << 8 | ByteArray[0];

    return LongVlaue;
}

long UInt8ToLong()
{
    long LongVlaue = 0;
    LongVlaue = UInt8Array[3] << 24 | UInt8Array[2] << 16 | UInt8Array[1] << 8 | UInt8Array[0];

    return LongVlaue;
}

long UInt16ToLong()
{
    long LongVlaue = 0;
    // LongVlaue = UInt8Array[0];
    // LongVlaue = (LongVlaue << 16) | UInt8Array[1];

    LongVlaue = UInt16Array[1] << 16 | UInt16Array[0];

    return LongVlaue;
}

int BytesToInt(byte IntLowByte, byte IntHighByte)
{
    int tmpInt = IntHighByte;
    tmpInt = (tmpInt << 8) | IntLowByte;

    return tmpInt;
}

void LongToBytes(long LongVlaue)
{
    ByteArray[0] = LongVlaue & 0xFF;         // 0x78
    ByteArray[1] = (LongVlaue >> 8) & 0xFF;  // 0x56
    ByteArray[2] = (LongVlaue >> 16) & 0xFF; // 0x34
    ByteArray[3] = (LongVlaue >> 24) & 0xFF; // 0x12
}

void UnsignedLongToUInt16(unsigned long uLongVaue)
{
    UInt16Array[0] = uLongVaue & 0xFFFF;         // 0x78
    UInt16Array[1] = (uLongVaue >> 16) & 0xFFFF; // 0x56
}

void UnsignedLongToUInt8(unsigned long uLongVaue)
{
    UInt8Array[0] = uLongVaue & 0xFF;         // 0x78
    UInt8Array[1] = (uLongVaue >> 8) & 0xFF;  // 0x56
    UInt8Array[2] = (uLongVaue >> 16) & 0xFF; // 0x34
    UInt8Array[3] = (uLongVaue >> 24) & 0xFF; // 0x12
}

/*****************************************************************
 * Internal flash section
 ******************************************************************/
// // storage area definitions each should be max 256 bytes
// FlashStorage(General_flash_store, PMMGeneral);

// FlashStorage(Serial_flash_store00, PMMSERIAL);     // Ethernet port
// FlashStorage(Serial_Protoco_store00, PMMProtocol); // Ethernet port

// FlashStorage(Serial_flash_store01, PMMSERIAL);
// FlashStorage(Serial_Protoco_store01, PMMProtocol);

// FlashStorage(Serial_flash_store02, PMMSERIAL);
// FlashStorage(Serial_Protoco_store02, PMMProtocol);

// FlashStorage(Serial_flash_store03, PMMSERIAL);
// FlashStorage(Serial_Protoco_store03, PMMProtocol);

// FlashStorage(Serial_flash_store04, PMMSERIAL);
// FlashStorage(Serial_Protoco_store04, PMMProtocol);

// FlashStorage(Timers_flash_store, PMMTimer);

// FlashStorage(Claibration_flash_Page01, PMMDeviceCalibration);
// FlashStorage(Claibration_flash_Page02, PMMDeviceCalibration);
// FlashStorage(Claibration_flash_Page03, PMMDeviceCalibration);
// FlashStorage(Claibration_flash_Page04, PMMDeviceCalibration);

// FlashStorage(GerneralPurpose_flash_store, GerneralPurpose);

/*
          Message : comma sperated string with max 32 elements
                i : Serial port number
uint8_t RomTarget : 0 : internal / 1 : ext eeprom / 2 : ext flash
*/
String PmmWriteGeneralSettings(string Message, int RomTarget)
{
    String result = "";
    // 100,000,0620,20230425,1682370000,1682405659,0,1682405999,1000,10,13,11,1,36373,59392,1,5050,0,0,0,0,10,0,0,0,0,0,0
    PmmStringToArray(Message);

    ThisProduct.PmmGeneral.DeviceName = stoi(values[0]);
    ThisProduct.PmmGeneral.SerialNumber = stoul(values[1]);
    ThisProduct.PmmGeneral.FirstTimeRun = stoul(values[2]);
    ThisProduct.PmmGeneral.LifeTime = stoul(values[3]);
    ThisProduct.PmmGeneral.NumberOfCycles = stoul(values[4]);
    ThisProduct.PmmGeneral.LastRunningTime = stoul(values[5]);
    ThisProduct.PmmGeneral.NumberOfRunningTimes = stoul(values[6]);
    ThisProduct.PmmGeneral.SoftwareVersion = stoi(values[7]);
    ThisProduct.PmmGeneral.FirmwareVersion = stoi(values[8]);
    ThisProduct.PmmGeneral.HardwareVersion = stoi(values[9]);
    ThisProduct.PmmGeneral.ControlerType = stoi(values[10]);

    int tmpInt = stoi(values[11]);
    byte IntLowByte = lowByte(tmpInt);
    byte IntHighByte = highByte(tmpInt);

    ThisProduct.PmmGeneral.ItHasEthernet = bitRead(IntLowByte, 0);
    ThisProduct.PmmGeneral.ItHasSwitch = bitRead(IntLowByte, 1);
    ThisProduct.PmmGeneral.ItHasExtEEPROM = bitRead(IntLowByte, 2);
    ThisProduct.PmmGeneral.ItHasExtFlash = bitRead(IntLowByte, 3);

    ThisProduct.PmmGeneral.ItHasSerial = bitRead(IntLowByte, 4);
    ThisProduct.PmmGeneral.ItHasWebServer = bitRead(IntLowByte, 5);
    ThisProduct.PmmGeneral.ItHasFiber = bitRead(IntLowByte, 6);
    ThisProduct.PmmGeneral.LoRA = bitRead(IntLowByte, 7);

    ThisProduct.PmmGeneral.Zidbee = bitRead(IntHighByte, 0);
    ThisProduct.PmmGeneral.GSM = bitRead(IntHighByte, 1);
    ThisProduct.PmmGeneral.GPS = bitRead(IntHighByte, 2);
    ThisProduct.PmmGeneral.Antenna = bitRead(IntHighByte, 3);

    ThisProduct.PmmGeneral.ExternalRTC = bitRead(IntHighByte, 4);
    ThisProduct.PmmGeneral.InternalRTC = bitRead(IntHighByte, 5);
    ThisProduct.PmmGeneral.UDPOption = bitRead(IntHighByte, 6);
    ThisProduct.PmmGeneral.GateWay = bitRead(IntHighByte, 7);

    // // int Options02 ; //(18)
    tmpInt = stoi(values[12]);
    IntLowByte = lowByte(tmpInt);
    IntHighByte = highByte(tmpInt);

    ThisProduct.PmmGeneral.Canprint = bitRead(IntLowByte, 0);
    ThisProduct.PmmGeneral.Ext01Pac9535 = bitRead(IntLowByte, 1);
    ThisProduct.PmmGeneral.Ext02Pac9535 = bitRead(IntLowByte, 2);
    ThisProduct.PmmGeneral.Ext03Pac9535 = bitRead(IntLowByte, 3);

    ThisProduct.PmmGeneral.I2CServer = bitRead(IntLowByte, 4);
    ThisProduct.PmmGeneral.spare125 = bitRead(IntLowByte, 5);
    ThisProduct.PmmGeneral.spare126 = bitRead(IntLowByte, 6);
    ThisProduct.PmmGeneral.spare127 = bitRead(IntLowByte, 7);

    ThisProduct.PmmGeneral.spare128 = bitRead(IntHighByte, 0);
    ThisProduct.PmmGeneral.spare129 = bitRead(IntHighByte, 1);
    ThisProduct.PmmGeneral.spare130 = bitRead(IntHighByte, 2);
    ThisProduct.PmmGeneral.spare131 = bitRead(IntHighByte, 3);

    ThisProduct.PmmGeneral.spare132 = bitRead(IntHighByte, 4);
    ThisProduct.PmmGeneral.spare133 = bitRead(IntHighByte, 5);
    ThisProduct.PmmGeneral.spare134 = bitRead(IntHighByte, 6);
    ThisProduct.PmmGeneral.spare135 = bitRead(IntHighByte, 7);

    ThisProduct.PmmGeneral.Ext01Name = stoi(values[13]); //(19) 00 => zero = No extesion board
    ThisProduct.PmmGeneral.Ext01Address01 = lowByte(stoi(values[14]));
    ThisProduct.PmmGeneral.Ext01Address02 = highByte(stoi(values[14]));
    ThisProduct.PmmGeneral.Ext02Name = stoi(values[15]); //(19) 00 => zero = No extesion board
    ThisProduct.PmmGeneral.Ext02Address01 = lowByte(stoi(values[16]));
    ThisProduct.PmmGeneral.Ext02Address02 = highByte(stoi(values[16]));
    ThisProduct.PmmGeneral.Ext03Name = stoi(values[17]); //(19) 00 => zero = No extesion board
    ThisProduct.PmmGeneral.Ext03Address01 = lowByte(stoi(values[18]));
    ThisProduct.PmmGeneral.Ext03Address02 = highByte(stoi(values[18]));

    ThisProduct.PmmGeneral.I2CServerAddress = stoi(values[19]);
    ThisProduct.PmmGeneral.Spare26 = stoi(values[20]);
    ThisProduct.PmmGeneral.Spare27 = stoi(values[21]);
    ThisProduct.PmmGeneral.Spare28 = stoi(values[22]);
    ThisProduct.PmmGeneral.Spare29 = stoi(values[23]);
    ThisProduct.PmmGeneral.Spare30 = stoi(values[24]);

    ThisProduct.PmmGeneral.SettingsRef = stoi(values[25]);

    if (RomTarget == 0)
    {
         PmmInternalEEPROM.put(0, ThisProduct.PmmGeneral);
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        PmmEEporom.put(0, ThisProduct.PmmGeneral);
    }

    result = "Done";
    return result;
}
String PmmReadGeneralSettings(int RomTarget)
{

    if (RomTarget == 0)
    {
        //General_flash_store.read(ThisProduct.PmmGeneral);
        PmmInternalEEPROM.get(0,ThisProduct.PmmGeneral);

    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        PmmEEporom.get(0, ThisProduct.PmmGeneral);
    }

    String settings = "";

    // Build String

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

    byte IntLowByte = 0;
    byte IntHighByte = 0;

    bitWrite(IntLowByte, 0, ThisProduct.PmmGeneral.ItHasEthernet);
    bitWrite(IntLowByte, 1, ThisProduct.PmmGeneral.ItHasSwitch);
    bitWrite(IntLowByte, 2, ThisProduct.PmmGeneral.ItHasExtEEPROM);
    bitWrite(IntLowByte, 3, ThisProduct.PmmGeneral.ItHasExtFlash);
    bitWrite(IntLowByte, 4, ThisProduct.PmmGeneral.ItHasSerial);
    bitWrite(IntLowByte, 5, ThisProduct.PmmGeneral.ItHasWebServer);
    bitWrite(IntLowByte, 6, ThisProduct.PmmGeneral.ItHasFiber);
    bitWrite(IntLowByte, 7, ThisProduct.PmmGeneral.LoRA);

    bitWrite(IntHighByte, 0, ThisProduct.PmmGeneral.Zidbee);
    bitWrite(IntHighByte, 1, ThisProduct.PmmGeneral.GSM);
    bitWrite(IntHighByte, 2, ThisProduct.PmmGeneral.GPS);
    bitWrite(IntHighByte, 3, ThisProduct.PmmGeneral.Antenna);
    bitWrite(IntHighByte, 4, ThisProduct.PmmGeneral.ExternalRTC);
    bitWrite(IntHighByte, 5, ThisProduct.PmmGeneral.InternalRTC);
    bitWrite(IntHighByte, 6, ThisProduct.PmmGeneral.UDPOption);
    bitWrite(IntHighByte, 7, ThisProduct.PmmGeneral.GateWay);

    settings = String(settings + String(BytesToInt(IntLowByte, IntHighByte)));
    settings = String(settings + ",");

    bitWrite(IntLowByte, 0, ThisProduct.PmmGeneral.Canprint);
    bitWrite(IntLowByte, 1, ThisProduct.PmmGeneral.Ext01Pac9535);
    bitWrite(IntLowByte, 2, ThisProduct.PmmGeneral.Ext02Pac9535);
    bitWrite(IntLowByte, 3, ThisProduct.PmmGeneral.Ext03Pac9535);

    bitWrite(IntLowByte, 4, ThisProduct.PmmGeneral.I2CServer);
    bitWrite(IntLowByte, 5, ThisProduct.PmmGeneral.spare125);
    bitWrite(IntLowByte, 6, ThisProduct.PmmGeneral.spare126);
    bitWrite(IntLowByte, 7, ThisProduct.PmmGeneral.spare127);

    bitWrite(IntHighByte, 0, ThisProduct.PmmGeneral.spare128);
    bitWrite(IntHighByte, 1, ThisProduct.PmmGeneral.spare129);
    bitWrite(IntHighByte, 2, ThisProduct.PmmGeneral.spare130);
    bitWrite(IntHighByte, 3, ThisProduct.PmmGeneral.spare131);

    bitWrite(IntHighByte, 4, ThisProduct.PmmGeneral.spare132);
    bitWrite(IntHighByte, 5, ThisProduct.PmmGeneral.spare133);
    bitWrite(IntHighByte, 6, ThisProduct.PmmGeneral.spare134);
    bitWrite(IntHighByte, 7, ThisProduct.PmmGeneral.spare135);

    settings = String(settings + String(BytesToInt(IntLowByte, IntHighByte)));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmGeneral.Ext01Name)); //(19) 00 => zero = No extesion board
    settings = String(settings + ",");
    settings = String(settings + String(BytesToInt(ThisProduct.PmmGeneral.Ext01Address01,
                                                   ThisProduct.PmmGeneral.Ext01Address02)));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.Ext02Name)); //(19) 00 => zero = No extesion board
    settings = String(settings + String(BytesToInt(ThisProduct.PmmGeneral.Ext02Address01,
                                                   ThisProduct.PmmGeneral.Ext02Address02)));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.Ext03Name)); //(19) 00 => zero = No extesion board
    settings = String(settings + ",");
    settings = String(settings + String(BytesToInt(ThisProduct.PmmGeneral.Ext03Address01,
                                                   ThisProduct.PmmGeneral.Ext03Address02)));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.I2CServerAddress));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.Spare26));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.Spare27));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.Spare28));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.Spare29));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.Spare30));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmGeneral.SettingsRef));

    return settings;
}

String PmmWriteSerialSettings(string Message, int Portnumber, int RomTarget)
{
    String result = "";
    // 110,000,16779265,524289,9600,35,36,1,1,485,845440125,1677830336,4294967295,838926784,134744072,33687560,32965110,33096184,0,0,0,32771
    // 110,000,16779265,524289,9600,35,36,1,1,485,845440125,1677830336,-1,838926784,134744072,33687560,32965110,33096184,0,0,0,32771
    PmmStringToArray(Message);

    LongToBytes(stol(values[0]));                                 // 00000001000000000000100000000001
    ThisProduct.PmmSerial[Portnumber].PortStopBit = ByteArray[0]; // 00000001
    ThisProduct.PmmSerial[Portnumber].PortDataBit = ByteArray[1]; // 00001000
    ThisProduct.PmmSerial[Portnumber].PortParity = ByteArray[2];  // 00000000
    ThisProduct.PmmSerial[Portnumber].PortName = ByteArray[3];    // 00000001

    ThisProduct.PmmSerial[Portnumber].SerialConfig = stol(values[1]);
    ThisProduct.PmmSerial[Portnumber].BaudRate = stol(values[2]);
    ThisProduct.PmmSerial[Portnumber].TXPin = stoi(values[3]);
    ThisProduct.PmmSerial[Portnumber].RXPin = stoi(values[4]);

    ThisProduct.PmmSerial[Portnumber].SerialSelectionPin = stoi(values[5]);
    ThisProduct.PmmSerial[Portnumber].SerialPort = stoi(values[6]);
    ThisProduct.PmmSerial[Portnumber].Interface = stoi(values[7]);

    LongToBytes(stol(values[8]));                                  // 00110010011001000110010001111101
    ThisProduct.PmmSerial[Portnumber].MACAddress01 = ByteArray[0]; // 01111101
    ThisProduct.PmmSerial[Portnumber].MACAddress02 = ByteArray[1]; // 01001011
    ThisProduct.PmmSerial[Portnumber].MACAddress03 = ByteArray[2]; // 01100100
    ThisProduct.PmmSerial[Portnumber].MACAddress04 = ByteArray[3]; // 00110010
    // ThisProduct.PmmSerial[Portnumber].MACAddress05 = ByteArray[4]; // fixed
    // ThisProduct.PmmSerial[Portnumber].MACAddress06 = ByteArray[5]; // fixed

    UnsignedLongToUInt8(stoul(values[9]));                         // 01100100000000011010100011000000
    ThisProduct.PmmSerial[Portnumber].IpAddress01 = UInt8Array[0]; // 11000000
    ThisProduct.PmmSerial[Portnumber].IpAddress02 = UInt8Array[1]; // 10101000
    ThisProduct.PmmSerial[Portnumber].IpAddress03 = UInt8Array[2]; // 00000001
    ThisProduct.PmmSerial[Portnumber].IpAddress04 = UInt8Array[3]; // 01100100

    UnsignedLongToUInt8(stoul(values[10]));                         // 11111111111111111111111111111111
    ThisProduct.PmmSerial[Portnumber].SubnetMask01 = UInt8Array[0]; // 11111111
    ThisProduct.PmmSerial[Portnumber].SubnetMask02 = UInt8Array[1]; // 11111111
    ThisProduct.PmmSerial[Portnumber].SubnetMask03 = UInt8Array[2]; // 11111111
    ThisProduct.PmmSerial[Portnumber].SubnetMask04 = UInt8Array[3]; // 11111111

    UnsignedLongToUInt8(stoul(values[11]));                      // 00110010000000010000000111000000
    ThisProduct.PmmSerial[Portnumber].GateWay01 = UInt8Array[0]; // 11000000
    ThisProduct.PmmSerial[Portnumber].GateWay02 = UInt8Array[1]; // 10101000
    ThisProduct.PmmSerial[Portnumber].GateWay03 = UInt8Array[2]; // 00000001
    ThisProduct.PmmSerial[Portnumber].GateWay04 = UInt8Array[3]; // 00110010

    LongToBytes(stol(values[12]));                           // 00001000000010000000100000001000
    ThisProduct.PmmSerial[Portnumber].DNS101 = ByteArray[0]; // 00001000
    ThisProduct.PmmSerial[Portnumber].DNS102 = ByteArray[1]; // 00001000
    ThisProduct.PmmSerial[Portnumber].DNS103 = ByteArray[2]; // 00001000
    ThisProduct.PmmSerial[Portnumber].DNS104 = ByteArray[3]; // 00001000

    LongToBytes(stol(values[13]));                           // 00000010000000100000100000001000
    ThisProduct.PmmSerial[Portnumber].DNS201 = ByteArray[0]; // 00001000
    ThisProduct.PmmSerial[Portnumber].DNS202 = ByteArray[1]; // 00001000
    ThisProduct.PmmSerial[Portnumber].DNS203 = ByteArray[2]; // 00000010
    ThisProduct.PmmSerial[Portnumber].DNS204 = ByteArray[3]; // 00000010

    UnsignedLongToUInt16(stoul(values[14]));                   // 00000001111101110000000111110110
    ThisProduct.PmmSerial[Portnumber].Port01 = UInt16Array[0]; // 0000000111110110
    ThisProduct.PmmSerial[Portnumber].Port02 = UInt16Array[1]; // 0000000111110111

    UnsignedLongToUInt16(stoul(values[15]));                   // 00000001111110010000000111111000
    ThisProduct.PmmSerial[Portnumber].Port03 = UInt16Array[0]; // 0000000111111000
    ThisProduct.PmmSerial[Portnumber].Port04 = UInt16Array[1]; // 0000000111111001

    ThisProduct.PmmSerial[Portnumber].Spare28 = stoi(values[16]);
    ThisProduct.PmmSerial[Portnumber].Spare29 = stoi(values[17]);
    ThisProduct.PmmSerial[Portnumber].Spare30 = stoi(values[18]);

    // int Spare31 = 0;
    int tmpInt = stoi(values[19]);
    byte IntLowByte = lowByte(tmpInt); // 1000000000000011
    byte IntHighByte = highByte(tmpInt);

    ThisProduct.PmmSerial[Portnumber].Enabled = bitRead(IntLowByte, 0);
    ThisProduct.PmmSerial[Portnumber].spare3101 = bitRead(IntLowByte, 1);
    ThisProduct.PmmSerial[Portnumber].spare3102 = bitRead(IntLowByte, 2);
    ThisProduct.PmmSerial[Portnumber].spare3103 = bitRead(IntLowByte, 3);

    ThisProduct.PmmSerial[Portnumber].spare3104 = bitRead(IntLowByte, 4);
    ThisProduct.PmmSerial[Portnumber].spare3105 = bitRead(IntLowByte, 5);
    ThisProduct.PmmSerial[Portnumber].spare3106 = bitRead(IntLowByte, 6);
    ThisProduct.PmmSerial[Portnumber].spare3107 = bitRead(IntLowByte, 7);

    ThisProduct.PmmSerial[Portnumber].spare3108 = bitRead(IntHighByte, 0);
    ThisProduct.PmmSerial[Portnumber].spare3109 = bitRead(IntHighByte, 1);
    ThisProduct.PmmSerial[Portnumber].spare3110 = bitRead(IntHighByte, 2);
    ThisProduct.PmmSerial[Portnumber].spare3111 = bitRead(IntHighByte, 3);

    ThisProduct.PmmSerial[Portnumber].spare3112 = bitRead(IntHighByte, 4);
    ThisProduct.PmmSerial[Portnumber].spare3113 = bitRead(IntHighByte, 5);
    ThisProduct.PmmSerial[Portnumber].IsEthernet = bitRead(IntHighByte, 6);
    ThisProduct.PmmSerial[Portnumber].IsRunning = bitRead(IntHighByte, 7);

    if (RomTarget == 0)
    {
        switch (Portnumber)
        {
        case 0:
            PmmInternalEEPROM.put(256, ThisProduct.PmmSerial[0]); // Ethernet Port
            result = "Done 0";
            break;

        case 1:
            PmmInternalEEPROM.put(384, ThisProduct.PmmSerial[1]); // Serial Port
            result = "Done 1";
            break;
        case 2:
            PmmInternalEEPROM.put(512, ThisProduct.PmmSerial[2]); // Serial Port
            result = "Done 2";
            break;
        case 3:
            PmmInternalEEPROM.put(640, ThisProduct.PmmSerial[3]); // Serial Port
            result = "Done 3";
            break;
        case 4:
            PmmInternalEEPROM.put(768, ThisProduct.PmmSerial[4]); // Serial Port
            result = "Done 4";
            break;
        }
    }
    else if (RomTarget == 1) // EEProm address = 256 384 512 640 768
    {
        PmmEEporom.begin();
        switch (Portnumber)
        {
        case 0:
            PmmEEporom.put(256, ThisProduct.PmmSerial[0]); // Ethernet Port
            result = "Done 0";
            break;

        case 1:
            PmmEEporom.put(384, ThisProduct.PmmSerial[1]); // Ethernet Port
            result = "Done 1";
            break;
        case 2:
            PmmEEporom.put(512, ThisProduct.PmmSerial[2]); // Ethernet Port
            result = "Done 2";
            break;
        case 3:
            PmmEEporom.put(640, ThisProduct.PmmSerial[3]); // Ethernet Port
            result = "Done 3";
            break;
        case 4:
            PmmEEporom.put(768, ThisProduct.PmmSerial[4]); // Ethernet Port
            result = "Done 4";
            break;
        }
    }

    return result;
}
String PmmReadSerialSettings(int Portnumber, int RomTarget)
{
    String settings = "";
    if (RomTarget == 0)
    {
        switch (Portnumber)
        {
            case 0:
            //Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Ethernet Port
            PmmInternalEEPROM.get(256,ThisProduct.PmmSerial[Portnumber]);
            break;

            case 1:
            //Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Serial Port
            PmmInternalEEPROM.get(384,ThisProduct.PmmSerial[Portnumber]);
            break;

            case 2:
            //Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Serial Port
            PmmInternalEEPROM.get(512,ThisProduct.PmmSerial[Portnumber]);
            break;

            case 3:
            //Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Serial Port
            PmmInternalEEPROM.get(640,ThisProduct.PmmSerial[Portnumber]);
            break;

            case 4:
            //Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Serial Port
            PmmInternalEEPROM.get(768,ThisProduct.PmmSerial[Portnumber]);
            break;

        }
        
    }
    else if (RomTarget == 1) // EEProm address = 256 384 512 640 768
    {
        PmmEEporom.begin();
        switch (Portnumber)
        {
        case 0:
            PmmEEporom.get(256, ThisProduct.PmmSerial[0]); // Ethernet Port
            break;

        case 1:
            PmmEEporom.get(384, ThisProduct.PmmSerial[1]); // Ethernet Port
            break;
        case 2:
            PmmEEporom.get(512, ThisProduct.PmmSerial[2]); // Ethernet Port
            break;
        case 3:
            PmmEEporom.get(640, ThisProduct.PmmSerial[3]); // Ethernet Port
            break;
        case 4:
            PmmEEporom.get(768, ThisProduct.PmmSerial[4]); // Ethernet Port
            break;
        }
    }

    // settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressCoilsStatus));

    ByteArray[0] = ThisProduct.PmmSerial[Portnumber].PortStopBit;
    ByteArray[1] = ThisProduct.PmmSerial[Portnumber].PortDataBit;
    ByteArray[2] = ThisProduct.PmmSerial[Portnumber].PortParity;
    ByteArray[3] = ThisProduct.PmmSerial[Portnumber].PortName;

    long tpmLong = BytesToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].SerialConfig));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].BaudRate));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].TXPin));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].RXPin));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].SerialSelectionPin));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].SerialPort));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].Interface));
    settings = String(settings + ",");

    ByteArray[0] = ThisProduct.PmmSerial[Portnumber].MACAddress01;
    ByteArray[1] = ThisProduct.PmmSerial[Portnumber].MACAddress02;
    ByteArray[2] = ThisProduct.PmmSerial[Portnumber].MACAddress03;
    ByteArray[3] = ThisProduct.PmmSerial[Portnumber].MACAddress04;
    tpmLong = BytesToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");

    UInt8Array[0] = ThisProduct.PmmSerial[Portnumber].IpAddress01;
    UInt8Array[1] = ThisProduct.PmmSerial[Portnumber].IpAddress02;
    UInt8Array[2] = ThisProduct.PmmSerial[Portnumber].IpAddress03;
    UInt8Array[3] = ThisProduct.PmmSerial[Portnumber].IpAddress04;
    tpmLong = UInt8ToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");

    UInt8Array[0] = ThisProduct.PmmSerial[Portnumber].SubnetMask01;
    UInt8Array[1] = ThisProduct.PmmSerial[Portnumber].SubnetMask02;
    UInt8Array[2] = ThisProduct.PmmSerial[Portnumber].SubnetMask03;
    UInt8Array[3] = ThisProduct.PmmSerial[Portnumber].SubnetMask04;
    tpmLong = UInt8ToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");

    UInt8Array[0] = ThisProduct.PmmSerial[Portnumber].GateWay01;
    UInt8Array[1] = ThisProduct.PmmSerial[Portnumber].GateWay02;
    UInt8Array[2] = ThisProduct.PmmSerial[Portnumber].GateWay03;
    UInt8Array[3] = ThisProduct.PmmSerial[Portnumber].GateWay04;
    tpmLong = UInt8ToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");

    ByteArray[0] = ThisProduct.PmmSerial[Portnumber].DNS101;
    ByteArray[1] = ThisProduct.PmmSerial[Portnumber].DNS102;
    ByteArray[2] = ThisProduct.PmmSerial[Portnumber].DNS103;
    ByteArray[3] = ThisProduct.PmmSerial[Portnumber].DNS104;
    tpmLong = BytesToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");

    ByteArray[0] = ThisProduct.PmmSerial[Portnumber].DNS201;
    ByteArray[1] = ThisProduct.PmmSerial[Portnumber].DNS202;
    ByteArray[2] = ThisProduct.PmmSerial[Portnumber].DNS203;
    ByteArray[3] = ThisProduct.PmmSerial[Portnumber].DNS204;
    tpmLong = BytesToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");

    UInt16Array[0] = ThisProduct.PmmSerial[Portnumber].Port01;
    UInt16Array[1] = ThisProduct.PmmSerial[Portnumber].Port02;
    tpmLong = UInt16ToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");

    UInt16Array[0] = ThisProduct.PmmSerial[Portnumber].Port03;
    UInt16Array[1] = ThisProduct.PmmSerial[Portnumber].Port04;
    tpmLong = UInt16ToLong();
    settings = String(settings + String(tpmLong));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].Spare28));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].Spare29));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].Spare30));
    settings = String(settings + ",");

    // int Spare31 = 0;
    // int tmpInt = 0;
    byte IntLowByte = 0;
    byte IntHighByte = 0;

    bitWrite(IntLowByte, 0, ThisProduct.PmmSerial[Portnumber].Enabled);
    bitWrite(IntLowByte, 1, ThisProduct.PmmSerial[Portnumber].spare3101);
    bitWrite(IntLowByte, 2, ThisProduct.PmmSerial[Portnumber].spare3102);
    bitWrite(IntLowByte, 3, ThisProduct.PmmSerial[Portnumber].spare3103);

    bitWrite(IntLowByte, 4, ThisProduct.PmmSerial[Portnumber].spare3104);
    bitWrite(IntLowByte, 5, ThisProduct.PmmSerial[Portnumber].spare3105);
    bitWrite(IntLowByte, 6, ThisProduct.PmmSerial[Portnumber].spare3106);
    bitWrite(IntLowByte, 7, ThisProduct.PmmSerial[Portnumber].spare3107);

    bitWrite(IntHighByte, 0, ThisProduct.PmmSerial[Portnumber].spare3108);
    bitWrite(IntHighByte, 1, ThisProduct.PmmSerial[Portnumber].spare3109);
    bitWrite(IntHighByte, 2, ThisProduct.PmmSerial[Portnumber].spare3110);
    bitWrite(IntHighByte, 3, ThisProduct.PmmSerial[Portnumber].spare3111);

    bitWrite(IntHighByte, 4, ThisProduct.PmmSerial[Portnumber].spare3112);
    bitWrite(IntHighByte, 5, ThisProduct.PmmSerial[Portnumber].spare3113);
    bitWrite(IntHighByte, 6, ThisProduct.PmmSerial[Portnumber].IsEthernet);
    bitWrite(IntHighByte, 7, ThisProduct.PmmSerial[Portnumber].IsRunning);

    // tmpInt = IntHighByte ;
    // tmpInt = (tmpInt << 8) | IntLowByte ;
    settings = String(settings + String(BytesToInt(IntLowByte, IntHighByte)));

    return settings;
}

String PmmWriteProtocol(string Message, int Portnumber, int RomTarget)
{
    String result = "";
    // 130,000,384,162,0,100,200,300,10,20,30,40,03,1,1,1000,1,1000,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    PmmStringToArray(Message);
    int tmpInt = stoi(values[0]);        // 0000000110000000
    byte IntLowByte = lowByte(tmpInt);   // 10000000
    byte IntHighByte = highByte(tmpInt); // 00000001

    ThisProduct.PmmSerial[Portnumber].Enabled = bitRead(IntLowByte, 0);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsModBus = bitRead(IntLowByte, 0);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsCanBus = bitRead(IntLowByte, 1);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsProfiBus = bitRead(IntLowByte, 2);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsProfiNet = bitRead(IntLowByte, 3);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsBACnet = bitRead(IntLowByte, 4);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsDLMS = bitRead(IntLowByte, 5);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsMBus = bitRead(IntLowByte, 6);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsOPC = bitRead(IntLowByte, 7);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare08 = bitRead(IntHighByte, 0);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare09 = bitRead(IntHighByte, 1);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare10 = bitRead(IntHighByte, 2);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare11 = bitRead(IntHighByte, 3);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare12 = bitRead(IntHighByte, 4);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare13 = bitRead(IntHighByte, 5);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare14 = bitRead(IntHighByte, 6);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsRunning = bitRead(IntHighByte, 7);

    // ModBus Settings
    // int Spare00 ; (01)
    tmpInt = stoi(values[1]);       // 000000010100010
    IntLowByte = lowByte(tmpInt);   // 10100010
    IntHighByte = highByte(tmpInt); // 0000000
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ModBusRTU = bitRead(IntLowByte, 0);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ModBusTCP = bitRead(IntLowByte, 1);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ModBusSlave = bitRead(IntLowByte, 2);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ModBusMaster = bitRead(IntLowByte, 3);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ReadCoilsStatus = bitRead(IntLowByte, 4);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ReadInputStatus = bitRead(IntLowByte, 5);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ReadHoldingRegisters = bitRead(IntLowByte, 6);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ReadInputRegisters = bitRead(IntLowByte, 7);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.WriteSingleCoil = bitRead(IntHighByte, 0);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.WriteSingleRegister = bitRead(IntHighByte, 1);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.WriteMultipleCoils = bitRead(IntHighByte, 2);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.WriteMultipleRegisters = bitRead(IntHighByte, 3);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.CoilsStatus = bitRead(IntHighByte, 4);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.InputStatus = bitRead(IntHighByte, 5);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.HoldingRegisters = bitRead(IntHighByte, 6);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.InputRegisters = bitRead(IntHighByte, 7);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressCoilsStatus = stoi(values[2]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressInputStatus = stoi(values[3]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressHoldingRegisters = stoi(values[4]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressInputRegisters = stoi(values[5]);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.QuantityCoilsStatus = stoi(values[6]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.QuantityInputStatus = stoi(values[7]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.QuantityHoldingRegisters = stoi(values[8]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.QuantityInputRegisters = stoi(values[9]);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.FunctionCode = stoi(values[10]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IODataOrder = stoi(values[11]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.IODataType = stoi(values[12]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.PollInterval = stoi(values[13]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.SlaveID = stoi(values[14]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.ConnectionTimeout = stoi(values[15]);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.MaxRetry = stoi(values[16]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare17 = stoi(values[17]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare18 = stoi(values[18]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare19 = stoi(values[19]);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare20 = stoi(values[20]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare21 = stoi(values[21]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare22 = stoi(values[22]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare23 = stoi(values[23]);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare24 = stoi(values[24]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare25 = stoi(values[25]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare26 = stoi(values[26]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare27 = stoi(values[27]);

    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare28 = stoi(values[28]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare29 = stoi(values[29]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare30 = stoi(values[30]);
    ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare31 = stoi(values[31]);

    if (RomTarget == 0)
    {
        switch (Portnumber)
        {
        case 0:
           PmmInternalEEPROM.put(896, ThisProduct.PmmSerial[0].PmmProtocols); // Ethernet Port
            result = "Done 0";
            break;

        case 1:
            PmmInternalEEPROM.put(1024, ThisProduct.PmmSerial[1].PmmProtocols); // Ethernet Port
            result = "Done 1";
            break;
        case 2:
            PmmInternalEEPROM.put(1152, ThisProduct.PmmSerial[2].PmmProtocols); // Ethernet Port
            result = "Done 2";
            break;
        case 3:
            PmmInternalEEPROM.put(1280, ThisProduct.PmmSerial[3].PmmProtocols); // Ethernet Port
            result = "Done 3";
            break;
        case 4:
            PmmInternalEEPROM.put(1408, ThisProduct.PmmSerial[4].PmmProtocols); // Ethernet Port
            result = "Done 4";
            break;
        }
        
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        switch (Portnumber)
        {
        case 0:
            PmmEEporom.put(896, ThisProduct.PmmSerial[0].PmmProtocols); // Ethernet Port
            result = "Done 0";
            break;

        case 1:
            PmmEEporom.put(1024, ThisProduct.PmmSerial[1].PmmProtocols); // Ethernet Port
            result = "Done 1";
            break;
        case 2:
            PmmEEporom.put(1152, ThisProduct.PmmSerial[2].PmmProtocols); // Ethernet Port
            result = "Done 2";
            break;
        case 3:
            PmmEEporom.put(1280, ThisProduct.PmmSerial[3].PmmProtocols); // Ethernet Port
            result = "Done 3";
            break;
        case 4:
            PmmEEporom.put(1408, ThisProduct.PmmSerial[4].PmmProtocols); // Ethernet Port
            result = "Done 4";
            break;
        }
    }

    return result;
}
String PmmReadProtocol(int Portnumber, int RomTarget)
{
    String settings = "";

    if (RomTarget == 0)
    {

        switch (Portnumber)
        {
        case 0:
            PmmInternalEEPROM.get(896, ThisProduct.PmmSerial[0].PmmProtocols); // Ethernet Port
            break;
        case 1:
            PmmInternalEEPROM.get(1024, ThisProduct.PmmSerial[1].PmmProtocols); // Serial Port1
            break;
        case 2:
            PmmInternalEEPROM.get(1152, ThisProduct.PmmSerial[2].PmmProtocols); // Serial Port3
            break;
        case 3:
            PmmInternalEEPROM.get(1280, ThisProduct.PmmSerial[3].PmmProtocols); // Serial Port3
            break;
        case 4:
            PmmInternalEEPROM.get(1408, ThisProduct.PmmSerial[4].PmmProtocols); // Serial Port4
            break;
        }
                
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        switch (Portnumber)
        {
        case 0:
            PmmEEporom.get(896, ThisProduct.PmmSerial[0].PmmProtocols); // Ethernet Port
            break;
        case 1:
            PmmEEporom.get(1024, ThisProduct.PmmSerial[1].PmmProtocols); // Serial Port1
            break;
        case 2:
            PmmEEporom.get(1152, ThisProduct.PmmSerial[2].PmmProtocols); // Serial Port3
            break;
        case 3:
            PmmEEporom.get(1280, ThisProduct.PmmSerial[3].PmmProtocols); // Serial Port3
            break;
        case 4:
            PmmEEporom.get(1408, ThisProduct.PmmSerial[4].PmmProtocols); // Serial Port4
            break;
        }
    }

    // Build String
    //  int tmpInt = 0;
    byte IntLowByte = 0;
    byte IntHighByte = 0;

    bitWrite(IntLowByte, 0, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsModBus);
    bitWrite(IntLowByte, 1, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsCanBus);
    bitWrite(IntLowByte, 2, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsProfiBus);
    bitWrite(IntLowByte, 3, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsProfiNet);
    bitWrite(IntLowByte, 4, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsBACnet);
    bitWrite(IntLowByte, 5, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsDLMS);
    bitWrite(IntLowByte, 6, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsMBus);
    bitWrite(IntLowByte, 7, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsOPC);

    bitWrite(IntHighByte, 0, ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare08);
    bitWrite(IntHighByte, 1, ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare09);
    bitWrite(IntHighByte, 2, ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare10);
    bitWrite(IntHighByte, 3, ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare11);
    bitWrite(IntHighByte, 4, ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare12);
    bitWrite(IntHighByte, 5, ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare13);
    bitWrite(IntHighByte, 6, ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare14);
    bitWrite(IntHighByte, 7, ThisProduct.PmmSerial[Portnumber].PmmProtocols.IsRunning);

    settings = String(settings + String(BytesToInt(IntLowByte, IntHighByte)));
    settings = String(settings + ",");
    // ModBus Settings
    // int Spare00 ; (01)

    IntLowByte = 0;
    IntHighByte = 0;

    bitWrite(IntLowByte, 0, ThisProduct.PmmSerial[Portnumber].PmmProtocols.ModBusRTU);
    bitWrite(IntLowByte, 1, ThisProduct.PmmSerial[Portnumber].PmmProtocols.ModBusTCP);
    bitWrite(IntLowByte, 2, ThisProduct.PmmSerial[Portnumber].PmmProtocols.ModBusSlave);
    bitWrite(IntLowByte, 3, ThisProduct.PmmSerial[Portnumber].PmmProtocols.ModBusMaster);
    bitWrite(IntLowByte, 4, ThisProduct.PmmSerial[Portnumber].PmmProtocols.ReadCoilsStatus);
    bitWrite(IntLowByte, 5, ThisProduct.PmmSerial[Portnumber].PmmProtocols.ReadInputStatus);
    bitWrite(IntLowByte, 6, ThisProduct.PmmSerial[Portnumber].PmmProtocols.ReadHoldingRegisters);
    bitWrite(IntLowByte, 7, ThisProduct.PmmSerial[Portnumber].PmmProtocols.ReadInputRegisters);

    bitWrite(IntHighByte, 0, ThisProduct.PmmSerial[Portnumber].PmmProtocols.WriteSingleCoil);
    bitWrite(IntHighByte, 1, ThisProduct.PmmSerial[Portnumber].PmmProtocols.WriteSingleRegister);
    bitWrite(IntHighByte, 2, ThisProduct.PmmSerial[Portnumber].PmmProtocols.WriteMultipleCoils);
    bitWrite(IntHighByte, 3, ThisProduct.PmmSerial[Portnumber].PmmProtocols.WriteMultipleRegisters);
    bitWrite(IntHighByte, 4, ThisProduct.PmmSerial[Portnumber].PmmProtocols.CoilsStatus);
    bitWrite(IntHighByte, 5, ThisProduct.PmmSerial[Portnumber].PmmProtocols.InputStatus);
    bitWrite(IntHighByte, 6, ThisProduct.PmmSerial[Portnumber].PmmProtocols.HoldingRegisters);
    bitWrite(IntHighByte, 7, ThisProduct.PmmSerial[Portnumber].PmmProtocols.InputRegisters);

    settings = String(settings + String(BytesToInt(IntLowByte, IntHighByte)));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressCoilsStatus));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressInputStatus));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressHoldingRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.StartingAddressInputRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.QuantityCoilsStatus));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.QuantityInputStatus));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.QuantityHoldingRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.QuantityInputRegisters));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.FunctionCode));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.IODataOrder));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.IODataType));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.PollInterval));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.SlaveID));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.ConnectionTimeout));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.MaxRetry));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare17));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare18));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare19));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare20));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare21));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare22));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare23));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare24));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare25));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare26));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare27));
    settings = String(settings + ",");

    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare28));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare29));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare30));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmSerial[Portnumber].PmmProtocols.Spare31));

    return settings;
}

String PmmWriteTimerSettings(string Message, int RomTarget)
{
    String result = "";
    // 102,000,000,1000,1000,1000,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    PmmStringToArray(Message);

    ThisProduct.PmmTimers.ScanTimer = stoul(values[1]);
    ThisProduct.PmmTimers.ConfigTimer = stoul(values[2]);
    ThisProduct.PmmTimers.CommUpdateTimer = stoul(values[3]);

    // process the array
    const int arrLength = sizeof(ThisProduct.PmmTimers.Spare) / sizeof(ThisProduct.PmmTimers.Spare[0]);

    for (int i = 4; i < arrLength; i++)
    {
        ThisProduct.PmmTimers.Spare[i] = stoi(values[i]);
    }

    if (RomTarget == 0)
    {
        PmmInternalEEPROM.put(128, ThisProduct.PmmTimers);
    }
    else if (RomTarget == 1)
    {
        PmmEEporom.begin();
        PmmEEporom.put(128, ThisProduct.PmmTimers);
        result = "Done";
    }

    return result;
}
String PmmReadTimersSettings(int RomTarget)
{
    if (RomTarget == 0)
    {
        //ThisProduct.PmmTimers = Timers_flash_store.read();
        PmmInternalEEPROM.get(128, ThisProduct.PmmTimers);
    }
    else if (RomTarget == 1)
    {
        PmmEEporom.begin();
        PmmEEporom.get(128, ThisProduct.PmmTimers);
    }

    String settings = "";

    // build string
    // int tmpInt = 0;
    byte IntLowByte = 0;
    byte IntHighByte = 0;

    bitWrite(IntLowByte, 0, ThisProduct.PmmTimers.Cycle);
    bitWrite(IntLowByte, 0, ThisProduct.PmmTimers.FirstScan);
    bitWrite(IntLowByte, 0, ThisProduct.PmmTimers.OneMS);
    bitWrite(IntLowByte, 0, ThisProduct.PmmTimers.TenMS);

    bitWrite(IntLowByte, 0, ThisProduct.PmmTimers.M30ms);
    bitWrite(IntLowByte, 0, ThisProduct.PmmTimers.OneSec);
    bitWrite(IntLowByte, 0, ThisProduct.PmmTimers.M30Sec);
    bitWrite(IntLowByte, 0, ThisProduct.PmmTimers.OneMin);

    bitWrite(IntHighByte, 0, ThisProduct.PmmTimers.M30Min);
    bitWrite(IntHighByte, 0, ThisProduct.PmmTimers.OneHour);
    bitWrite(IntHighByte, 0, ThisProduct.PmmTimers.OneDay);
    bitWrite(IntHighByte, 0, ThisProduct.PmmTimers.OneMonnth);

    bitWrite(IntHighByte, 0, ThisProduct.PmmTimers.OneYear);
    bitWrite(IntHighByte, 0, ThisProduct.PmmTimers.Spare0012);
    bitWrite(IntHighByte, 0, ThisProduct.PmmTimers.Spare0013);
    bitWrite(IntHighByte, 0, ThisProduct.PmmTimers.Spare0014);

    settings = String(settings + String(BytesToInt(IntLowByte, IntHighByte)));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTimers.ScanTimer));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTimers.ConfigTimer));
    settings = String(settings + ",");
    settings = String(settings + String(ThisProduct.PmmTimers.CommUpdateTimer));
    settings = String(settings + ",");

    const int arrLength = sizeof(ThisProduct.PmmTimers.Spare) / sizeof(ThisProduct.PmmTimers.Spare[0]);

    for (int i = 0; i < arrLength; i++)
    {
        settings = String(settings + String(ThisProduct.PmmTimers.Spare[i]));
        settings = String(settings + ",");
    }

    return settings;
}

String PmmWriteGerneralPurpose(string Message, int RomTarget)
{
    String result = "";
    PmmStringToArray(Message);

    const int arrLength = sizeof(ThisProduct.PmmGerneralPurpose.Spare) / sizeof(ThisProduct.PmmGerneralPurpose.Spare[0]);

    for (int i = 0; i < arrLength; i++)
    {
        ThisProduct.PmmGerneralPurpose.Spare[i] = stoi(values[i]);
    }

    if (RomTarget == 0)
    {
       PmmInternalEEPROM.put(1536, ThisProduct.PmmGerneralPurpose);
        result = "Done";
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        PmmEEporom.put(1536, ThisProduct.PmmGerneralPurpose);
        result = "Done";
    }

    return result;
}
String PmmReadGerneralPurpose(int RomTarget)
{
    String settings = "";

    if (RomTarget == 0)
    {
        PmmInternalEEPROM.get(1536, ThisProduct.PmmTimers);
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        PmmEEporom.get(1536, ThisProduct.PmmTimers);
    }

    // Build String

    const int arrLength = sizeof(ThisProduct.PmmGerneralPurpose.Spare) / sizeof(ThisProduct.PmmGerneralPurpose.Spare[0]);

    for (int i = 0; i < arrLength; i++)
    {
        settings = String(settings + String(ThisProduct.PmmGerneralPurpose.Spare[i]));
        settings = String(settings + ",");
    }

    return settings;
}

String PmmWriteDeviceCalibration(string Message, int PageNumber, int RomTarget)
{
    String result = "";
    PmmStringToArray(Message);

    const int arrLength = sizeof(ThisProduct.PmmCalibrationPage[PageNumber].Calebrate) / sizeof(ThisProduct.PmmCalibrationPage[PageNumber].Calebrate[0]);

    for (int i = 0; i < arrLength; i++)
    {
        ThisProduct.PmmCalibrationPage[PageNumber].Calebrate[i] = stof(values[i]);
    }

    if (RomTarget == 0)
    {
       switch (PageNumber)
        {
        case 0:
           PmmInternalEEPROM.put(1664, ThisProduct.PmmCalibrationPage[0]);
            result = "Done 0";
            break;

        case 1:
            PmmInternalEEPROM.put(1792, ThisProduct.PmmCalibrationPage[1]);
            result = "Done 1";
            break;
        case 2:
            PmmInternalEEPROM.put(1920, ThisProduct.PmmCalibrationPage[2]);
            result = "Done 2";
            break;
        case 3:
            PmmInternalEEPROM.put(2048, ThisProduct.PmmCalibrationPage[3]);
            result = "Done 3";
            break;
        }
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        switch (PageNumber)
        {
        case 0:
            PmmEEporom.put(1664, ThisProduct.PmmCalibrationPage[0]);
            result = "Done 0";
            break;

        case 1:
            PmmEEporom.put(1792, ThisProduct.PmmCalibrationPage[1]);
            result = "Done 1";
            break;
        case 2:
            PmmEEporom.put(1920, ThisProduct.PmmCalibrationPage[2]);
            result = "Done 2";
            break;
        case 3:
            PmmEEporom.put(2048, ThisProduct.PmmCalibrationPage[3]);
            result = "Done 3";
            break;
        }
    }

    return result;
}
String PmmReadDeviceCalibration(int PageNumber, int RomTarget, long floatfactor)
{

    String settings = "";

    if (RomTarget == 0)
    {
        switch (PageNumber)
        {
        case 0:
            PmmInternalEEPROM.get(1664, ThisProduct.PmmCalibrationPage[0]);
            break;
        case 1:
            PmmInternalEEPROM.get(1792, ThisProduct.PmmCalibrationPage[1]);
            break;
        case 2:
            PmmInternalEEPROM.get(1920, ThisProduct.PmmCalibrationPage[2]);
            break;
        case 3:
            PmmInternalEEPROM.get(2048, ThisProduct.PmmCalibrationPage[3]);
            break;
        }
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        switch (PageNumber)
        {
        case 0:
            PmmEEporom.get(1664, ThisProduct.PmmCalibrationPage[0]);
            break;
        case 1:
            PmmEEporom.get(1792, ThisProduct.PmmCalibrationPage[1]);
            break;
        case 2:
            PmmEEporom.get(1920, ThisProduct.PmmCalibrationPage[2]);
            break;
        case 3:
            PmmEEporom.get(2048, ThisProduct.PmmCalibrationPage[3]);
            break;
        }
    }

    // Build String

    const int arrLength = sizeof(ThisProduct.PmmCalibrationPage[PageNumber].Calebrate) / sizeof(ThisProduct.PmmCalibrationPage[PageNumber].Calebrate[0]);

    SerialUSB.print("Calibration PageNumber : ");
    SerialUSB.println(PageNumber);
    SerialUSB.print("Calibration arrLength : ");
    SerialUSB.println(arrLength);

    float tmpFloat;
    long tmplong;

    for (int i = 0; i < arrLength; i++)
    {

        // settings = String(settings + String(  ThisProduct.PmmCalibrationPage[PageNumber].Calebrate[i],3));
        // settings = settings + ",";

        SerialUSB.println(ThisProduct.PmmCalibrationPage[PageNumber].Calebrate[i], 6);

        tmpFloat = ThisProduct.PmmCalibrationPage[PageNumber].Calebrate[i];
        tmplong = tmpFloat * floatfactor;

        settings = String(settings + String(tmplong));
        settings = settings + ",";
    }

    return settings;
}

void PmmReadAllSettings(int RomTarget)
{
    // STEP01 : Read general settings from internal flash
    // PmmReadGeneralSettings(0);
    // // STEP02 : Select ROM To read from where 0 =>Internal Flash,1=>EEPROM,2=>External Flash
    // RomTarget = ThisProduct.PmmGeneral.SettingsRef;
    // // STEP03 : Read Ethernet settings and protocols
    // if (ThisProduct.PmmGeneral.ItHasEthernet)
    // {
    //     PmmReadSerialSettings(0, RomTarget);
    //     PmmReadProtocol(0, RomTarget);
    // }
    // // STEP04 : Read Serial ports settings
    // PmmReadSerialSettings(1, RomTarget);
    // PmmReadProtocol(1, RomTarget);
    // PmmReadSerialSettings(2, RomTarget);
    // PmmReadProtocol(2, RomTarget);
    // PmmReadSerialSettings(3, RomTarget);
    // PmmReadProtocol(3, RomTarget);
    // PmmReadSerialSettings(4, RomTarget);
    // PmmReadProtocol(4, RomTarget);
    // // STEP05 : Read Timers settings
    // PmmReadTimersSettings(RomTarget);
    // // STEP06 : Read Calibration settings
    // PmmReadDeviceCalibration(0, RomTarget);
    // PmmReadDeviceCalibration(1, RomTarget);
    // PmmReadDeviceCalibration(2, RomTarget);
    // PmmReadDeviceCalibration(3, RomTarget);
    // // STEP07 : Read generl purpose settings
    // PmmReadGerneralPurpose(RomTarget);
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

String PMMCommnads(string readData)
{
    String result = "Wrong command";
    if (readData.length() > 9)
    {
        // rowData = 33 int starting from 0 => { int CommandCode, int ROMSelect ,int Data01 , int Data02 , ...
        // rowData = 001,001,000,
        std::string commandtype = readData.substr(0, 3); // obtain commandtype
        int CommandCode = stoi(commandtype);             // obtain command code
        int RomSelect = stoi(readData.substr(5, 2));     // obtain ROM Select=> 0:Internal flash ,1:Ext EEPROM ,Ext Flash
        readData.erase(0, 8);                            // Clean readData

        switch (CommandCode)
        {
        case 100:
            result = PmmWriteGeneralSettings(readData, RomSelect); // EEProm address = ( 0 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                                 // 100,000,0620,20230425,1682370000,1682405659,0,1682405999,1000,10,13,11,1,36373,59392,1,5050,0,0,0,0,10,0,0,0,0,0,0
        case 101:
            result = PmmReadGeneralSettings(RomSelect);
            break; // 101,000,000

        case 102:
            result = PmmWriteTimerSettings(readData, RomSelect); // EEProm address = ( 128 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                               // 102,000,000,1000,1000,1000,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        case 103:
            result = PmmReadTimersSettings(RomSelect);
            break; // 103,000,000

        case 110:
            result = PmmWriteSerialSettings(readData, 0, RomSelect); // EEProm address = ( 256 384 512 640 768 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                                   // Ethernet port  -- 110,000,16779265,524289,9600,35,36,1,1,485,845440125,1677830336,4294967295,838926784,134744072,33687560,32965110,33096184,0,0,0,32771
        case 111:
            result = PmmReadSerialSettings(0, RomSelect);
            break; // 111,000,000
        case 112:
            result = PmmWriteSerialSettings(readData, 1, RomSelect); // EEProm address = ( 256 384 512 640 768 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                                   // 112,000,16779265,524289,9600,35,36,1,1,485,845440125,1677830336,4294967295,838926784,134744072,33687560,32965110,33096184,0,0,0,32771
        case 113:
            result = PmmReadSerialSettings(1, RomSelect);
            break; // 113,000,000
        case 114:
            result = PmmWriteSerialSettings(readData, 2, RomSelect); // EEProm address = ( 256 384 512 640 768 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                                   // 114,000,16779265,524289,9600,35,36,1,1,485,845440125,1677830336,4294967295,838926784,134744072,33687560,32965110,33096184,0,0,0,32771
        case 115:
            result = PmmReadSerialSettings(2, RomSelect);
            break; // 115,000,000
        case 116:
            result = PmmWriteSerialSettings(readData, 3, RomSelect); // EEProm address = ( 256 384 512 640 768 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                                   // 116,000,16779265,524289,9600,35,36,1,1,485,845440125,1677830336,4294967295,838926784,134744072,33687560,32965110,33096184,0,0,0,32771
        case 117:
            result = PmmReadSerialSettings(3, RomSelect);
            break; // 117,000,000
        case 118:
            result = PmmWriteSerialSettings(readData, 4, RomSelect); // EEProm address = ( 256 384 512 640 768 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                                   // 118,000,16779265,524289,9600,35,36,1,1,485,845440125,1677830336,4294967295,838926784,134744072,33687560,32965110,33096184,0,0,0,32771
        case 119:
            result = PmmReadSerialSettings(4, RomSelect);
            break; // 119,000,000

        case 130:
            result = PmmWriteProtocol(readData, 0, RomSelect); // EEProm address = ( 896 1024 1152 1280 1408 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                             // 130,000,384,162,0,100,200,300,10,20,30,40,03,1,1,1000,1,1000,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        case 131:
            result = PmmReadProtocol(0, RomSelect);
            break; // 131,000,000
        case 132:
            result = PmmWriteProtocol(readData, 1, RomSelect); // EEProm address = ( 896 1024 1152 1280 1408 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                             // 132,000,384,162,0,100,200,300,10,20,30,40,03,1,1,1000,1,1000,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        case 133:
            result = PmmReadProtocol(1, RomSelect);
            break; // 133,000,000
        case 134:
            result = PmmWriteProtocol(readData, 2, RomSelect); // EEProm address = ( 896 1024 1152 1280 1408 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                             // 134,000,384,162,0,100,200,300,10,20,30,40,03,1,1,1000,1,1000,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        case 135:
            result = PmmReadProtocol(2, RomSelect);
            break; // 135,000,000
        case 136:
            result = PmmWriteProtocol(readData, 3, RomSelect); // EEProm address = ( 896 1024 1152 1280 1408 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                             // 136,000,384,162,0,100,200,300,10,20,30,40,03,1,1,1000,1,1000,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        case 137:
            result = PmmReadProtocol(3, RomSelect);
            break; // 137,000,000

        case 138:
            result = PmmWriteProtocol(readData, 4, RomSelect); // EEProm address = ( 896 1024 1152 1280 1408 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                             // 138,000,384,162,0,100,200,300,10,20,30,40,03,1,1,1000,1,1000,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        case 139:
            result = PmmReadProtocol(4, RomSelect);
            break; // 139,000,000

        case 140:
            result = PmmWriteGerneralPurpose(readData, RomSelect); // EEProm address = ( 1536 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                                 // 140,000,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
        case 141:
            result = PmmReadGerneralPurpose(RomSelect);
            break; // 141,000,000

        case 142:
            result = PmmWriteDeviceCalibration(readData, 0, 0); // EEProm address = ( 1664 1792 1920 2048 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                              // 142,000,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5
        case 143:
            PmmStringToArray(readData);
            result = PmmReadDeviceCalibration(0, RomSelect, stol(values[2]));
            break; // 143,000,1000000

        case 144:
            result = PmmWriteDeviceCalibration(readData, 1, 0); // EEProm address = ( 1664 1792 1920 2048 )  and Replace 000 In the second part of the comma seperated to 001
            break;                                              // 144,000,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5
        case 145:
            PmmStringToArray(readData);
            result = PmmReadDeviceCalibration(1, RomSelect, stol(values[2]));
            break; // 145,000,1000000

        case 146:
            result = PmmWriteDeviceCalibration(readData, 2, 0); // EEProm address = ( 1664 1792 1920 2048 )  and Replace 000 In the second part of the comma seperated to 001
            break;                                              // 146,000,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5
        case 147:
            PmmStringToArray(readData);
            result = PmmReadDeviceCalibration(2, RomSelect, stol(values[2]));
            break; // 147,000,1000000

        case 148:
            result = PmmWriteDeviceCalibration(readData, 3, 0); // EEProm address = ( 1664 1792 1920 2048 )  and Replace 000 In the second part of the comma seperated to 001
            break;                                              // 148,000,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5
        case 149:
            PmmStringToArray(readData);
            result = PmmReadDeviceCalibration(3, RomSelect, stol(values[2]));
            break; // 149,000,1000000

        case 900:
            result = PMMIsAlive();
            break; // 948,001,000
        case 948:
            NVIC_SystemReset();
            break; // 948,001,000
        case 910:
            result = SetInternalRTC(readData);
            break; // 910,001,23,04,22,21,48,00
        case 911:
            result = GetInternalRTC();
            break; // 911,000,,

        case 920:
            result = SetExternalRTC(readData);
            break; // 920,001,23,04,22,22,00,00
        case 921:
            result = GetExternalRTC();
            break; // 921,001,000

        default:
            break;
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
    // result = result + ",";

    return result;
}
// External RTC DS3231
String SetExternalRTC(string Message)
{
    PmmStringToArray(Message);

    PmmRTCExternal.setYear(lowByte(stoi(values[0])) - 48 + 2000); // 48 correction for unix time
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
    // result = result + ",";

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
