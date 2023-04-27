// #include "ProjectDef.h"

#ifndef PMMGLOBALFUNCTIONS_H
#define PMMGLOBALFUNCTIONS_H

#include <PmmTypes.h>


PmmModBus ModbusPort[2] = 
{
  PmmModBus() ,
//   PmmModBus() ,
//   PmmModBus() ,
//   PmmModBus() ,
//   PmmModBus() 

};

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
uint8_t *MACAddress;

/*****************************************************************
 * Ethernet functions
 * **************************************************************/

u_int16_t PmmReturnConfig(int databit, int parity, int stopbit)
{
    unsigned long databitLong = 0x1ul;
    unsigned long parityLong = 0;
    unsigned long stopbitLong = 0x400ul;

    // PARITY
    if (parity == 1)
    {
        parityLong = 0x1ul;
    } // EVEN
    else if (parity == 2)
    {
        parityLong = 0x2ul;
    } // ODD
    else if (parity == 3)
    {
        parityLong = 0x3ul;
    } // NONE
    else if (parity == 4)
    {
        parityLong = 0x4ul;
    } // MARK
    else if (parity == 5)
    {
        parityLong = 0x5ul;
    } // SPACE
    else if (parity == 6)
    {
        parityLong = 0xFul;
    } // MASK

    // STOP BIT
    if (stopbit == 10)
    {
        stopbitLong = 0x10ul;
    }
    else if (stopbit == 15)
    {
        stopbitLong = 0x20ul;
    }
    else if (stopbit == 20)
    {
        stopbitLong = 0x30ul;
    }
    else
    {
        stopbitLong = 0xF0ul;
    }

    // DATA BITS
    if (databit == 5)
    {
        databitLong = 0x100ul;
    }
    else if (databit == 6)
    {
        databitLong = 0x200ul;
    }
    else if (databit == 7)
    {
        databitLong = 0x300ul;
    }
    else if (databit == 8)
    {
        databitLong = 0x400ul;
    }
    else
    {
        databitLong = 0xF00ul;
    }

    // 0x400ul | 0x10ul | 0x3ul = ??
    u_int16_t config = (databitLong | parityLong | stopbitLong);
    return config;
}

