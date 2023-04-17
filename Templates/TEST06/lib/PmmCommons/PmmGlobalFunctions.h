// #include "ProjectDef.h"

#ifndef PMMGLOBALFUNCTIONS_H
#define PMMGLOBALFUNCTIONS_H

#include <PmmTypes.h>

extern void PmmInitializeProjectSettings();
void PMMInitializeEthernet();
// extern void PmmSetEthernetSettings();

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

byte mac[] = {98, 43, 60, 28, 40, 14};
IPAddress ip(192, 168, 1, 100);

/*****************************************************************
 * Ethernet functions
 * **************************************************************/

string PmmReturnConfif(int databit, int parity, int stopbit)
{
    string config = "SERIAL_8N1";
    string Parity = "N";

    switch (parity)
    {
    case 1:
        Parity = "N";
        break;
    case 2:
        Parity = "E";
        break;
    case 3:
        Parity = "O";
        break;
    case 4:
        Parity = "M";
        break;
    case 5:
        Parity = "S";
        break;
    }

    config = "SERIAL_" + ConvertIntTostring(databit) + Parity + ConvertIntTostring(stopbit);
    return config;
}

void PMMInitializeEthernet()
{

    // read settings
    byte mac1 = (byte)ThisProduct.PmmTCPUDP.MacAddress01;
    byte mac2 = (byte)ThisProduct.PmmTCPUDP.MacAddress02;
    byte mac3 = (byte)ThisProduct.PmmTCPUDP.MacAddress03;
    byte mac4 = (byte)ThisProduct.PmmTCPUDP.MacAddress04;
    byte mac5 = (byte)ThisProduct.PmmTCPUDP.MacAddress05;
    byte mac6 = (byte)ThisProduct.PmmTCPUDP.MacAddress06;

    int ip1 = ThisProduct.PmmTCPUDP.IPAddress01;
    int ip2 = ThisProduct.PmmTCPUDP.IPAddress02;
    int ip3 = ThisProduct.PmmTCPUDP.IPAddress03;
    int ip4 = ThisProduct.PmmTCPUDP.IPAddress04;

    //mac[] = {mac1, mac2, mac3, mac4, mac5, mac6};

    mac[0] = mac1;
    mac[1] = mac2;
    mac[2] = mac3;
    mac[3] = mac4;
    mac[4] = mac5;
    mac[5] = mac6;
    
    IPAddress ip(ip1, ip2, ip3, ip4);

    // try to start
    ThisProduct.EthernetRunning = false;

    if (ThisProduct.PmmGeneral.ItHasEthernet)
    {
        Ethernet.init(10);
        Ethernet.begin(mac, ip);
        ThisProduct.EthernetRunning = true;
    }
    else
    {
        SerialUSB.println("No Ethernet found ..");
    }
}

void InitializeWire()
{
    Wire.begin();
}

void PmmInitializeProjectSettings()
{
    // STEP01 : Read All settings from ROM into "ThisProduct" struct
    PmmReadAllSettingsInternalFlash();

    // STEP02 : Select ROM source for Settings , default is the Internal flash
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

    // STEP03: Initialize needed Modules
    // 1. WatchDog 32s
    PmmWatchDoggy.setup(WDT_SOFTCYCLE32S);

    // 3. Wire
    InitializeWire();
    // 4. EEprom
    ThisProduct.I2CRunning = false;
    if (ThisProduct.PmmGeneral.ItHasExtEEPROM == true)
    {
        StartEEprom();
        ThisProduct.I2CRunning = true;
    }

    // 5. Ethernet
    if (ThisProduct.PmmGeneral.ItHasEthernet == true)
    {
        PMMInitializeEthernet();
    }

    // 6. Protocols : a. modbus

    if (ThisProduct.PmmGeneral.IsModBus)
    {
        if (ThisProduct.PmmModbus.ModBusTCP)
        {
            if (ThisProduct.PmmModbus.ModBusMaster)
            {
                //PmmModbus.PMMmodbusTCPServerSetup();
            }

            if (ThisProduct.PmmModbus.ModBusSlave)
            {
            }
        }

        if (ThisProduct.PmmModbus.ModBusRTU)
        {
            if (ThisProduct.PmmModbus.ModBusMaster)
            {
                u_int16_t config = stoi(PmmReturnConfif(
                    ThisProduct.PmmModbus.DataBitConfig,
                    ThisProduct.PmmModbus.ParityConfig,
                    ThisProduct.PmmModbus.StopBitConfig));

                PmmModbus.PMMModBUSRTUClientSetup(
                    config,
                    ThisProduct.PmmModbus.BaudRate,
                    ThisProduct.PmmModbus.TXPin,
                    ThisProduct.PmmModbus.RXPin,
                    ThisProduct.PmmModbus.SerialSelectionPin,
                    ThisProduct.PmmModbus.SerialPort);
            }

            if (ThisProduct.PmmModbus.ModBusSlave)
            {
                u_int16_t config = stoi(PmmReturnConfif(
                    ThisProduct.PmmModbus.DataBitConfig,
                    ThisProduct.PmmModbus.ParityConfig,
                    ThisProduct.PmmModbus.StopBitConfig));

                PmmModbus.PMMModBUSRTUServerSetup(
                    ThisProduct.PmmModbus.SlaveID,
                    config,
                    ThisProduct.PmmModbus.BaudRate,
                    ThisProduct.PmmModbus.TXPin,
                    ThisProduct.PmmModbus.RXPin,
                    ThisProduct.PmmModbus.SerialSelectionPin,
                    ThisProduct.PmmModbus.SerialPort);

                PmmModbus.PMMModBUSRTUServerconfigure(
                    ThisProduct.PmmModbus.CoilsStatus,
                    ThisProduct.PmmModbus.StartingAddressCoilsStatus,
                    ThisProduct.PmmModbus.QuantityCoilsStatus,
                    ThisProduct.PmmModbus.InputStatus,
                    ThisProduct.PmmModbus.StartingAddressInputStatus,
                    ThisProduct.PmmModbus.QuantityInputStatus,
                    ThisProduct.PmmModbus.HoldingRegisters,
                    ThisProduct.PmmModbus.StartingAddressHoldingRegisters,
                    ThisProduct.PmmModbus.QuantityHoldingRegisters,
                    ThisProduct.PmmModbus.InputRegisters,
                    ThisProduct.PmmModbus.StartingAddressInputRegisters,
                    ThisProduct.PmmModbus.QuantityInputRegisters);
            }
        }
    }

    // STEP LAST ONE: Start General services

    Scheduler.startLoop(PMMConfiguration);
    Scheduler.startLoop(PMMCommunication);
    Scheduler.startLoop(PMMTimers);
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
    if (ThisProduct.PmmGeneral.Canprint)
        SerialUSB.println(PrintData);
}

void Debugprint(string toPrint)
{
    String PrintData = String(toPrint.c_str());
    if (ThisProduct.PmmGeneral.Canprint)
        SerialUSB.print(PrintData);
}

// #pragma endregion

// #pragma region HTTP FUNCTIONS

IPAddress PMMGetLocalIP()
{
    return Ethernet.localIP();
}

// #pragma endregion

#endif