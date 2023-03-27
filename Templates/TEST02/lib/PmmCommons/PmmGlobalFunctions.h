// #include "ProjectDef.h"

#ifdef PMMGlobalFunctions

#include "PmmTypes.h"

PmmExternalEEPROM PMMEprom;

struct PmmPLC PMMPLC;
struct PmmModBus PMMMODBUS;
struct PmmTCPUDP PMMTCPUDP;
struct PmmSerial PMMSERIAL;

extern void PMMSetUSBConfigurationSettings(string Settings);
extern string PMMGetUSBConfigurationSettings();

extern void PMMSetDeviceSettingsEProm();
extern void PMMGetDeviceSettingsEProm();

extern string PMMIsAlive();
extern std::array<string, 100> PMMSplitString(string parameter, std::array<string, 100> OutputArray);
extern void PMMSplitString2(string parameter, string &returnVal, string &returnstring);
extern string ConvertUint8_tTostring(uint8_t number);
extern string ConvertUint16_tTostring(uint16_t number);
extern string ConvertUint32_tTostring(uint32_t number);
extern bool PMMCheckExternalRTC();
extern bool PMMCheckEEPROM();
extern string CheckAvailabeHardware();
extern string PMMSplitAddreess(string sentence);
extern string ConvertIntTostring(int number);
extern string PMMGetRequest(string url);
extern string PMMGetJSONRequest(string url);
extern string PMMPostRequest(string url, string Databody);
extern string PMMPostJSONRequest(string url, string JSONDATA);
extern void InitializeEthernet();
extern IPAddress PMMGetLocalIP();
extern void PMMInitializeEEPROM();
extern unsigned int EEPROMLength();
extern void InitializeWire();

// #pragma region CONGIGRATION FUNCTIONS

void InitializeWire()
{
    Wire.begin();
}

void PMMInitializeEEPROM()
{
#ifdef PMMEEPROM
    PMMEprom.begin();
#endif

}

// 0620,0,1,192.168.1.200,255.255.255.0,8.8.8.0,192.168.1.1,192.168.1.201,1000,5,00-B0-D0-63-C2-26,91,92,93,94,COM1,9600,8,1,None,true,1000,5,18101995,1,1,false\n
void PMMSetUSBConfigurationSettings(string Settings)
{
    // if (SerialUSB.available() > 0)
    // {
    //     String SerialData = SerialUSB.readStringUntil('\n');
    // String SerialData = "0620,0,1,192.168.1.200,255.255.255.0,8.8.8.0,192.168.1.1,192.168.1.201,1000,5,00-B0-D0-63-C2-26,91,92,93,94,COM1,9600,8,1,None,true,1000,5,18101995,1,1,false\n";
    // string Settings = settings;
    std::array<string, 100> OUTPUT;
    OUTPUT = PMMSplitString(Settings, OUTPUT);

    PMMPLC.Name = OUTPUT[0];
    PMMPLC.TCPorRTU = stoi(OUTPUT[1]);
    PMMMODBUS.SlaveID = stoi(OUTPUT[2]);
    PMMTCPUDP.IPAddressEthOne = OUTPUT[3];
    PMMTCPUDP.SubnetMaskEthOne = OUTPUT[4];
    PMMTCPUDP.DNSOneEthOne = OUTPUT[5];
    PMMTCPUDP.GatewayEthOne = OUTPUT[6];
    PMMTCPUDP.RemoteIPAddressEthOne = OUTPUT[7];
    PMMTCPUDP.TimeOutConnEthOne = stoi(OUTPUT[8]);
    PMMTCPUDP.MaxRetriesEthOne = stoi(OUTPUT[9]);
    PMMTCPUDP.MacAddressEthOne = OUTPUT[10];
    PMMTCPUDP.UDPPortOne = stoi(OUTPUT[11]);
    PMMTCPUDP.UDPPortTow = stoi(OUTPUT[12]);
    PMMTCPUDP.UDPPortThree = stoi(OUTPUT[13]);
    PMMTCPUDP.UDPPortFour = stoi(OUTPUT[14]);
    PMMSERIAL.PortOneName = OUTPUT[15];
    PMMSERIAL.PortOneBaudRate = stoi(OUTPUT[16]);
    PMMSERIAL.PortOneDataBits = stoi(OUTPUT[17]);
    PMMSERIAL.PortOneStopBits = stoi(OUTPUT[18]);
    PMMSERIAL.PortOneParity = OUTPUT[19];
    bool PortOneInterface = false;
    if (OUTPUT[20] == "true")
        PortOneInterface = true;
    else
        PortOneInterface = false;
    PMMSERIAL.PortOneInterface = PortOneInterface;
    PMMSERIAL.PortOneConnTimeOut = stoi(OUTPUT[21]);
    PMMSERIAL.PortOneMaxRetries = stoi(OUTPUT[22]);
    PMMPLC.SerialNumber = stoi(OUTPUT[23]);
    PMMPLC.FirmwareVersion = OUTPUT[24];
    PMMPLC.HardwareVersion = OUTPUT[25];
    bool WebServer = false;
    if (OUTPUT[26] == "true")
        WebServer = true;
    else
        WebServer = false;
    PMMPLC.WebServer = WebServer;

    
    //}
}

