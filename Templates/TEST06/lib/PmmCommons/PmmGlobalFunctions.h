// #include "ProjectDef.h"

#ifndef PMMGLOBALFUNCTIONS_H
#define PMMGLOBALFUNCTIONS_H

#include "PmmTypes.h"

extern void PMMSetUSBConfigurationSettings(string Settings);
extern string PMMGetUSBConfigurationSettings();

extern void PMMSetDeviceSettingsEProm();
extern void PMMGetDeviceSettingsEProm();

extern void PmmInitializeProjectSettings();

// extern string PMMIsAlive();
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



void InitializeWire()
{
    Wire.begin();
}






void PmmInitializeProjectSettings()
{
    PmmReadAllSettingsInternalFlash();

    int Ref = ThisProduct.PmmGeneral.SettingsRef;
    if (Ref == 1)
    {
        PmmReadAllSettingsInternalFlash();
    } // Read Settings From Internal Flash

    if (Ref == 2)
    {
    } // Read Settings From External Flash

    if (Ref == 3)
    {
        PmmReadAllSettingsEEPROM();
    } // Read Settings From EEPROM
}

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