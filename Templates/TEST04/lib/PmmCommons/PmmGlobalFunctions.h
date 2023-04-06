// #include "ProjectDef.h"

#ifdef PMMGlobalFunctions

#include "PmmTypes.h"

PmmExternalEEPROM PMMEprom;

struct PmmGeneralSettings PMMGENERALSETTINGS;
struct PmmModBusSettings PMMMODBUSSETTINGS;
struct PmmTCPUDPSettings PMMTCPUDPSETTINGS;
struct PmmSerialSettings PMMSERIALSETTINGS;

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

    PMMGENERALSETTINGS.Name = OUTPUT[0];
    PMMGENERALSETTINGS.TCPorRTU = stoi(OUTPUT[1]);
    PMMMODBUSSETTINGS.SlaveID = stoi(OUTPUT[2]);
    PMMTCPUDPSETTINGS.IPAddressEthOne = OUTPUT[3];
    PMMTCPUDPSETTINGS.SubnetMaskEthOne = OUTPUT[4];
    PMMTCPUDPSETTINGS.DNSOneEthOne = OUTPUT[5];
    PMMTCPUDPSETTINGS.GatewayEthOne = OUTPUT[6];
    PMMTCPUDPSETTINGS.RemoteIPAddressEthOne = OUTPUT[7];
    PMMTCPUDPSETTINGS.TimeOutConnEthOne = stoi(OUTPUT[8]);
    PMMTCPUDPSETTINGS.MaxRetriesEthOne = stoi(OUTPUT[9]);
    PMMTCPUDPSETTINGS.MacAddressEthOne = OUTPUT[10];
    PMMTCPUDPSETTINGS.UDPPortOne = stoi(OUTPUT[11]);
    PMMTCPUDPSETTINGS.UDPPortTow = stoi(OUTPUT[12]);
    PMMTCPUDPSETTINGS.UDPPortThree = stoi(OUTPUT[13]);
    PMMTCPUDPSETTINGS.UDPPortFour = stoi(OUTPUT[14]);
    PMMSERIALSETTINGS.PortOneName = OUTPUT[15];
    PMMSERIALSETTINGS.PortOneBaudRate = stoi(OUTPUT[16]);
    PMMSERIALSETTINGS.PortOneDataBits = stoi(OUTPUT[17]);
    PMMSERIALSETTINGS.PortOneStopBits = stoi(OUTPUT[18]);
    PMMSERIALSETTINGS.PortOneParity = OUTPUT[19];
    bool PortOneInterface = false;
    if (OUTPUT[20] == "true")
        PortOneInterface = true;
    else
        PortOneInterface = false;
    PMMSERIALSETTINGS.PortOneInterface = PortOneInterface;
    PMMSERIALSETTINGS.PortOneConnTimeOut = stoi(OUTPUT[21]);
    PMMSERIALSETTINGS.PortOneMaxRetries = stoi(OUTPUT[22]);
    PMMGENERALSETTINGS.SerialNumber = stoi(OUTPUT[23]);
    PMMGENERALSETTINGS.FirmwareVersion = OUTPUT[24];
    PMMGENERALSETTINGS.HardwareVersion = OUTPUT[25];
    bool WebServer = false;
    if (OUTPUT[26] == "true")
        WebServer = true;
    else
        WebServer = false;
    PMMGENERALSETTINGS.WebServer = WebServer;

    
    //}
}