string PMMGetUSBConfigurationSettings()
{

    PMMGetDeviceSettingsEProm();

    string DeviceSettings = "EEPROM SETTINGS : " + PMMPLC.Name + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMPLC.TCPorRTU) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMMODBUS.SlaveID) + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDP.IPAddressEthOne + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDP.SubnetMaskEthOne + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDP.DNSOneEthOne + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDP.GatewayEthOne + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDP.RemoteIPAddressEthOne + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDP.TimeOutConnEthOne) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDP.MaxRetriesEthOne) + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDP.MacAddressEthOne + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDP.UDPPortOne) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDP.UDPPortTow) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDP.UDPPortThree) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDP.UDPPortFour) + ",";
    DeviceSettings = DeviceSettings + PMMSERIAL.PortOneName + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIAL.PortOneBaudRate) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIAL.PortOneDataBits) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIAL.PortOneStopBits) + ",";
    DeviceSettings = DeviceSettings + PMMSERIAL.PortOneParity + ",";
    DeviceSettings = DeviceSettings + (PMMSERIAL.PortOneInterface ? "true" : "false") + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIAL.PortOneConnTimeOut) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIAL.PortOneMaxRetries) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMPLC.SerialNumber) + ",";
    DeviceSettings = DeviceSettings + PMMPLC.FirmwareVersion + ",";
    DeviceSettings = DeviceSettings + PMMPLC.HardwareVersion + ",";
    DeviceSettings = DeviceSettings + (PMMPLC.WebServer ? "true" : "false");

    return DeviceSettings;
}

unsigned int EEPROMLength()
{
    return PMMEprom.length();
}

string PMMIsAlive()
{
    return "PMMAlive";
}

void PMMSetDeviceSettingsEProm()
{
    if (PMMEprom.begin() == true)
    {
        PMMEprom.put(0, PMMPLC.Name);
        PMMEprom.put(4, PMMPLC.TCPorRTU);
        PMMEprom.put(8, PMMMODBUS.SlaveID);
        PMMEprom.put(12, PMMTCPUDP.IPAddressEthOne);
        PMMEprom.put(16, PMMTCPUDP.SubnetMaskEthOne);
        PMMEprom.put(20, PMMTCPUDP.DNSOneEthOne);
        PMMEprom.put(24, PMMTCPUDP.GatewayEthOne);
        PMMEprom.put(28, PMMTCPUDP.RemoteIPAddressEthOne);
        PMMEprom.put(32, PMMTCPUDP.TimeOutConnEthOne);
        PMMEprom.put(36, PMMTCPUDP.MaxRetriesEthOne);
        PMMEprom.put(40, PMMTCPUDP.MacAddressEthOne);
        PMMEprom.put(44, PMMTCPUDP.UDPPortOne);
        PMMEprom.put(48, PMMTCPUDP.UDPPortTow);
        PMMEprom.put(52, PMMTCPUDP.UDPPortThree);
        PMMEprom.put(56, PMMTCPUDP.UDPPortFour);
        PMMEprom.put(60, PMMSERIAL.PortOneName);
        PMMEprom.put(64, PMMSERIAL.PortOneBaudRate);
        PMMEprom.put(68, PMMSERIAL.PortOneDataBits);
        PMMEprom.put(72, PMMSERIAL.PortOneStopBits);
        PMMEprom.put(76, PMMSERIAL.PortOneParity);
        PMMEprom.put(80, PMMSERIAL.PortOneInterface);
        PMMEprom.put(84, PMMSERIAL.PortOneConnTimeOut);
        PMMEprom.put(88, PMMSERIAL.PortOneMaxRetries);
        PMMEprom.put(92, PMMPLC.SerialNumber);
        PMMEprom.put(96, PMMPLC.FirmwareVersion);
        PMMEprom.put(100, PMMPLC.HardwareVersion);
        PMMEprom.put(104, PMMPLC.WebServer);
    }
}

