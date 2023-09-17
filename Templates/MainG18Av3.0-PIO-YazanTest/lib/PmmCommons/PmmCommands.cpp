
#include <Arduino.h>
#include <string>

#include <PmmTypes.h>
#include <PmmEthernet.h>
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
void PMMWebCommunication();
void PMMReadCommands();

String SetIpAddress(string Message);
String SetMACAddress(string Message);

String SetInternalRTC(string Message);
String GetInternalRTC();

String SetExternalRTC(string Message);
String GetExternalRTC();

String PMMReturnAPIFromHTTPHeader();
void ViewPMM0503Settings(PmmEthernetClient client);
void ViewPMM0501Settings(PmmEthernetClient client);
// void route(String HTTP_req, PmmEthernetClient client, String requestBody = "");
// void PMMIndexPage();
// void stringToWebPages(const String webPage, PmmEthernetClient client);
// void returnOKHeader(PmmEthernetClient client);
// void printHeaderTag(PmmEthernetClient client);
// void printNavAndLeftBar(PmmEthernetClient client);
// void getIndexPage(PmmEthernetClient client);
// void printFotterTag(PmmEthernetClient client);

PmmEthernetServer server(80);
char *strings[32]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;
string values[32];

uint8_t FACTORYRESET = 0;

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

void StartCommandHttpServer()
{

    if (ThisProduct.EthernetRunning == false)
    {

        // Reset Ehternet controller

        // read settings
        byte mac[6] = {};
        mac[0] = ThisProduct.PmmSerial[0].MACAddress01;
        mac[1] = ThisProduct.PmmSerial[0].MACAddress02;
        mac[2] = ThisProduct.PmmSerial[0].MACAddress03;
        mac[3] = ThisProduct.PmmSerial[0].MACAddress04;
        mac[4] = 0x48; // Locally administered always fixed
        mac[5] = 0x7E; // Locally administered always fixed

        IPAddress ip(ThisProduct.PmmSerial[0].IpAddress01,
                     ThisProduct.PmmSerial[0].IpAddress02,
                     ThisProduct.PmmSerial[0].IpAddress03,
                     ThisProduct.PmmSerial[0].IpAddress04);

        //  byte mac[] = {0x23, 0x22, 0x22, 0x22, 0x22, 0x22};
        // IPAddress ip(192, 168, 1, 110);
        Ethernet.init(SS); // for W5100 sspin
        Ethernet.begin(mac, ip);
        server.begin(); // port 80
        ThisProduct.EthernetRunning = true;
        // SerialUSB.print("-> Ethernet Connected  -> ");
        SerialUSB.println(Ethernet.localIP());
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

/*
          Message : comma sperated string with max 32 elements
                i : Serial port number
uint8_t RomTarget : 0 : internal / 1 : ext eeprom / 2 : ext flash
*/
String PmmWriteGeneralSettings(string Message, int RomTarget)
{
    String result = "";
    // 100,000,0620,20230425,1682370000,1682405659,0,1682405999,1000,10,13,11,1,36373,59392,1,5050,0,0,0,0,10,0,0,0,0,0,0
    if (Message != "*") // for internal call just to save struct to ROM
    {
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
    }

    if (RomTarget == 0)
    {
        PmmInternalEEPROM.put(0, ThisProduct.PmmGeneral);
        result = "Done RomTarget = 0";
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        PmmEEporom.put(0, ThisProduct.PmmGeneral);
        result = "Done RomTarget = 1";
    }

    return result;
}
String PmmReadGeneralSettings(int RomTarget)
{

    if (RomTarget == 0)
    {
        // General_flash_store.read(ThisProduct.PmmGeneral);
        PmmInternalEEPROM.get(0, ThisProduct.PmmGeneral);
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
    settings = String(settings + ",");
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
    if (Message != "*") // for internal call just to save struct to ROM
    {
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
    }
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
            // Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Ethernet Port
            PmmInternalEEPROM.get(256, ThisProduct.PmmSerial[Portnumber]);
            break;

        case 1:
            // Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Serial Port
            PmmInternalEEPROM.get(384, ThisProduct.PmmSerial[Portnumber]);
            break;

        case 2:
            // Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Serial Port
            PmmInternalEEPROM.get(512, ThisProduct.PmmSerial[Portnumber]);
            break;

        case 3:
            // Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Serial Port
            PmmInternalEEPROM.get(640, ThisProduct.PmmSerial[Portnumber]);
            break;

        case 4:
            // Serial_flash_store00.read(ThisProduct.PmmSerial[Portnumber]); // Serial Port
            PmmInternalEEPROM.get(768, ThisProduct.PmmSerial[Portnumber]);
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
    if (Message != "*") // for internal call just to save struct to ROM
    {
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
    }

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
    if (Message != "*") // for internal call just to save struct to ROM
    {
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
        // ThisProduct.PmmTimers = Timers_flash_store.read();
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
    if (Message != "*") // for internal call just to save struct to ROM
    {
        PmmStringToArray(Message);

        ThisProduct.PmmGerneralPurpose.Header = 159;

        const int arrLength = sizeof(ThisProduct.PmmGerneralPurpose.Spare) / sizeof(ThisProduct.PmmGerneralPurpose.Spare[0]);

        for (int i = 0; i < arrLength; i++)
        {
            ThisProduct.PmmGerneralPurpose.Spare[i] = stoi(values[i]);
        }
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
        PmmInternalEEPROM.get(1536, ThisProduct.PmmGerneralPurpose);
    }
    else if (RomTarget == 1) // EEprom
    {
        PmmEEporom.begin();
        PmmEEporom.get(1536, ThisProduct.PmmGerneralPurpose);
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
    if (Message != "*") // for internal call just to save struct to ROM
    {
        PmmStringToArray(Message);

        ThisProduct.PmmCalibrationPage[PageNumber].Header = 159;

        const int arrLength = sizeof(ThisProduct.PmmCalibrationPage[PageNumber].Calebrate) / sizeof(ThisProduct.PmmCalibrationPage[PageNumber].Calebrate[0]);

        for (int i = 0; i < arrLength; i++)
        {
            ThisProduct.PmmCalibrationPage[PageNumber].Calebrate[i] = stof(values[i]);
        }
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

String PmmReadPmmIO(int PageNumber, int RomTarget)
{

    // PageNumber = {0,1,2,3} => slot number in the hardware
    // RomTarget = {0,1}  => where '0' is outputs and '1' is inputs
    String settings = "";

    // Build String

    int i = 0;
    int arrLength = (PageNumber * 32) + 32;

    for (i = PageNumber * 32; i < arrLength; i++)
    {

        if (RomTarget == 1)
        {
            settings = String(settings + String(PmmIO.Inputs[i]));
        }
        else
        {

            settings = String(settings + String(PmmIO.Outputs[i]));
        }

        settings = settings + ",";
    }

    return settings;
}

String PmmReadPmmIO()
{

    // PageNumber = {0,1,2,3} => slot number in the hardware
    // RomTarget = {0,1}  => where '0' is outputs and '1' is inputs
    String settings = "";
    String settings01 = "";

    // Build String

    int i = 0;
    int arrLength = 128;

    for (i = 0; i < arrLength; i++)
    {

        settings = String(settings + String(PmmIO.Inputs[i]));

        settings01 = String(settings01 + String(PmmIO.Outputs[i]));

        settings = settings + ",";
        settings01 = settings01 + ",";
    }

    return settings + settings01;
}

String PmmWritePmmIO(int PageNumber)
{
    // PageNumber = {0,1,2,3} => slot number in the hardware
    String settings = "Done";

    // int i = 0 ;
    // int arrLength = (PageNumber * 32 ) + 32 ;

    for (int i = 0; i < 31; i++)
    {

        // PmmIO.Outputs[(i-1) + (32 * PageNumber)] = stoi(values[i]);
        PmmIO.Outputs[i + (32 * PageNumber)] = stoi(values[i + 1]);
    }

    return settings;
}

void PmmReadAllSettings(int RomTarget)
{

    // STEP01 : Read general settings from internal flash
    PmmReadGeneralSettings(0);

    // STEP02 : Defualt Factory Setting => Load First starting only , this will load the struct initial values
    if (FACTORYRESET == 1)
    {
        SerialUSB.println("Factory reset / First start Mode ..");
        ThisProduct.PmmGeneral.NumberOfCycles = 4294967295; // = 0xffff ffff
    }

    // STEP03 : First starting only
    if (ThisProduct.PmmGeneral.NumberOfCycles == 4294967295) // load defualt settings form struct's to internal flash 0xffff ffff
    {
        ThisProduct.PmmGeneral.NumberOfCycles = 1;
        PmmWriteGeneralSettings("0600,20230425,1682370000,1682405659,1,1682405999,1000,10,13,11,1,65535,59392,1,5050,0,0,0,0,10,0,0,0,0,0,0", 0);
        PmmWriteGeneralSettings("*", 0);
        PmmWriteSerialSettings("*", 0, 0); // Ethernet (192,168,1,110)
        PmmWriteSerialSettings("*", 1, 0); // Main Serial port
        PmmWriteSerialSettings("*", 2, 0); // Serial port 2
        PmmWriteSerialSettings("*", 3, 0); // Serial port 3
        PmmWriteSerialSettings("*", 4, 0); // Serial port 4

        PmmWriteProtocol("*", 0, 0);
        PmmWriteProtocol("*", 1, 0);
        PmmWriteProtocol("*", 2, 0);
        PmmWriteProtocol("*", 3, 0);
        PmmWriteProtocol("*", 4, 0);

        PmmWriteTimerSettings("*", 0); // Timers

        PmmWriteGerneralPurpose("*", 0);
        PmmWriteDeviceCalibration("*", 0);
        PmmWriteDeviceCalibration("*", 1);
        PmmWriteDeviceCalibration("*", 2);
        PmmWriteDeviceCalibration("*", 3);
    }
    else // Normal start
    {
        ThisProduct.FirstTimeStart = false;
        ThisProduct.PmmGeneral.NumberOfCycles++;

        // STEP02 : Select ROM To read from where 0 =>Internal Flash,1=>EEPROM,2=>External Flash
        RomTarget = ThisProduct.PmmGeneral.SettingsRef;
        // STEP03 : Read Ethernet settings and protocols
        PmmReadSerialSettings(0, RomTarget);
        PmmReadProtocol(0, RomTarget);
        // STEP04 : Read Serial ports settings
        PmmReadSerialSettings(1, RomTarget);
        PmmReadProtocol(1, RomTarget);
        PmmReadSerialSettings(2, RomTarget);
        PmmReadProtocol(2, RomTarget);
        PmmReadSerialSettings(3, RomTarget);
        PmmReadProtocol(3, RomTarget);
        PmmReadSerialSettings(4, RomTarget);
        PmmReadProtocol(4, RomTarget);
        // STEP05 : Read Timers settings
        PmmReadTimersSettings(RomTarget);
        // STEP06 : Read Calibration settings
        PmmReadDeviceCalibration(0, RomTarget);
        PmmReadDeviceCalibration(1, RomTarget);
        PmmReadDeviceCalibration(2, RomTarget);
        PmmReadDeviceCalibration(3, RomTarget);
        // STEP07 : Read generl purpose settings
        PmmReadGerneralPurpose(RomTarget);
    }
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
    String result = "Ignore";
    if (readData.length() > 9)
    {
        // rowData = 33 int starting from 0 => { int CommandCode, int ROMSelect ,int Data01 , int Data02 , ...
        // rowData = 001,001,000,
        std::string commandtype = readData.substr(0, 3); // obtain commandtype
        int CommandCode = stoi(commandtype);             // obtain command code
        // sserialUSB.println (CommandCode);
        int RomSelect = stoi(readData.substr(5, 2)); // obtain ROM Select=> 0:Internal flash ,1:Ext EEPROM ,Ext Flash
        readData.erase(0, 8);                        // Clean readData

        // SerialUSB.println(CommandCode);
        switch (CommandCode)
        {
        case 100:
            result = PmmWriteGeneralSettings(readData, RomSelect); // EEProm address = ( 0 ) and Replace 000 In the second part of the comma seperated to 001
            break;                                                 // 100,000,0620,20230425,1682370000,1682405659,0,1682405999,1000,10,13,11,1,65535,59392,1,5050,0,0,0,0,10,0,0,0,0,0,0
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
            break; // 143,000,000,000,1000000

        case 144:
            result = PmmWriteDeviceCalibration(readData, 1, 0); // EEProm address = ( 1664 1792 1920 2048 )  and Replace 000 In the second part of the comma seperated to 001
            break;                                              // 144,000,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5
        case 145:
            PmmStringToArray(readData);
            result = PmmReadDeviceCalibration(1, RomSelect, stol(values[2]));
            break; // 145,000,000,000,1000000

        case 146:
            result = PmmWriteDeviceCalibration(readData, 2, 0); // EEProm address = ( 1664 1792 1920 2048 )  and Replace 000 In the second part of the comma seperated to 001
            break;                                              // 146,000,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5
        case 147:
            PmmStringToArray(readData);
            result = PmmReadDeviceCalibration(2, RomSelect, stol(values[2]));
            break; // 147,000,000,000,1000000

        case 148:
            result = PmmWriteDeviceCalibration(readData, 3, 0); // EEProm address = ( 1664 1792 1920 2048 )  and Replace 000 In the second part of the comma seperated to 001
            break;                                              // 148,000,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5
        case 149:
            PmmStringToArray(readData);
            result = PmmReadDeviceCalibration(3, RomSelect, stol(values[2]));
            break; // 149,000,000,000,1000000

        case 160: // Monitoring All pages
            PmmStringToArray(readData);
            result = PmmReadPmmIO();
            // result = PmmReadDeviceCalibration(3, RomSelect, stol(values[2])); // MonitorIO => stol(values[0] = regidter to read

            break; // 160,001,001

        case 161: // Monitoring Read Inputs/Outputs Page
            // PageNumber = {0,1,2,3} => slot number in the hardware
            // RomTarget = {0,1}  => where '0' is outputs and '1' is inputs
            PmmStringToArray(readData);
            SerialUSB.println(stoi(values[0]));
            result = PmmReadPmmIO(RomSelect, stoi(values[0]));

            break; // 161,001,001 => to read all inputs ,'161,000,001' for all outputs

        case 163:
            // Monitoring =>> Write Outputs Page
            // PageNumber = {0,1,2,3} => slot number in the hardware => values[0]
            PmmStringToArray(readData);
            SerialUSB.println(163);
            SerialUSB.println(stoi(values[0]));
            result = PmmWritePmmIO(stoi(values[0]));
            // result = "Done";

            break; // 163,001,001,65535,65535,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

            // Actions from PC Configrator

        case 900:
            // result = PMMIsAlive();
            result = "Alive!!";
            break; // 900,001,000
        case 948:
            NVIC_SystemReset();
            break; // 948,001,000
        case 910:
            result = SetInternalRTC(readData);
            break; // 910,001,23,04,22,21,48,00
        case 911:
            result = GetInternalRTC();
            break; // 911,000,000

        case 920:
            result = SetExternalRTC(readData);
            break; // 920,001,23,04,22,22,00,00
        case 921:
            result = GetExternalRTC();
            break; // 921,001,000

        case 950:
            ThisProduct.EthernetRunning = false;
            break; // 950,000,000   reconnect Ethernet and show IP Address

        case 951:
            SetIpAddress(readData);
            ThisProduct.EthernetRunning = false;
            break; // 951,000,000,192,168,1,110   reconnect Ethernet and Set IP Address

        case 952:
            SetMACAddress(readData);
            ThisProduct.EthernetRunning = false;
            break; // 952,000,000,192,192,192,192    reconnect Ethernet and show MAC Address

        default:
            break;
        }
    }
    return result;
}

String SetIpAddress(string Message)
{

    PmmStringToArray(Message);
    ThisProduct.PmmSerial[0].IpAddress01 = stoi(values[1]);
    ThisProduct.PmmSerial[0].IpAddress02 = stoi(values[2]);
    ThisProduct.PmmSerial[0].IpAddress03 = stoi(values[3]);
    ThisProduct.PmmSerial[0].IpAddress04 = stoi(values[4]);

    return "Done";
}

// String SetMACAddress(string Message);

String SetMACAddress(string Message)
{

    PmmStringToArray(Message);
    ThisProduct.PmmSerial[0].MACAddress01 = stoi(values[1]);
    ThisProduct.PmmSerial[0].MACAddress02 = stoi(values[2]);
    ThisProduct.PmmSerial[0].MACAddress03 = stoi(values[3]);
    ThisProduct.PmmSerial[0].MACAddress04 = stoi(values[4]);

    return "Done";
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

    if (ThisProduct.EthernetRunning == true)
    {
        String tmpString = PMMReturnAPIFromHTTPHeader(); // http://192.168.1.107/api?message=900,001,000

        if (tmpString != "")
        {
            SerialUSB.print("Recived Http command : ");
            SerialUSB.println(tmpString);
        }
    }
}

String PMMReturnAPIFromHTTPHeader()
{
    // SerialUSB.println("Test incoming message ..");

    String APIData = "";
    String Data = "";
    String readString = " ";
    PmmEthernetClient client = server.available();
    if (client)
    {
        // SerialUSB.println("Client reciveing");
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();

                if (readString.length() < 1200)
                {
                    readString += c;
                }
                if (c == '\n')
                {
                    if (readString.indexOf("settings0503.html") >= 0)
                    {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close"); // the connection will be closed after completion of the response
                        // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println();
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");
                        client.println("<head>");

                        ViewPMM0503Settings(client);

                        client.println("</body>");
                        client.println("</html>");

                        client.stop();
                    }
                    else if (readString.indexOf("settings0501.html") >= 0)
                    {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close"); // the connection will be closed after completion of the response
                        // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println();
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");
                        client.println("<head>");

                        ViewPMM0501Settings(client);

                        client.println("</body>");
                        client.println("</html>");

                        client.stop();
                    }
                    else
                    {
                        APIData = readString.substring(readString.indexOf("api?message=") + 12, readString.indexOf("HTTP/1.1"));
                        string cmd = APIData.c_str();
                        Data = PMMCommnads(cmd);

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close"); // the connection will be closed after completion of the response
                        // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println();
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println(Data);
                        client.println("</head>");
                        client.println("<html>");

                        client.stop();
                        // SerialUSB.println(readString);
                        // SerialUSB.println(APIData);
                    }
                }
            }
        }
    }
    if (Data != "")
        Data = "Result : " + Data;
    return Data;
}

void ViewPMM0503Settings(PmmEthernetClient client)
{
    client.println("<title>PMM-Easy Config</title> <style>.footer{position:fixed;bottom:5px;color:var(--contentTextColor);text-align:center;font-size:13px;padding:2px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.footer p{display:contents}body{font-family:Arial,sans-serif;margin:0;padding:0;overflow-x:hidden}#navbar{background-color:#339898;color:#fff;padding:10px;height:44px;width:100%}");
    client.println("#leftbar,#navbar{background-color:#339898;color:#fff}#leftbar,#navbar,span{color:#fff}.button,.button2{text-align:center;text-decoration:none;display:inline-block;cursor:pointer}.ddl,.input,textarea{box-sizing:border-box}.button,.button2,.container{cursor:pointer}.footer{position:fixed;bottom:5px;color:var(--contentTextColor);text-align:center;font-size:13px;padding:2px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.footer p{display:contents}body{font-family:Arial,sans-serif;margin:0;padding:0;overflow-x:hidden}#navbar{padding:10px;height:44px;width:100%}#leftbar{width:190px;padding:10px;position:absolute;top:0;left:0;height:100%;margin-top:64px}#content{margin-left:210px;padding:20px}.content-div{background-color:#eee;padding:10px;margin-bottom:10px;height:350px}ul{list-style:none}li{padding:12px;width:92%}#div2,#div3{display:none}.button{width:40px;height:40px}.button2{width:80px;height:40px}.container,.container input:checked~.checkmark:after{display:block}.button3,.ddl,.input{height:28px;position:absolute}textarea{width:89%;height:40px;padding:8px;border:2px solid #ccc;border-radius:4px;font-size:12px;resize:none}.input.udpport {width: 45px;position: static;}.ddl,.input{width:10%;left:24%;border:2px solid #ccc}.button3{width:140px}.container{position:relative;font-size:16px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.container input{position:absolute;opacity:0;cursor:pointer;height:0;width:0}.checkmark{position:absolute;top:-6px;left:13.5%;height:25px;width:25px;background-color:#eee}.container:hover input~.checkmark{background-color:#ccc}.container input:checked~.checkmark{background-color:#2196f3}.checkmark:after{content:'';");
    client.println("position:absolute;display:none}.container .checkmark:after{left:9px;top:5px;width:5px;height:10px;border:solid #fff;border-width:0 3px 3px 0;-webkit-transform:rotate(45deg);-ms-trasform:rotate(45deg);transform:rotate(45deg)}.Divs.active{background-color:#fff;color:#339898}</style></head>");
    client.println("<body> <div id='navbar'> <h1 style='position: absolute;top: -6px;left: 26px;'>PMM</h1> <h4 style='position: absolute;top: 7px;left: 112px;'>Easy Config 0503</h4> </div><div id='leftbar'> <ul style='margin-left: -10%;'> <li class='Divs' onclick='GoToDiv(\"div1\");' id='div1_btn'> <span id='div1_span' style='font-size: 14px;'> Ethernet (Serial 00)</span> </li><li class='Divs' onclick='GoToDiv(\"div2\");' id='div2_btn'> <span id='div2_span' style='font-size: 14px;'> Serial (Serial 00)</span> </li><li class='Divs' onclick='GoToDiv(\"div3\");' id='div3_btn'> <span id='div3_span' style='font-size: 14px;'> Operation Mode</span> </li><button class='button3' onclick='ReadFromController();' style='margin-top: 10%;'>Upload</button><br><br><button class='button3' onclick='WriteToController();' style='margin-top: 10%;'>Download</button><br><br><button class='button3' onclick='ResetMCU();' style='margin-top: 10%;'>Reset MCU</button><br><br></ul> <div class='footer'> <p>&#169;2014-</p><span id='thisYear'> </span><br><p>PMM LLC. All rights reserved.</p><br><a style='text-decoration: none;color: #fff;' href='https://pmm-usa.us/'>www.pmm-usa.us</a> </div></div>");
    client.println("<div id='content'> <div class='content-div' id='div1'> <h2>Ethernet (Serial 00)</h2> <label class='container'>Enabled <input type='checkbox' id='Enabled_Ethernet'> <span class='checkmark'></span> </label><br><label for='IPAddress'>IP Address</label> <input type='text' class='input' id='IPAddress' name='IPAddress' value='192.168.1.100'><br><br><label for='NetMask'>Net Mask</label> <input type='text' class='input' id='NetMask' name='NetMask' value='255.255.255.0'><br><br><label for='PreferredDNS'>Preferred DNS</label> <input type='text' class='input' id='PreferredDNS' name='PreferredDNS' value='8.8.8.8'><br><br><label for='AlternateDNS'>Alternate DNS</label> <input type='text' class='input' id='AlternateDNS' name='AlternateDNS' value='8.8.4.4'><br><br><label for='Gateway'>Gateway</label> <input type='text' class='input' id='Gateway' name='Gateway' value='192.168.1.1'><br><br><label for='MacAddress'>Mac Address</label> <label for='MacAddress' style='position: absolute;left: 24%;top: 40%;'>48:7E</label> <input type='text' class='input' id='MacAddress' name='MacAddress' value='72:64:64:32' style='width: 7%;left: 27%;'><br><br><label for='UDPPort'>UDP Port</label> <input type='number' class='input' id='UDPPort' name='UDPPort' style='width:60px;' value='502'><br><br></div>");
    client.println("<div class='content-div' id='div2'> <h2>Serial (Serial 00)</h2> <label class='container'>Enabled <input type='checkbox' id='Enabled_Serial'> <span class='checkmark'></span> </label><br><label for='BaudRate'>Baud Rate</label> <select id='BaudRate' class='ddl'> <option value='300'>300</option> <option value='600'>600</option> <option value='1200'>1200</option> <option value='2400'>2400</option> <option value='4800'>4800</option> <option value='9600'>9600</option> <option value='14400'>14400</option> <option value='19200'>19200</option> <option value='38400'>38400</option> <option value='56000'>56000</option> <option value='57600'>57600</option> <option value='115200'>115200</option> </select><br><br><label for='DataBit'>Data Bit</label> <select id='DataBit' class='ddl'> <option value='5'>5</option> <option value='6'>6</option> <option value='7'>7</option> <option value='8'>8</option> </select><br><br><label for='StopBit'>Stop Bit</label> <select id='StopBit' class='ddl'> <option value='1'>1</option> <option value='2'>2</option> </select><br><br><label for='Parity'>Parity</label> <select id='Parity' class='ddl'> <option value='None'>None</option> <option value='Odd'>Odd</option> <option value='Even'>Even</option> </select><br><br><label for='Interface'>Interface</label> <select id='Interface' class='ddl'> <option value='485'>RS485</option> <option value='422'>RS422</option> <option value='232'>RS232</option> </select><br><br><!-- <label for='SerialConfig'>Serial Config</label> <input type='text' class='input' id='SerialConfig' name='SerialConfig' value='1043'><br><br>--> <label for='PortID'>Port ID</label> <input type='number' class='input' id='PortID' name='PortID' style='width:7%;' value='1'> <label style='margin-left: 16%;position: absolute;top: 40%;'>(1-30)</label><br><br></div>");
    client.println("<div class='content-div' id='div3'> <h2>Operation Mode</h2><input type='radio' id='SerialServer' name='Operation_Mode' value='SerialServer' checked><label for='SerialServer'>Serial Server</label><br><p style='margin-left: 3%;'>multi ethernet client can communicate with serial port on this device</p><br><input type='radio' id='PeerToPeer' name='Operation_Mode' value='PeerToPeer'><label for='PeerToPeer'>Peer To Peer</label><br><label for='RemoteIP' style='top: 6px;position: relative;margin-left: 42px;'>Remote IP</label> <label style='top: 6px;position: relative;margin-left: 7%;'>192.168.1.</label> <input type='number' min='1' class='input' id='RemoteIP' name='RemoteIP' style='width:3%;left: 29%;' value='200'><br><br></div>");
    client.println("<div class='content-div' id='div4' style='height: 75px;'> <h2>Command</h2> <div style='display: inline-block; position: absolute; margin-left: 10%; width: 80%; top: 51%;'> <textarea id='textarea_cmd'>Command...</textarea> </div></div></div>");
    client.println("<script>var d = new Date();\n        var y = d.getFullYear();\n        document.getElementById(\"thisYear\").textContent = y;\n        let CommandStr = \"\";\n        let SerialConfig = 1043;\n        GoToDiv(\"div1\");\n        ReadFromController();\n        ");
    client.println("function GoToDiv(div) {if (div == \"div1\") {document.getElementById(\"div1\").style.display = \"block\";document.getElementById(\"div2\").style.display = \"none\";document.getElementById(\"div3\").style.display = \"none\";document.getElementById(\"div1_btn\").classList.add(\"active\");document.getElementById(\"div2_btn\").classList.remove(\"active\");document.getElementById(\"div3_btn\").classList.remove(\"active\");document.getElementById(\"div1_span\").style.color = \"#339898\";document.getElementById(\"div2_span\").style.color = \"#ffffff\";document.getElementById(\"div3_span\").style.color = \"#ffffff\";}else if (div == \"div2\") {\n                document.getElementById(\"div1\").style.display = \"none\";document.getElementById(\"div2\").style.display = \"block\";document.getElementById(\"div3\").style.display = \"none\";document.getElementById(\"div1_btn\").classList.remove(\"active\");document.getElementById(\"div2_btn\").classList.add(\"active\");document.getElementById(\"div3_btn\").classList.remove(\"active\");document.getElementById(\"div1_span\").style.color = \"#ffffff\";document.getElementById(\"div2_span\").style.color = \"#339898\";document.getElementById(\"div3_span\").style.color = \"#ffffff\";\n            }\n            ");
    client.println("else if (div == \"div3\") {document.getElementById(\"div1\").style.display = \"none\";document.getElementById(\"div2\").style.display = \"none\";document.getElementById(\"div3\").style.display = \"block\";document.getElementById(\"div1_btn\").classList.remove(\"active\");document.getElementById(\"div2_btn\").classList.remove(\"active\");document.getElementById(\"div3_btn\").classList.add(\"active\");document.getElementById(\"div1_span\").style.color = \"#ffffff\";document.getElementById(\"div2_span\").style.color = \"#ffffff\";document.getElementById(\"div3_span\").style.color = \"#339898\";}}");
    client.println("function convertIPToDecimal(b1, b2, b3, b4) {\n            var binaryString =\n                b4.toString(2).padStart(8, '0') +\n                b3.toString(2).padStart(8, '0') +\n                b2.toString(2).padStart(8, '0') +\n                b1.toString(2).padStart(8, '0');\n            return parseInt(binaryString, 2).toString();\n        }\n        ");
    client.println("function convertMacToDecimal(byte1, byte2, byte3, byte4) {\n            var macBytes = [byte1, byte2, byte3, byte4];\n\n            var binaryString = \"\";\n            for (var i = 3; i >= 0; i--) {\n                binaryString += macBytes[i].toString(2).padStart(8, '0');\n            }\n\n            return parseInt(binaryString, 2).toString();\n        }\n        ");
    client.println("function numberToByte(number) {\n            if (number >= 0 && number <= 255) {\n                return number & 0xFF;\n            } else {\n                return null;\n            }\n        }\n        ");
    client.println("function convertBoolToString(val) {\n            var result = \"0\";\n            if (val !== null && val !== undefined) {\n                val = val.toString().toLowerCase();\n                if (val === \"false\" || val === \"0\") {\n                    result = \"0\";\n                } else if (val === \"true\" || val === \"1\") {\n                    result = \"1\";\n                }\n            }\n            return result;\n        }\n        ");
    client.println("function WriteToController() {\n            var sparity = document.getElementById(\"Parity\").value\n            let parity = 1;\n            if (sparity === \"None\") parity = 3;\n            if (sparity === \"Odd\") parity = 2;\n            var Val1 = convertIPToDecimal(\n                parseInt(document.getElementById(\"StopBit\").value),\n                parseInt(document.getElementById(\"DataBit\").value),\n                parseInt(parity),\n                1\n            );\n            var Val2 = convertMacToDecimal(\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[0]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[1]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[2]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[3])\n            );\n            var Val3 = convertIPToDecimal(\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[3])\n            );\n            var Val4 = convertIPToDecimal(\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[3]));");
    client.println("var Val5 = convertIPToDecimal(parseInt((document.getElementById(\"Gateway\").value).split(\".\")[0]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[1]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[2]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[3]));\n            var Val6 = convertIPToDecimal(\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[3])\n            );\n            var Val7 = convertIPToDecimal(\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[3])\n            );\n\n            SerialConfig = calcSerialConfig(\n                parseInt(document.getElementById(\"StopBit\").value),\n                document.getElementById(\"Parity\").value,\n                parseInt(document.getElementById(\"StopBit\").value)\n            );\n            var PeerToPeer = \"0\";\n            var SerialServer = \"0\";\n            var Operation_Mode = document.querySelector(\"input[name='Operation_Mode']:checked\").value;\n            if (Operation_Mode == \"PeerToPeer\") { PeerToPeer = \"1\"; }\n            if (Operation_Mode == \"SerialServer\") { SerialServer = \"1\"; }\n            var EnabledSerial = \"0\";\n            var EnabledEthernet = \"0\";\n            var Enabled_Ethernet = document.getElementById(\"Enabled_Ethernet\").checked;\n            var Enabled_Serial = document.getElementById(\"Enabled_Serial\").checked;\n            if (Enabled_Ethernet) { EnabledEthernet = \"1\"; }\n         ");
    client.println("if (Enabled_Serial) { EnabledSerial = \"1\"; }\n            var Bits = convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(PeerToPeer);Bits += convertBoolToString(SerialServer);Bits += convertBoolToString(EnabledSerial);Bits += convertBoolToString(EnabledEthernet);");
    client.println("var CmdWriteEthernet_Serial_00 = \"110,000,\";\n            CmdWriteEthernet_Serial_00 += Val1 + \",\";\n            CmdWriteEthernet_Serial_00 += SerialConfig + \",\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"BaudRate\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"35,\";\n            CmdWriteEthernet_Serial_00 += \"36,\";\n            CmdWriteEthernet_Serial_00 += \"1,\";\n            CmdWriteEthernet_Serial_00 += \"1,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"Interface\").value + \",\";\n            CmdWriteEthernet_Serial_00 += Val2 + \",\";\n            CmdWriteEthernet_Serial_00 += Val3 + \",\";\n            CmdWriteEthernet_Serial_00 += Val4 + \",\";\n            CmdWriteEthernet_Serial_00 += Val5 + \",\";\n            CmdWriteEthernet_Serial_00 += Val6 + \",\";\n            CmdWriteEthernet_Serial_00 += Val7 + \",\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"UDPPort\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"503,\";\n            CmdWriteEthernet_Serial_00 += \"504,\";\n            CmdWriteEthernet_Serial_00 += \"505,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"PortID\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"0,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"RemoteIP\").value + \",\";\n            CmdWriteEthernet_Serial_00 += parseInt(Bits, 2);\n            document.getElementById(\"textarea_cmd\").value = CmdWriteEthernet_Serial_00;\n       CommandStr = CmdWriteEthernet_Serial_00;document.getElementById(\"textarea_cmd\").value = CommandStr;SendRequest(CommandStr);}");
    client.println("function ReadFromController() {\n            CommandStr = \"111,000,000\";\n            const xhr = new XMLHttpRequest();\n            const url = \"http://192.168.1.110/api?message=\" + CommandStr;\n            xhr.open('GET', url, true);\n            xhr.onreadystatechange = function () {\n                if (xhr.readyState === 4 && xhr.status === 200) {\n                    const responseText = xhr.responseText;\n                    SetDataToFild(responseText);\n                }\n            };\n            xhr.send();\n            document.getElementById(\"textarea_cmd\").value = CommandStr;\n        }\n        ");
    client.println("function ResetMCU() {\n            CommandStr = \"948,001,000\";\n            document.getElementById(\"textarea_cmd\").value = CommandStr;\n            SendRequest(CommandStr);\n        }\n        ");
    client.println("function SendRequest(CommandStr) {\n            const xhr = new XMLHttpRequest();\n            const url = \"http://192.168.1.110/api?message=\" + CommandStr;\n            xhr.open('GET', url, true);\n            xhr.onreadystatechange = function () {\n                if (xhr.readyState === 4 && xhr.status === 200) {\n                    const responseText = xhr.responseText;\n                    console.log(responseText);\n                }\n            };\n            xhr.send();\n        }\n        ");
    client.println("function calcSerialConfig(sdatabit, sparity, sstopbit) {\n            let databitLong = 0x1;\n            let parityLong = 0;\n            let stopbitLong = 0x400;\n\n            let parity = 1;\n            if (sparity === \"None\") parity = 3;\n            if (sparity === \"Odd\") parity = 2;\n\n            let databit = parseInt(sdatabit);\n            let stopbit = parseInt(sstopbit);\n            stopbit = stopbit * 10;\n\n            if (parity === 1) {\n                parityLong = 0x1;\n            } else if (parity === 2) {\n                parityLong = 0x2;\n            } else if (parity === 3) {\n                parityLong = 0x3;\n            } else if (parity === 4) {\n                parityLong = 0x4;\n            } else if (parity === 5) {\n                parityLong = 0x5;\n            } else if (parity === 6) {\n                parityLong = 0xF;\n            }\n\n            if (stopbit === 10) {\n                stopbitLong = 0x10;\n            } else if (stopbit === 15) {\n                stopbitLong = 0x20;\n            } else if (stopbit === 20) {\n                stopbitLong = 0x30;\n            } else {\n                stopbitLong = 0xF0;\n            }\n\n            if (databit === 5) {\n                databitLong = 0x100;\n            } else if (databit === 6) {\n                databitLong = 0x200;\n            } else if (databit === 7) {\n                databitLong = 0x300;\n            } else if (databit === 8) {\n                databitLong = 0x400;\n            } else {\n                databitLong = 0xF00;\n            }\n\n            let config = databitLong | parityLong | stopbitLong;\n            return config;\n        }\n        ");
    client.println("function SetSerialConfig() {\n            SerialConfig = calcSerialConfig(\n                parseInt(document.getElementById(\"DataBit\").value),\n                document.getElementById(\"Parity\").value,\n                parseInt(document.getElementById(\"StopBit\").value)\n            );\n        }\n        const selectElement1 = document.getElementById('DataBit');\n        selectElement1.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n        const selectElement2 = document.getElementById('Parity');\n        selectElement2.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n        const selectElement3 = document.getElementById('StopBit');\n        selectElement3.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n\n        function ConvertDecimalToIP(decimalValue) {\n            let decimalNumber = parseInt(decimalValue);\n            let binaryString = decimalNumber.toString(2).padStart(32, '0');\n\n            let ip = new Array(4);\n            ip[3] = parseInt(binaryString.substring(24, 32), 2);\n            ip[2] = parseInt(binaryString.substring(16, 24), 2);\n            ip[1] = parseInt(binaryString.substring(8, 16), 2);\n            ip[0] = parseInt(binaryString.substring(0, 8), 2);\n\n            return ip;\n        }\n        function ConvertDecimalToMac(decimalValue) {\n            let decimalNumber = parseInt(decimalValue);\n            let binaryString = decimalNumber.toString(2).padStart(32, '0');\n\n            let macBytes = new Uint8Array(4);\n            for (let i = 0; i < 4; i++) {\n                macBytes[i] = parseInt(binaryString.substring(i * 8, (i + 1) * 8), 2);\n            }\n\n            return [macBytes[3], macBytes[2], macBytes[1], macBytes[0]];\n        }\n        ");
    client.println("function ConvertDecimalToBool(value) {\n            let binary = value.toString(2).padStart(16, '0');\n            return binary;\n        }\n        ");
    client.println("function ConvertStringToBool(value) {\n            return value === \"1\";\n        }\n        ");
    client.println("function SetDataToFild(CommResponse) {\n            var values = (CommResponse.replace(/<[^>]*>/g, '')).split(\",\");\n\n            let Temp = ConvertDecimalToIP(values[0]);\n            document.getElementById(\"StopBit\").value = Temp[3];\n            document.getElementById(\"DataBit\").value = Temp[2];\n\n            let parity = \"Even\";\n            if (Temp[1] === 3) parity = \"None\";\n            if (Temp[1] === 2) parity = \"Odd\";\n            document.getElementById(\"Parity\").value = parity;\n\n            SerialConfig = parseInt(values[1]);\n\n            document.getElementById(\"BaudRate\").value = parseInt(values[2]);\n            document.getElementById(\"Interface\").value = parseInt(values[7]);\n\n            let macBytes = ConvertDecimalToMac(values[8]);\n            document.getElementById(\"MacAddress\").value = macBytes[0] + \":\" + macBytes[1] + \":\" + macBytes[2] + \":\" + macBytes[3];\n\n            let ipAddress = ConvertDecimalToIP(values[9]);\n            document.getElementById(\"IPAddress\").value = ipAddress[3] + \".\" + ipAddress[2] + \".\" + ipAddress[1] + \".\" + ipAddress[0];\n\n            let subnetMask = ConvertDecimalToIP(values[10]);\n            document.getElementById(\"NetMask\").value = subnetMask[3] + \".\" + subnetMask[2] + \".\" + subnetMask[1] + \".\" + subnetMask[0];\n\n            let gateway = ConvertDecimalToIP(values[11]);\n            document.getElementById(\"Gateway\").value = gateway[3] + \".\" + gateway[2] + \".\" + gateway[1] + \".\" + gateway[0];\n\n            let dns1 = ConvertDecimalToIP(values[12]);\n            document.getElementById(\"PreferredDNS\").value = dns1[3] + \".\" + dns1[2] + \".\" + dns1[1] + \".\" + dns1[0];\n\n            let dns2 = ConvertDecimalToIP(values[13]);\n            document.getElementById(\"AlternateDNS\").value = dns2[3] + \".\" + dns2[2] + \".\" + dns2[1] + \".\" + dns2[0];\n\n            document.getElementById(\"UDPPort\").value = values[14];\n            document.getElementById(\"PortID\").value = values[18];\n            document.getElementById(\"RemoteIP\").value = values[20];\n");
    client.println("let bitsString = ConvertDecimalToBool(parseInt(values[21]));\nlet Enabled_Ethernet = document.getElementById(\"Enabled_Ethernet\");\n            if (ConvertStringToBool(bitsString.substring(0, 1)) == \"1\") { Enabled_Ethernet.checked = true; } else { Enabled_Ethernet.checked = false; }\n            let Enabled_Serial = document.getElementById(\"Enabled_Ethernet\");\n            if (ConvertStringToBool(bitsString.substring(1, 2)) == \"1\") { Enabled_Serial.checked = true; } else { Enabled_Serial.checked = false; }\n\n            var Operation_Mode = \"SerialServer\";\n            if (ConvertStringToBool(bitsString.substring(2, 3)) == \"1\") { Operation_Mode = \"SerialServer\"; }\n            if (ConvertStringToBool(bitsString.substring(3, 4)) == \"1\") { Operation_Mode = \"PeerToPeer\"; }\n\n            let radioButtons = document.querySelectorAll(`input[type=\"radio\"][name=\"${Operation_Mode}\"]`);\n\n            radioButtons.forEach(function (radioButton) {\n                radioButton.checked = true;\n            });\n        }\n</script>");
}

void ViewPMM0501Settings(PmmEthernetClient client)
{
    client.println("<title>PMM-Easy Config</title> <style>.footer{position:fixed;bottom:5px;color:var(--contentTextColor);text-align:center;font-size:13px;padding:2px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.footer p{display:contents}body{font-family:Arial,sans-serif;margin:0;padding:0;overflow-x:hidden}#navbar{background-color:#339898;color:#fff;padding:10px;height:44px;width:100%}");
    client.println("#leftbar,#navbar{background-color:#339898;color:#fff}#leftbar,#navbar,span{color:#fff}.button,.button2{text-align:center;text-decoration:none;display:inline-block;cursor:pointer}.ddl,.input,textarea{box-sizing:border-box}.button,.button2,.container{cursor:pointer}.footer{position:fixed;bottom:5px;color:var(--contentTextColor);text-align:center;font-size:13px;padding:2px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.footer p{display:contents}body{font-family:Arial,sans-serif;margin:0;padding:0;overflow-x:hidden}#navbar{padding:10px;height:44px;width:100%}#leftbar{width:190px;padding:10px;position:absolute;top:0;left:0;height:100%;margin-top:64px}#content{margin-left:210px;padding:20px}.content-div{background-color:#eee;padding:10px;margin-bottom:10px;height:350px}ul{list-style:none}li{padding:12px;width:92%}#div2,#div3{display:none}.button{width:40px;height:40px}.button2{width:80px;height:40px}.container,.container input:checked~.checkmark:after{display:block}.button3,.ddl,.input{height:28px;position:absolute}textarea{width:89%;height:40px;padding:8px;border:2px solid #ccc;border-radius:4px;font-size:12px;resize:none}.ddl,.input{width:10%;left:24%;border:2px solid #ccc}.button3{width:140px}.container{position:relative;font-size:16px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.container input{position:absolute;opacity:0;cursor:pointer;height:0;width:0}.checkmark{position:absolute;top:-6px;left:13.5%;height:25px;width:25px;background-color:#eee}.container:hover input~.checkmark{background-color:#ccc}.container input:checked~.checkmark{background-color:#2196f3}.checkmark:after{content:'';");
    client.println("position:absolute;display:none}.container .checkmark:after{left:9px;top:5px;width:5px;height:10px;border:solid #fff;border-width:0 3px 3px 0;-webkit-transform:rotate(45deg);-ms-trasform:rotate(45deg);transform:rotate(45deg)}.Divs.active{background-color:#fff;color:#339898}</style></head>");
    client.println("<body><div id=\"navbar\">\n        <h1 style=\"position: absolute;top: -6px;left: 26px;\">PMM</h1>\n        <h4 style=\"position: absolute;top: 7px;left: 112px;\">Easy Config 0501</h4>\n    </div>\n    <div id=\"leftbar\">\n        <ul style=\"margin-left: -10%;\">\n            <li class=\"Divs\" onclick=\"GoToDiv('div1');\" id=\"div1_btn\">\n                <span id=\"div1_span\" style=\"font-size: 14px;\"> Ethernet (Serial 00)</span>\n            </li>\n            <li class=\"Divs\" onclick=\"GoToDiv('div2');\" id=\"div2_btn\">\n                <span id=\"div2_span\" style=\"font-size: 14px;\"> Serial (Serial 00)</span>\n            </li>\n            <li class=\"Divs\" onclick=\"GoToDiv('div3');\" id=\"div3_btn\">\n                <span id=\"div3_span\" style=\"font-size: 14px;\"> Operation Mode</span>\n            </li>\n            <button class=\"button3\" onclick=\"ReadFromController();\" style=\"margin-top: 10%;\">Upload</button><br><br>\n            <button class=\"button3\" onclick=\"WriteToController();\" style=\"margin-top: 10%;\">Download</button><br><br>\n            <button class=\"button3\" onclick=\"ResetMCU();\" style=\"margin-top: 10%;\">Reset MCU</button><br><br>\n        </ul>\n\n        <div class=\"footer\">\n            <p>&#169;2014-</p><span id=\"thisYear\"> </span><br>\n            <p>PMM LLC. All rights reserved.</p><br>\n            <a style=\"text-decoration: none;color: #fff;\" href=\"https://pmm-usa.us/\">www.pmm-usa.us</a>\n        </div>\n    </div>");
    client.println("<div id='content'> <div class='content-div' id='div1'> <h2>Ethernet (Serial 00)</h2> <label class='container'>Enabled <input type='checkbox' id='Enabled_Ethernet'> <span class='checkmark'></span> </label><br><label for='IPAddress'>IP Address</label> <input type='text' class='input' id='IPAddress' name='IPAddress' value='192.168.1.100'><br><br><label for='NetMask'>Net Mask</label> <input type='text' class='input' id='NetMask' name='NetMask' value='255.255.255.0'><br><br><label for='PreferredDNS'>Preferred DNS</label> <input type='text' class='input' id='PreferredDNS' name='PreferredDNS' value='8.8.8.8'><br><br><label for='AlternateDNS'>Alternate DNS</label> <input type='text' class='input' id='AlternateDNS' name='AlternateDNS' value='8.8.4.4'><br><br><label for='Gateway'>Gateway</label> <input type='text' class='input' id='Gateway' name='Gateway' value='192.168.1.1'><br><br><label for='MacAddress'>Mac Address</label> <label for='MacAddress' style='position: absolute;left: 24%;top: 40%;'>48:7E</label> <input type='text' class='input' id='MacAddress' name='MacAddress' value='72:64:64:32' style='width: 7%;left: 27%;'><br><br><label for=\"UDPPort\" style=\"margin-right: 142px;\">UDP Port</label>\n            <input type=\"number\" class=\"input udpport\" id=\"UDPPort\" name=\"UDPPort\" style=\"\" value=\"502\">\n            <input type=\"number\" class=\"input udpport\" id=\"UDPPort2\" name=\"UDPPort2\" style=\"\" value=\"503\">\n            <input type=\"number\" class=\"input udpport\" id=\"UDPPort3\" name=\"UDPPort3\" style=\"\" value=\"504\">\n            <input type=\"number\" class=\"input udpport\" id=\"UDPPort4\" name=\"UDPPort4\" style=\"\" value=\"505\"><br><br></div>");
    client.println("<div class='content-div' id='div2'> <h2>Serial (Serial 00)</h2> <label class='container'>Enabled <input type='checkbox' id='Enabled_Serial'> <span class='checkmark'></span>  <label for=\"SerialPort\">Serial Port</label>\n            <select id=\"SerialPort\" class=\"ddl\">\n                <option value=\"Serial00\">Serial 00</option>\n                <option value=\"Serial01\">Serial 01</option>\n                <option value=\"Serial02\">Serial 02</option>\n                <option value=\"Serial03\">Serial 03</option>\n            </select><br><br></label><br><label for='BaudRate'>Baud Rate</label> <select id='BaudRate' class='ddl'> <option value='300'>300</option> <option value='600'>600</option> <option value='1200'>1200</option> <option value='2400'>2400</option> <option value='4800'>4800</option> <option value='9600'>9600</option> <option value='14400'>14400</option> <option value='19200'>19200</option> <option value='38400'>38400</option> <option value='56000'>56000</option> <option value='57600'>57600</option> <option value='115200'>115200</option> </select><br><br><label for='DataBit'>Data Bit</label> <select id='DataBit' class='ddl'> <option value='5'>5</option> <option value='6'>6</option> <option value='7'>7</option> <option value='8'>8</option> </select><br><br><label for='StopBit'>Stop Bit</label> <select id='StopBit' class='ddl'> <option value='1'>1</option> <option value='2'>2</option> </select><br><br><label for='Parity'>Parity</label> <select id='Parity' class='ddl'> <option value='None'>None</option> <option value='Odd'>Odd</option> <option value='Even'>Even</option> </select><br><br><label for='Interface'>Interface</label> <select id='Interface' class='ddl'> <option value='485'>RS485</option> <option value='422'>RS422</option> <option value='232'>RS232</option> </select><br><br>");
    client.println(" <label for='PortID'>Port ID</label> <input type='number' class='input' id='PortID' name='PortID' style='width:7%;' value='1'> <label style='margin-left: 16%;\n    position: absolute;\n    top: 43.5%;'>(1-30)</label><br><br></div>");
    client.println("<div class='content-div' id='div3'> <h2>Operation Mode</h2><input type='radio' id='SerialServer' name='Operation_Mode' value='SerialServer' checked><label for='SerialServer'>Serial Server</label><br><p style='margin-left: 3%;'>multi ethernet client can communicate with serial port on this device</p><br><input type='radio' id='PeerToPeer' name='Operation_Mode' value='PeerToPeer'><label for='PeerToPeer'>Peer To Peer</label><br><label for='RemoteIP' style='top: 6px;position: relative;margin-left: 42px;'>Remote IP</label> <label style='top: 6px;position: relative;margin-left: 7%;'>192.168.1.</label> <input type='number' min='1' class='input' id='RemoteIP' name='RemoteIP' style='width:3%;left: 29%;' value='200'><br><br></div>");
    client.println("<div class='content-div' id='div4' style='height: 75px;'> <h2>Command</h2> <div style='display: inline-block; position: absolute; margin-left: 10%; width: 80%; top: 51%;'> <textarea id='textarea_cmd'>Command...</textarea> </div></div></div>");
    client.println("<script>var d = new Date();\n        var y = d.getFullYear();\n        document.getElementById(\"thisYear\").textContent = y;\n        let CommandStr = \"\";\n        let SerialConfig = 1043;\n        GoToDiv(\"div1\");\n        ReadFromController();\n        ");
    client.println("function GoToDiv(div) {if (div == \"div1\") {document.getElementById(\"div1\").style.display = \"block\";document.getElementById(\"div2\").style.display = \"none\";document.getElementById(\"div3\").style.display = \"none\";document.getElementById(\"div1_btn\").classList.add(\"active\");document.getElementById(\"div2_btn\").classList.remove(\"active\");document.getElementById(\"div3_btn\").classList.remove(\"active\");document.getElementById(\"div1_span\").style.color = \"#339898\";document.getElementById(\"div2_span\").style.color = \"#ffffff\";document.getElementById(\"div3_span\").style.color = \"#ffffff\";}else if (div == \"div2\") {\n                document.getElementById(\"div1\").style.display = \"none\";document.getElementById(\"div2\").style.display = \"block\";document.getElementById(\"div3\").style.display = \"none\";document.getElementById(\"div1_btn\").classList.remove(\"active\");document.getElementById(\"div2_btn\").classList.add(\"active\");document.getElementById(\"div3_btn\").classList.remove(\"active\");document.getElementById(\"div1_span\").style.color = \"#ffffff\";document.getElementById(\"div2_span\").style.color = \"#339898\";document.getElementById(\"div3_span\").style.color = \"#ffffff\";\n            }\n            ");
    client.println("else if (div == \"div3\") {document.getElementById(\"div1\").style.display = \"none\";document.getElementById(\"div2\").style.display = \"none\";document.getElementById(\"div3\").style.display = \"block\";document.getElementById(\"div1_btn\").classList.remove(\"active\");document.getElementById(\"div2_btn\").classList.remove(\"active\");document.getElementById(\"div3_btn\").classList.add(\"active\");document.getElementById(\"div1_span\").style.color = \"#ffffff\";document.getElementById(\"div2_span\").style.color = \"#ffffff\";document.getElementById(\"div3_span\").style.color = \"#339898\";}}");
    client.println("function convertIPToDecimal(b1, b2, b3, b4) {\n            var binaryString =\n                b4.toString(2).padStart(8, '0') +\n                b3.toString(2).padStart(8, '0') +\n                b2.toString(2).padStart(8, '0') +\n                b1.toString(2).padStart(8, '0');\n            return parseInt(binaryString, 2).toString();\n        }\n        ");
    client.println("function convertMacToDecimal(byte1, byte2, byte3, byte4) {\n            var macBytes = [byte1, byte2, byte3, byte4];\n\n            var binaryString = \"\";\n            for (var i = 3; i >= 0; i--) {\n                binaryString += macBytes[i].toString(2).padStart(8, '0');\n            }\n\n            return parseInt(binaryString, 2).toString();\n        }\n        ");
    client.println("function numberToByte(number) {\n            if (number >= 0 && number <= 255) {\n                return number & 0xFF;\n            } else {\n                return null;\n            }\n        }\n        ");
    client.println("function convertBoolToString(val) {\n            var result = \"0\";\n            if (val !== null && val !== undefined) {\n                val = val.toString().toLowerCase();\n                if (val === \"false\" || val === \"0\") {\n                    result = \"0\";\n                } else if (val === \"true\" || val === \"1\") {\n                    result = \"1\";\n                }\n            }\n            return result;\n        }\n        ");
    client.println("function WriteToController() {\n            var sparity = document.getElementById(\"Parity\").value\n            let parity = 1;\n            if (sparity === \"None\") parity = 3;\n            if (sparity === \"Odd\") parity = 2;\n            var Val1 = convertIPToDecimal(\n                parseInt(document.getElementById(\"StopBit\").value),\n                parseInt(document.getElementById(\"DataBit\").value),\n                parseInt(parity),\n                1\n            );\n            var Val2 = convertMacToDecimal(\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[0]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[1]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[2]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[3])\n            );\n            var Val3 = convertIPToDecimal(\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[3])\n            );\n            var Val4 = convertIPToDecimal(\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[3]));");
    client.println("var Val5 = convertIPToDecimal(parseInt((document.getElementById(\"Gateway\").value).split(\".\")[0]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[1]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[2]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[3]));\n            var Val6 = convertIPToDecimal(\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[3])\n            );\n            var Val7 = convertIPToDecimal(\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[3])\n            );\n\n            SerialConfig = calcSerialConfig(\n                parseInt(document.getElementById(\"StopBit\").value),\n                document.getElementById(\"Parity\").value,\n                parseInt(document.getElementById(\"StopBit\").value)\n            );\n            var PeerToPeer = \"0\";\n            var SerialServer = \"0\";\n            var Operation_Mode = document.querySelector(\"input[name='Operation_Mode']:checked\").value;\n            if (Operation_Mode == \"PeerToPeer\") { PeerToPeer = \"1\"; }\n            if (Operation_Mode == \"SerialServer\") { SerialServer = \"1\"; }\n            var EnabledSerial = \"0\";\n            var EnabledEthernet = \"0\";\n            var Enabled_Ethernet = document.getElementById(\"Enabled_Ethernet\").checked;\n            var Enabled_Serial = document.getElementById(\"Enabled_Serial\").checked;\n            if (Enabled_Ethernet) { EnabledEthernet = \"1\"; }\n         ");
    client.println("if (Enabled_Serial) { EnabledSerial = \"1\"; }\n            var Bits = convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(PeerToPeer);Bits += convertBoolToString(SerialServer);Bits += convertBoolToString(EnabledSerial);Bits += convertBoolToString(EnabledEthernet);");
    client.println("var val = \"111\";\n            var serialport = document.getElementById(\"SerialPort\").value;\n            switch (serialport) {\n                case \"Serial00\":\n                    val = \"110\";\n                    break;\n                case \"Serial01\":\n                    val = \"112\";\n                    break;\n                case \"Serial02\":\n                    val = \"114\";\n                    break;\n                case \"Serial03\":\n                    val = \"116\";\n                    break;\n            }\n\n            var CmdWriteEthernet_Serial_00 = val+\",000,\";            CmdWriteEthernet_Serial_00 += Val1 + \",\";\n            CmdWriteEthernet_Serial_00 += SerialConfig + \",\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"BaudRate\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"35,\";\n            CmdWriteEthernet_Serial_00 += \"36,\";\n            CmdWriteEthernet_Serial_00 += \"1,\";\n            CmdWriteEthernet_Serial_00 += \"1,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"Interface\").value + \",\";\n            CmdWriteEthernet_Serial_00 += Val2 + \",\";\n            CmdWriteEthernet_Serial_00 += Val3 + \",\";\n            CmdWriteEthernet_Serial_00 += Val4 + \",\";\n            CmdWriteEthernet_Serial_00 += Val5 + \",\";\n            CmdWriteEthernet_Serial_00 += Val6 + \",\";\n            CmdWriteEthernet_Serial_00 += Val7 + \",\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"UDPPort\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"503,\";\n            CmdWriteEthernet_Serial_00 += \"504,\";\n            CmdWriteEthernet_Serial_00 += \"505,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"PortID\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"0,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"RemoteIP\").value + \",\";\n            CmdWriteEthernet_Serial_00 += parseInt(Bits, 2);\n            document.getElementById(\"textarea_cmd\").value = CmdWriteEthernet_Serial_00;\n       ");
    client.println("CommandStr = CmdWriteEthernet_Serial_00;document.getElementById(\"textarea_cmd\").value = CommandStr;SendRequest(CommandStr);}");
    client.println("function ReadFromController() {\n var val = \"111\";\n            var serialport = document.getElementById(\"SerialPort\").value;\n            switch (serialport) {\n                case \"Serial00\":\n                    val = \"111\";\n                    break;\n                case \"Serial01\":\n                    val = \"113\";\n                    break;\n                case \"Serial02\":\n                    val = \"115\";\n                    break;\n                case \"Serial03\":\n                    val = \"117\";\n                    break;\n            }\n\n            CommandStr = val + \",000,000\";\n            const xhr = new XMLHttpRequest();\n            const url = \"http://192.168.1.110/api?message=\" + CommandStr;\n            xhr.open('GET', url, true);\n            xhr.onreadystatechange = function () {\n                if (xhr.readyState === 4 && xhr.status === 200) {\n                    const responseText = xhr.responseText;\n                    SetDataToFild(responseText);\n                }\n            };\n            xhr.send();\n            document.getElementById(\"textarea_cmd\").value = CommandStr;\n        }\n        ");
    client.println("function ResetMCU() {\n            CommandStr = \"948,001,000\";\n            document.getElementById(\"textarea_cmd\").value = CommandStr;\n            SendRequest(CommandStr);\n        }\n        ");
    client.println("function SendRequest(CommandStr) {\n            const xhr = new XMLHttpRequest();\n            const url = \"http://192.168.1.110/api?message=\" + CommandStr;\n            xhr.open('GET', url, true);\n            xhr.onreadystatechange = function () {\n                if (xhr.readyState === 4 && xhr.status === 200) {\n                    const responseText = xhr.responseText;\n                    console.log(responseText);\n                }\n            };\n            xhr.send();\n        }\n        ");
    client.println("function calcSerialConfig(sdatabit, sparity, sstopbit) {\n            let databitLong = 0x1;\n            let parityLong = 0;\n            let stopbitLong = 0x400;\n\n            let parity = 1;\n            if (sparity === \"None\") parity = 3;\n            if (sparity === \"Odd\") parity = 2;\n\n            let databit = parseInt(sdatabit);\n            let stopbit = parseInt(sstopbit);\n            stopbit = stopbit * 10;\n\n            if (parity === 1) {\n                parityLong = 0x1;\n            } else if (parity === 2) {\n                parityLong = 0x2;\n            } else if (parity === 3) {\n                parityLong = 0x3;\n            } else if (parity === 4) {\n                parityLong = 0x4;\n            } else if (parity === 5) {\n                parityLong = 0x5;\n            } else if (parity === 6) {\n                parityLong = 0xF;\n            }\n\n            if (stopbit === 10) {\n                stopbitLong = 0x10;\n            } else if (stopbit === 15) {\n                stopbitLong = 0x20;\n            } else if (stopbit === 20) {\n                stopbitLong = 0x30;\n            } else {\n                stopbitLong = 0xF0;\n            }\n\n            if (databit === 5) {\n                databitLong = 0x100;\n            } else if (databit === 6) {\n                databitLong = 0x200;\n            } else if (databit === 7) {\n                databitLong = 0x300;\n            } else if (databit === 8) {\n                databitLong = 0x400;\n            } else {\n                databitLong = 0xF00;\n            }\n\n            let config = databitLong | parityLong | stopbitLong;\n            return config;\n        }\n        ");
    client.println("function SetSerialConfig() {\n            SerialConfig = calcSerialConfig(\n                parseInt(document.getElementById(\"DataBit\").value),\n                document.getElementById(\"Parity\").value,\n                parseInt(document.getElementById(\"StopBit\").value)\n            );\n        }\n        const selectElement1 = document.getElementById('DataBit');\n        selectElement1.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n        const selectElement2 = document.getElementById('Parity');\n        selectElement2.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n        const selectElement3 = document.getElementById('StopBit');\n        selectElement3.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n\n        function ConvertDecimalToIP(decimalValue) {\n            let decimalNumber = parseInt(decimalValue);\n            let binaryString = decimalNumber.toString(2).padStart(32, '0');\n\n            let ip = new Array(4);\n            ip[3] = parseInt(binaryString.substring(24, 32), 2);\n            ip[2] = parseInt(binaryString.substring(16, 24), 2);\n            ip[1] = parseInt(binaryString.substring(8, 16), 2);\n            ip[0] = parseInt(binaryString.substring(0, 8), 2);\n\n            return ip;\n        }\n        function ConvertDecimalToMac(decimalValue) {\n            let decimalNumber = parseInt(decimalValue);\n            let binaryString = decimalNumber.toString(2).padStart(32, '0');\n\n            let macBytes = new Uint8Array(4);\n            for (let i = 0; i < 4; i++) {\n                macBytes[i] = parseInt(binaryString.substring(i * 8, (i + 1) * 8), 2);\n            }\n\n            return [macBytes[3], macBytes[2], macBytes[1], macBytes[0]];\n        }\n        ");
    client.println("function ConvertDecimalToBool(value) {\n            let binary = value.toString(2).padStart(16, '0');\n            return binary;\n        }\n        ");
    client.println("function ConvertStringToBool(value) {\n            return value === \"1\";\n        }\n        ");
    client.println("function SetDataToFild(CommResponse) {\n            var values = (CommResponse.replace(/<[^>]*>/g, '')).split(\",\");\n\n            let Temp = ConvertDecimalToIP(values[0]);\n            document.getElementById(\"StopBit\").value = Temp[3];\n            document.getElementById(\"DataBit\").value = Temp[2];\n\n            let parity = \"Even\";\n            if (Temp[1] === 3) parity = \"None\";\n            if (Temp[1] === 2) parity = \"Odd\";\n            document.getElementById(\"Parity\").value = parity;\n\n            SerialConfig = parseInt(values[1]);\n\n            document.getElementById(\"BaudRate\").value = parseInt(values[2]);\n            document.getElementById(\"Interface\").value = parseInt(values[7]);\n\n            let macBytes = ConvertDecimalToMac(values[8]);\n            document.getElementById(\"MacAddress\").value = macBytes[0] + \":\" + macBytes[1] + \":\" + macBytes[2] + \":\" + macBytes[3];\n\n            let ipAddress = ConvertDecimalToIP(values[9]);\n            document.getElementById(\"IPAddress\").value = ipAddress[3] + \".\" + ipAddress[2] + \".\" + ipAddress[1] + \".\" + ipAddress[0];\n\n            let subnetMask = ConvertDecimalToIP(values[10]);\n            document.getElementById(\"NetMask\").value = subnetMask[3] + \".\" + subnetMask[2] + \".\" + subnetMask[1] + \".\" + subnetMask[0];\n\n            let gateway = ConvertDecimalToIP(values[11]);\n            document.getElementById(\"Gateway\").value = gateway[3] + \".\" + gateway[2] + \".\" + gateway[1] + \".\" + gateway[0];\n\n            let dns1 = ConvertDecimalToIP(values[12]);\n            document.getElementById(\"PreferredDNS\").value = dns1[3] + \".\" + dns1[2] + \".\" + dns1[1] + \".\" + dns1[0];\n\n            let dns2 = ConvertDecimalToIP(values[13]);\n            document.getElementById(\"AlternateDNS\").value = dns2[3] + \".\" + dns2[2] + \".\" + dns2[1] + \".\" + dns2[0];\n\n            document.getElementById(\"UDPPort\").value = values[14];\n              document.getElementById(\"UDPPort\").value = parseInt(values[14]);\n            ");
    client.println("document.getElementById(\"UDPPort2\").value = parseInt(values[15]);\n            document.getElementById(\"UDPPort3\").value = parseInt(values[16]);\n            document.getElementById(\"UDPPort4\").value = parseInt(values[17]);\n            document.getElementById(\"RemoteIP\").value = values[20];\n");
    client.println("let bitsString = ConvertDecimalToBool(parseInt(values[21]));\nlet Enabled_Ethernet = document.getElementById(\"Enabled_Ethernet\");\n            if (ConvertStringToBool(bitsString.substring(0, 1)) == \"1\") { Enabled_Ethernet.checked = true; } else { Enabled_Ethernet.checked = false; }\n            let Enabled_Serial = document.getElementById(\"Enabled_Ethernet\");\n            if (ConvertStringToBool(bitsString.substring(1, 2)) == \"1\") { Enabled_Serial.checked = true; } else { Enabled_Serial.checked = false; }\n\n            var Operation_Mode = \"SerialServer\";\n            if (ConvertStringToBool(bitsString.substring(2, 3)) == \"1\") { Operation_Mode = \"SerialServer\"; }\n            if (ConvertStringToBool(bitsString.substring(3, 4)) == \"1\") { Operation_Mode = \"PeerToPeer\"; }\n\n            let radioButtons = document.querySelectorAll(`input[type=\"radio\"][name=\"${Operation_Mode}\"]`);\n\n            radioButtons.forEach(function (radioButton) {\n                radioButton.checked = true;\n            });\n        }\n</script>");
}