string PMMGetUSBConfigurationSettings()
{

    PMMGetDeviceSettingsEProm();

    string DeviceSettings = "EEPROM SETTINGS : " + PMMGENERALSETTINGS.Name + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMGENERALSETTINGS.TCPorRTU) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMMODBUSSETTINGS.SlaveID) + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDPSETTINGS.IPAddressEthOne + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDPSETTINGS.SubnetMaskEthOne + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDPSETTINGS.DNSOneEthOne + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDPSETTINGS.GatewayEthOne + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDPSETTINGS.RemoteIPAddressEthOne + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDPSETTINGS.TimeOutConnEthOne) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDPSETTINGS.MaxRetriesEthOne) + ",";
    DeviceSettings = DeviceSettings + PMMTCPUDPSETTINGS.MacAddressEthOne + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDPSETTINGS.UDPPortOne) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDPSETTINGS.UDPPortTow) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDPSETTINGS.UDPPortThree) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMTCPUDPSETTINGS.UDPPortFour) + ",";
    DeviceSettings = DeviceSettings + PMMSERIALSETTINGS.PortOneName + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIALSETTINGS.PortOneBaudRate) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIALSETTINGS.PortOneDataBits) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIALSETTINGS.PortOneStopBits) + ",";
    DeviceSettings = DeviceSettings + PMMSERIALSETTINGS.PortOneParity + ",";
    DeviceSettings = DeviceSettings + (PMMSERIALSETTINGS.PortOneInterface ? "true" : "false") + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIALSETTINGS.PortOneConnTimeOut) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMSERIALSETTINGS.PortOneMaxRetries) + ",";
    DeviceSettings = DeviceSettings + std::to_string(PMMGENERALSETTINGS.SerialNumber) + ",";
    DeviceSettings = DeviceSettings + PMMGENERALSETTINGS.FirmwareVersion + ",";
    DeviceSettings = DeviceSettings + PMMGENERALSETTINGS.HardwareVersion + ",";
    DeviceSettings = DeviceSettings + (PMMGENERALSETTINGS.WebServer ? "true" : "false");

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
        PMMEprom.put(0, PMMGENERALSETTINGS.Name);
        PMMEprom.put(4, PMMGENERALSETTINGS.TCPorRTU);
        PMMEprom.put(8, PMMMODBUSSETTINGS.SlaveID);
        PMMEprom.put(12, PMMTCPUDPSETTINGS.IPAddressEthOne);
        PMMEprom.put(16, PMMTCPUDPSETTINGS.SubnetMaskEthOne);
        PMMEprom.put(20, PMMTCPUDPSETTINGS.DNSOneEthOne);
        PMMEprom.put(24, PMMTCPUDPSETTINGS.GatewayEthOne);
        PMMEprom.put(28, PMMTCPUDPSETTINGS.RemoteIPAddressEthOne);
        PMMEprom.put(32, PMMTCPUDPSETTINGS.TimeOutConnEthOne);
        PMMEprom.put(36, PMMTCPUDPSETTINGS.MaxRetriesEthOne);
        PMMEprom.put(40, PMMTCPUDPSETTINGS.MacAddressEthOne);
        PMMEprom.put(44, PMMTCPUDPSETTINGS.UDPPortOne);
        PMMEprom.put(48, PMMTCPUDPSETTINGS.UDPPortTow);
        PMMEprom.put(52, PMMTCPUDPSETTINGS.UDPPortThree);
        PMMEprom.put(56, PMMTCPUDPSETTINGS.UDPPortFour);
        PMMEprom.put(60, PMMSERIALSETTINGS.PortOneName);
        PMMEprom.put(64, PMMSERIALSETTINGS.PortOneBaudRate);
        PMMEprom.put(68, PMMSERIALSETTINGS.PortOneDataBits);
        PMMEprom.put(72, PMMSERIALSETTINGS.PortOneStopBits);
        PMMEprom.put(76, PMMSERIALSETTINGS.PortOneParity);
        PMMEprom.put(80, PMMSERIALSETTINGS.PortOneInterface);
        PMMEprom.put(84, PMMSERIALSETTINGS.PortOneConnTimeOut);
        PMMEprom.put(88, PMMSERIALSETTINGS.PortOneMaxRetries);
        PMMEprom.put(92, PMMGENERALSETTINGS.SerialNumber);
        PMMEprom.put(96, PMMGENERALSETTINGS.FirmwareVersion);
        PMMEprom.put(100, PMMGENERALSETTINGS.HardwareVersion);
        PMMEprom.put(104, PMMGENERALSETTINGS.WebServer);
    }
}