void PMMGetDeviceSettingsEProm()
{

    PMMEprom.get(0, PMMPLC.Name);
    PMMEprom.get(4, PMMPLC.TCPorRTU);
    PMMEprom.get(8, PMMMODBUS.SlaveID);
    PMMEprom.get(12, PMMTCPUDP.IPAddressEthOne);
    PMMEprom.get(16, PMMTCPUDP.SubnetMaskEthOne);
    PMMEprom.get(20, PMMTCPUDP.DNSOneEthOne);
    PMMEprom.get(24, PMMTCPUDP.GatewayEthOne);
    PMMEprom.get(28, PMMTCPUDP.RemoteIPAddressEthOne);
    PMMEprom.get(32, PMMTCPUDP.TimeOutConnEthOne);
    PMMEprom.get(36, PMMTCPUDP.MaxRetriesEthOne);
    PMMEprom.get(40, PMMTCPUDP.MacAddressEthOne);
    PMMEprom.get(44, PMMTCPUDP.UDPPortOne);
    PMMEprom.get(48, PMMTCPUDP.UDPPortTow);
    PMMEprom.get(52, PMMTCPUDP.UDPPortThree);
    PMMEprom.get(56, PMMTCPUDP.UDPPortFour);
    PMMEprom.get(60, PMMSERIAL.PortOneName);
    PMMEprom.get(64, PMMSERIAL.PortOneBaudRate);
    PMMEprom.get(68, PMMSERIAL.PortOneDataBits);
    PMMEprom.get(72, PMMSERIAL.PortOneStopBits);
    PMMEprom.get(76, PMMSERIAL.PortOneParity);
    PMMEprom.get(80, PMMSERIAL.PortOneInterface);
    PMMEprom.get(84, PMMSERIAL.PortOneConnTimeOut);
    PMMEprom.get(88, PMMSERIAL.PortOneMaxRetries);
    PMMEprom.get(92, PMMPLC.SerialNumber);
    PMMEprom.get(96, PMMPLC.FirmwareVersion);
    PMMEprom.get(100, PMMPLC.HardwareVersion);
    PMMEprom.get(104, PMMPLC.WebServer);
}


// void PMMSetDeviceSettingsIntoFlash()
// {
//     if (PMMEprom.begin() == true)
//     {
//         PMMEprom.put(0, PMMPLC.Name);
//         PMMEprom.put(4, PMMPLC.TCPorRTU);
//         PMMEprom.put(8, PMMMODBUS.SlaveID);
//         PMMEprom.put(12, PMMTCPUDP.IPAddressEthOne);
//         PMMEprom.put(16, PMMTCPUDP.SubnetMaskEthOne);
//         PMMEprom.put(20, PMMTCPUDP.DNSOneEthOne);
//         PMMEprom.put(24, PMMTCPUDP.GatewayEthOne);
//         PMMEprom.put(28, PMMTCPUDP.RemoteIPAddressEthOne);
//         PMMEprom.put(32, PMMTCPUDP.TimeOutConnEthOne);
//         PMMEprom.put(36, PMMTCPUDP.MaxRetriesEthOne);
//         PMMEprom.put(40, PMMTCPUDP.MacAddressEthOne);
//         PMMEprom.put(44, PMMTCPUDP.UDPPortOne);
//         PMMEprom.put(48, PMMTCPUDP.UDPPortTow);
//         PMMEprom.put(52, PMMTCPUDP.UDPPortThree);
//         PMMEprom.put(56, PMMTCPUDP.UDPPortFour);
//         PMMEprom.put(60, PMMSERIAL.PortOneName);
//         PMMEprom.put(64, PMMSERIAL.PortOneBaudRate);
//         PMMEprom.put(68, PMMSERIAL.PortOneDataBits);
//         PMMEprom.put(72, PMMSERIAL.PortOneStopBits);
//         PMMEprom.put(76, PMMSERIAL.PortOneParity);
//         PMMEprom.put(80, PMMSERIAL.PortOneInterface);
//         PMMEprom.put(84, PMMSERIAL.PortOneConnTimeOut);
//         PMMEprom.put(88, PMMSERIAL.PortOneMaxRetries);
//         PMMEprom.put(92, PMMPLC.SerialNumber);
//         PMMEprom.put(96, PMMPLC.FirmwareVersion);
//         PMMEprom.put(100, PMMPLC.HardwareVersion);
//         PMMEprom.put(104, PMMPLC.WebServer);
//     }
// }