void PMMInitializeEthernet()
{

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

    // Try to start if any
    ThisProduct.EthernetRunning = false;
    if (ThisProduct.PmmGeneral.ItHasEthernet)
    {
        Ethernet.init(10); // for W5100 sspin
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
    if (ThisProduct.WireRunning == false)
    {
        Wire.begin();

        ThisProduct.WireRunning = true;
    }
}

void PmmInitializeProjectSettings()
{

    // STEP01 :  Initializ Wire library
    InitializeWire();
    // STEP02 : Read All settings from ROM into "ThisProduct" struct
    PmmReadAllSettings(0);
    // 1. WatchDog 8s
    PmmWatchDoggy.setup(WDT_SOFTCYCLE8S);
    // 4. EEprom
    ThisProduct.I2CRunning = false;
    if (ThisProduct.PmmGeneral.ItHasExtEEPROM == true)
    {
        // StartEEprom();

        ThisProduct.I2CRunning = true;
    }

    // 5. Ethernet
    if (ThisProduct.PmmGeneral.ItHasEthernet == true)
    {
        PMMInitializeEthernet();
    }

    // 6. Protocols : a. modbus

    if (ThisProduct.PmmSerial[0].Enabled)
    {
        if (ThisProduct.PmmSerial[0].PmmProtocols.IsModBus)
        {
            if (ThisProduct.PmmSerial[0].PmmProtocols.ModBusTCP)
            {

                if (ThisProduct.PmmSerial[0].PmmProtocols.ModBusMaster)
                {
                    ModbusPort[5].PMMmodbusTCPServerSetup(ThisProduct.PmmSerial[0].Port01, ThisProduct.PmmSerial[0].PmmProtocols.SlaveID);

                    ModbusPort[5].PMMmodbusTCPServerconfigure(
                        ThisProduct.PmmSerial[0].PmmProtocols.CoilsStatus,
                        ThisProduct.PmmSerial[0].PmmProtocols.StartingAddressCoilsStatus,
                        ThisProduct.PmmSerial[0].PmmProtocols.QuantityCoilsStatus,
                        ThisProduct.PmmSerial[0].PmmProtocols.InputStatus,
                        ThisProduct.PmmSerial[0].PmmProtocols.StartingAddressInputStatus,
                        ThisProduct.PmmSerial[0].PmmProtocols.QuantityInputStatus,
                        ThisProduct.PmmSerial[0].PmmProtocols.HoldingRegisters,
                        ThisProduct.PmmSerial[0].PmmProtocols.StartingAddressHoldingRegisters,
                        ThisProduct.PmmSerial[0].PmmProtocols.QuantityHoldingRegisters,
                        ThisProduct.PmmSerial[0].PmmProtocols.InputRegisters,
                        ThisProduct.PmmSerial[0].PmmProtocols.StartingAddressInputRegisters,
                        ThisProduct.PmmSerial[0].PmmProtocols.QuantityInputRegisters);

                    ThisProduct.PmmSerial[0].PmmProtocols.IsRunning = true;
                }

                if (ThisProduct.PmmSerial[0].PmmProtocols.ModBusSlave)
                {
                    ThisProduct.PmmSerial[0].PmmProtocols.IsRunning = true;
                }
            }
        }
    }

    for (int serial = 1; serial <= 4; serial++)
    {
        if (ThisProduct.PmmSerial[serial].Enabled)
        {
            if (ThisProduct.PmmSerial[serial].PmmProtocols.IsModBus)
            {
                if (ThisProduct.PmmSerial[serial].PmmProtocols.ModBusRTU)
                {
                    u_int16_t configCom = PmmReturnConfig(
                        ThisProduct.PmmSerial[serial].PortDataBit,
                        ThisProduct.PmmSerial[serial].PortParity,
                        ThisProduct.PmmSerial[serial].PortStopBit);

                    ModbusPort[serial].PMMModBUSRTUClientSetup(
                        configCom,
                        ThisProduct.PmmSerial[serial].BaudRate,
                        ThisProduct.PmmSerial[serial].TXPin,
                        ThisProduct.PmmSerial[serial].RXPin,
                        ThisProduct.PmmSerial[serial].SerialSelectionPin,
                        ThisProduct.PmmSerial[serial].SerialPort);

                    if (ThisProduct.PmmSerial[serial].PmmProtocols.ModBusMaster)
                    {
                        ModbusPort[serial].PMMModBUSRTUClientSetup(
                            configCom,
                            ThisProduct.PmmSerial[serial].BaudRate,
                            ThisProduct.PmmSerial[serial].TXPin,
                            ThisProduct.PmmSerial[serial].RXPin,
                            ThisProduct.PmmSerial[serial].SerialSelectionPin,
                            ThisProduct.PmmSerial[serial].SerialPort);

                            ThisProduct.PmmSerial[serial].PmmProtocols.IsRunning = true;
                    }

                    if (ThisProduct.PmmSerial[serial].PmmProtocols.ModBusSlave)
                    {
                        ModbusPort[serial].PMMModBUSRTUServerSetup(
                            configCom,
                            ThisProduct.PmmSerial[serial].BaudRate,
                            ThisProduct.PmmSerial[serial].TXPin,
                            ThisProduct.PmmSerial[serial].RXPin,
                            ThisProduct.PmmSerial[serial].SerialSelectionPin,
                            ThisProduct.PmmSerial[serial].SerialPort);

                        ModbusPort[serial].PMMModBUSRTUServerconfigure(
                            ThisProduct.PmmSerial[serial].PmmProtocols.CoilsStatus,
                            ThisProduct.PmmSerial[serial].PmmProtocols.StartingAddressCoilsStatus,
                            ThisProduct.PmmSerial[serial].PmmProtocols.QuantityCoilsStatus,
                            ThisProduct.PmmSerial[serial].PmmProtocols.InputStatus,
                            ThisProduct.PmmSerial[serial].PmmProtocols.StartingAddressInputStatus,
                            ThisProduct.PmmSerial[serial].PmmProtocols.QuantityInputStatus,
                            ThisProduct.PmmSerial[serial].PmmProtocols.HoldingRegisters,
                            ThisProduct.PmmSerial[serial].PmmProtocols.StartingAddressHoldingRegisters,
                            ThisProduct.PmmSerial[serial].PmmProtocols.QuantityHoldingRegisters,
                            ThisProduct.PmmSerial[serial].PmmProtocols.InputRegisters,
                            ThisProduct.PmmSerial[serial].PmmProtocols.StartingAddressInputRegisters,
                            ThisProduct.PmmSerial[serial].PmmProtocols.QuantityInputRegisters);

                            ThisProduct.PmmSerial[serial].PmmProtocols.IsRunning = true;
                    }
                }
            }
        }
    }

    // 7. RTC internal

    ThisProduct.PmmGeneral.InternalRTC = true;

    if (ThisProduct.PmmGeneral.InternalRTC)
    {
        PmmRTCInternal.begin();
        ThisProduct.InternalRTCRunning = true;
    }

    // 8.RTC External

    if (ThisProduct.PmmGeneral.ExternalRTC) // DS3231
    {

        // PMMInitializeExternalRTC();
        if (PmmRTCExternal.RTCCheck())
            ThisProduct.ExternalRTCRunning = true;
    }

    // 7. get time from NTP Server if any

    // 8. Time and running hours

    // 9. Extension Boards if any

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