void PMMGetDeviceSettingsEProm()
{

    PMMEprom.get(0, PMMGENERALSETTINGS.Name);
    PMMEprom.get(4, PMMGENERALSETTINGS.TCPorRTU);
    PMMEprom.get(8, PMMMODBUSSETTINGS.SlaveID);
    PMMEprom.get(12, PMMTCPUDPSETTINGS.IPAddressEthOne);
    PMMEprom.get(16, PMMTCPUDPSETTINGS.SubnetMaskEthOne);
    PMMEprom.get(20, PMMTCPUDPSETTINGS.DNSOneEthOne);
    PMMEprom.get(24, PMMTCPUDPSETTINGS.GatewayEthOne);
    PMMEprom.get(28, PMMTCPUDPSETTINGS.RemoteIPAddressEthOne);
    PMMEprom.get(32, PMMTCPUDPSETTINGS.TimeOutConnEthOne);
    PMMEprom.get(36, PMMTCPUDPSETTINGS.MaxRetriesEthOne);
    PMMEprom.get(40, PMMTCPUDPSETTINGS.MacAddressEthOne);
    PMMEprom.get(44, PMMTCPUDPSETTINGS.UDPPortOne);
    PMMEprom.get(48, PMMTCPUDPSETTINGS.UDPPortTow);
    PMMEprom.get(52, PMMTCPUDPSETTINGS.UDPPortThree);
    PMMEprom.get(56, PMMTCPUDPSETTINGS.UDPPortFour);
    PMMEprom.get(60, PMMSERIALSETTINGS.PortOneName);
    PMMEprom.get(64, PMMSERIALSETTINGS.PortOneBaudRate);
    PMMEprom.get(68, PMMSERIALSETTINGS.PortOneDataBits);
    PMMEprom.get(72, PMMSERIALSETTINGS.PortOneStopBits);
    PMMEprom.get(76, PMMSERIALSETTINGS.PortOneParity);
    PMMEprom.get(80, PMMSERIALSETTINGS.PortOneInterface);
    PMMEprom.get(84, PMMSERIALSETTINGS.PortOneConnTimeOut);
    PMMEprom.get(88, PMMSERIALSETTINGS.PortOneMaxRetries);
    PMMEprom.get(92, PMMGENERALSETTINGS.SerialNumber);
    PMMEprom.get(96, PMMGENERALSETTINGS.FirmwareVersion);
    PMMEprom.get(100, PMMGENERALSETTINGS.HardwareVersion);
    PMMEprom.get(104, PMMGENERALSETTINGS.WebServer);
}


// void PMMSetDeviceSettingsIntoFlash()
// {
//     if (PMMEprom.begin() == true)
//     {
//         PMMEprom.put(0, PMMGENERALSETTINGS.Name);
//         PMMEprom.put(4, PMMGENERALSETTINGS.TCPorRTU);
//         PMMEprom.put(8, PMMMODBUSSETTINGS.SlaveID);
//         PMMEprom.put(12, PMMTCPUDPSETTINGS.IPAddressEthOne);
//         PMMEprom.put(16, PMMTCPUDPSETTINGS.SubnetMaskEthOne);
//         PMMEprom.put(20, PMMTCPUDPSETTINGS.DNSOneEthOne);
//         PMMEprom.put(24, PMMTCPUDPSETTINGS.GatewayEthOne);
//         PMMEprom.put(28, PMMTCPUDPSETTINGS.RemoteIPAddressEthOne);
//         PMMEprom.put(32, PMMTCPUDPSETTINGS.TimeOutConnEthOne);
//         PMMEprom.put(36, PMMTCPUDPSETTINGS.MaxRetriesEthOne);
//         PMMEprom.put(40, PMMTCPUDPSETTINGS.MacAddressEthOne);
//         PMMEprom.put(44, PMMTCPUDPSETTINGS.UDPPortOne);
//         PMMEprom.put(48, PMMTCPUDPSETTINGS.UDPPortTow);
//         PMMEprom.put(52, PMMTCPUDPSETTINGS.UDPPortThree);
//         PMMEprom.put(56, PMMTCPUDPSETTINGS.UDPPortFour);
//         PMMEprom.put(60, PMMSERIALSETTINGS.PortOneName);
//         PMMEprom.put(64, PMMSERIALSETTINGS.PortOneBaudRate);
//         PMMEprom.put(68, PMMSERIALSETTINGS.PortOneDataBits);
//         PMMEprom.put(72, PMMSERIALSETTINGS.PortOneStopBits);
//         PMMEprom.put(76, PMMSERIALSETTINGS.PortOneParity);
//         PMMEprom.put(80, PMMSERIALSETTINGS.PortOneInterface);
//         PMMEprom.put(84, PMMSERIALSETTINGS.PortOneConnTimeOut);
//         PMMEprom.put(88, PMMSERIALSETTINGS.PortOneMaxRetries);
//         PMMEprom.put(92, PMMGENERALSETTINGS.SerialNumber);
//         PMMEprom.put(96, PMMGENERALSETTINGS.FirmwareVersion);
//         PMMEprom.put(100, PMMGENERALSETTINGS.HardwareVersion);
//         PMMEprom.put(104, PMMGENERALSETTINGS.WebServer);
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