string CheckAvailabeHardware()
{
    string result = "";

    // Read Data From USB
    if (SerialUSB.available() > 0)
    {
        string Hardware = std::string((SerialUSB.readString()).c_str());
        if (Hardware == "InternalRTC")
        {
            // bool InternalRTCBool = PMMCheckInternalRTC();
            // result = InternalRTCBool ? "found" : "not found";
        }
    }

    return result;
}

// #pragma endregion

// #pragma region Check Hardware

bool PMMCheckExternalRTC()
{
    Wire.begin();
    Wire.beginTransmission(0x68);
    bool RTCFound = Wire.endTransmission() == 0;
    return RTCFound;
}

bool PMMCheckEEPROM()
{
    Wire.begin();
    Wire.beginTransmission(0x50);
    bool EEPROMFound = Wire.endTransmission() == 0;
    return EEPROMFound;
}

// #pragma endregion

// #pragma region STRINGS FUNCTIONS

std::array<string, 100> PMMSplitString(string parameter, std::array<string, 100> OutputArray)
{
    for (int i = 0; i < 100; i++)
    {
        string pattern = ",";
        // int index = indexof(parameter, pattern);
        int index = parameter.find(pattern);
        if (index > 0)
        {
            string returnVal = parameter.substr(0, index);
            OutputArray[i] = returnVal;
            parameter = parameter.substr((returnVal.length()) + 1, (parameter.length()));
        }
        else
        {
            OutputArray[i] = "NON";
        }
    }

    return OutputArray;
}

void PMMSplitString2(string parameter, string &returnVal, string &returnstring)
{
    string pattern = ",";
    // int index = indexof(parameter, pattern);
    int index = parameter.find(pattern);
    returnVal = parameter.substr(0, index);
    returnstring = parameter.substr((returnVal.length()) + 1, (parameter.length()));
}

string PMMSplitAddreess(string sentence)
{
    istringstream iss(sentence);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, '.'))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    return token;
}

string ConvertIntTostring(int number)
{
    std::string str = std::to_string(number);
    return str;
}

string ConvertUint8_tTostring(uint8_t number)
{
    std::string str = std::to_string(number);
    return str;
}

string ConvertUint16_tTostring(uint16_t number)
{
    std::string str = std::to_string(number);
    return str;
}

string ConvertUint32_tTostring(uint32_t number)
{
    std::string str = std::to_string(number);
    return str;
}

String ConvertStanderstringToString(string input)
{
    return input.c_str();
}

uint32_t GetUnixDateTimeNow()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    return now_c;
}

string DateTimeToString(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    string str = "";

    str += to_string((int)year) + "-" + to_string((int)month) + "-" + to_string((int)day) + "  " + to_string((int)hour) + ":" + to_string((int)min) + ":" + to_string((int)sec);
    return str;
}

// #pragma endregion

// #pragma region CHECK DEVICE USING DEBUG PRINT

void Debugprintln(string toPrint)
{
    String PrintData = String(toPrint.c_str());
    if (true)
        SerialUSB.println(PrintData);
}

void Debugprint(string toPrint)
{
    String PrintData = String(toPrint.c_str());
    if (true)
        SerialUSB.print(PrintData);
}

// #pragma endregion

// #pragma region HTTP FUNCTIONS

// void InitializeEthernet(byte mac[],IPAddress ip)
// {
//     Ethernet.init(10);
//     Ethernet.begin(mac, ip);
//     server.begin();
// }

IPAddress PMMGetLocalIP()
{
    return Ethernet.localIP();
}

// #pragma endregion

#endif