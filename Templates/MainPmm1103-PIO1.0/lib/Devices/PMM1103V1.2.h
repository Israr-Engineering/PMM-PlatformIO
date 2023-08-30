
#ifndef PMM1103V12_H
#define PMM1103V12_H

#include "Arduino.h"
#include <PmmTypes.h>
#include <FlashStorage_SAMD.h>

#ifdef PMMGlobalFunctions
#include <PmmGlobalFunctions.h>
#endif

// Common INPUT Pins If any (Pin Mask)
static const uint8_t DILOSSPOWER = 19ul;  // PA20
static const uint8_t DIMANAUTO = 19ul;    // PA06
static const uint8_t DIMANEAST = 19ul;    // PA19
static const uint8_t DIMANWEST = 18ul;    // PA18
static const uint8_t DILSFEEDBACK = 19ul; // PA15
static const uint8_t DIPODINT = 19ul;     // PA28
static const uint8_t DIPROG = 19ul;       // PA03
// Common Analoge INPUT Pins If any (Pin Mask)
static const uint8_t AIPOTENTIOMETER = 19ul; // (P2) PB02
static const uint8_t AITHERMOISTOR = 19ul;   // PA02
// Common OUTPUT Pins If any (Pin Mask)
static const uint8_t DORELAWEST = 15ul; // PA04
static const uint8_t DORELAEAST = 15ul; // PA05
static const uint8_t DORFAN = 15ul;     // PA14
static const uint8_t DOLED = 15ul;      // PA13
// Pmm1103 main struct
typedef struct PMM1103
{
    // General
    u_int16_t ModBusID = 1;
    u_int16_t DeviceType = 0; // 0 => Slave , 1 => Master , ...
    u_int16_t DeviceMode = 0;
    u_int16_t SaveCommand = 0;
    // Location and datetime
    long Latitude = 0;  // ×1000000|
    long Longitude = 0; // ×1000000|
    int TimeZone = 3;
    long UTCTime = 0; // unix format
    u_int16_t RTCYear = 2023;
    u_int16_t RTCMonth = 1;
    u_int16_t RTCDay = 1;
    u_int16_t RTCHours = 0;
    u_int16_t RTCMinutes = 0;
    u_int16_t RTCSeconds = 0;
    // Angle
    int MPUAngle = 0;
    int MPUTrueAngle = 0;
    int TargetAngle = 0;
    int TolernceAngle = 0;
    int MAXAngle = 55;
    int MAXMAXAngle = 60;
    int MINAngle = 55;
    int MINMAXAngle = 60;
    int ParkAngle = 0;
    int ClibrationAngle = 0;
    // WindSpeed Control
    int WindSpeed = 0;
    int WindSpeedLimit = 0;
    int WindSpeedTimeoutIN = 10000;
    int WindSpeedTimeoutOUT = 10000;
    // Modes
    bool MANUALMODE = false;
    bool AUTOREMOTEMODE = false;
    bool AUTOLOCALMODE = false;
    bool ALARMMODE = false;
    bool EMERGENCYMODE = false;
    bool DEBUGMODE = false;
    bool IsNotFirstStart = false;
} PMM1103;
// Global Classes and structs
PmmModBus ModbusRTU02Client;
PmmModBus ModbusRTU01Server;
PMM1103 Pmmm1103;

long BaudRate01 = 19200;
long BaudRate02 = 9600;
u_int16_t PinTx = 35;
u_int16_t PinRx = 36;
u_int16_t PinTxEna = 31;
u_long SettingSerial01 = 1043; // SERIAL_8N1

// Status LEDs

bool AlarmFlag = false; // false meanes its in alarm Active ,true is healthy status

// General Variables
uint16_t tempRead = 0;
int ErrorCounter = 0;
uint16_t OutRelayImage = 0x0;
uint16_t OutInputsImage = 0x0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This product main functions part                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PmmMPUSetup()
{
    Wire.begin();
    Wire.beginTransmission(0x69);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

float PmmMPUReadRawAngle() // This Function Get MPU Angle Without Calibration *** This Function Use Only To calibration Function
{
    int16_t AcX, AcY, AcZ;
    int minVal = 265;
    int maxVal = 402;
    Wire.beginTransmission(0x69);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x69, 14, true);
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    int xAng = map(AcX, minVal, maxVal, -90, 90);
    int yAng = map(AcY, minVal, maxVal, -90, 90);
    int zAng = map(AcZ, minVal, maxVal, -90, 90);

    double x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    // double y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
    // double z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
    if (abs(x) > 180)
    {
        x = x - 360;
    }
    return x;
}

void hardwareInit()
{
    // STEP01 : Prepare IOs
    pinMode(DILOSSPOWER, INPUT);
    pinMode(DIMANAUTO, INPUT);
    pinMode(DIMANEAST, INPUT);
    pinMode(DIMANWEST, INPUT);
    pinMode(DILSFEEDBACK, INPUT);
    pinMode(DIPODINT, INPUT);
    pinMode(DIPROG, INPUT);

    pinMode(DORELAWEST, OUTPUT);
    pinMode(DORELAEAST, OUTPUT);
    pinMode(DORFAN, OUTPUT);
    pinMode(DOLED, OUTPUT);

    // STEP02 : Get device address and load variables and check factory Defaults
    PmmInternalEEPROM.get(0, Pmmm1103);
    if (!Pmmm1103.IsNotFirstStart) // in not configured before Load defult settings
    {
        PMM1103 DefaultPmm1103;
        DefaultPmm1103.IsNotFirstStart = true;
        PmmInternalEEPROM.put(0, DefaultPmm1103);
        PmmInternalEEPROM.get(0, Pmmm1103);
    }

    // STEP03 : Initialize mdbus RTU Server and client         
    //Server
    ModbusRTU01Server.PmmModBusRTUServerSetup(Pmmm1103.ModBusID, SettingSerial01, BaudRate01, PinTx, PinRx, PinTxEna, 1);
    ModbusRTU01Server.PmmModBusRTUServerconfigure(true, 0, 24, true, 0, 32, true, 0, 32, true, 0, 24);
    if (Pmmm1103.DeviceType == 1) // if master device config serial02 as client to read time from GPS if any 
    {
       ModbusRTU02Client.PmmModBusRTUClientSetup(SettingSerial01, BaudRate01, PinTx, PinRx, PinTxEna, 1); // 35,36,31  
    }    

    // STEP04 : Initialize MPU then read positions

    PmmMPUSetup();


    // STEP05 : update internal RTC time

    // STEP06 : Do SunClac based on time

    // STEP7: Initialize WatchDog timer
    // ignore if in debug mode

    // if (!DIDEBUGMODE_Status)
    // {
    //     if (MasterIOMode & FactoryDefualtMode & ModbusTCPMode)
    //         PmmWatchDoggy.setup(WDT_HARDCYCLE2S);
    //     else if (MasterIOMode & FactoryDefualtMode & !ModbusTCPMode)
    //         PmmWatchDoggy.setup(WDT_HARDCYCLE2S);
    //     else if (MasterIOMode & !FactoryDefualtMode)
    //         PmmWatchDoggy.setup(WDT_HARDCYCLE4S);
    //     else
    //         PmmWatchDoggy.setup(WDT_HARDCYCLE8S);
    // }
}

void updateInputBuffers()
{
}

void updateOutputBuffers()
{

    OutRelayImage = 0;
    // update relays
}

void GenerateAlarm()
{
}

void syncModbusBuffers()
{

    int_input[2] = 1948;
    int_output[2] = 0;

    // for (int i = 0; i < MAX_DIGITAL_INPUT; i++)
    // {

    //     ModbusRTUServer.PmmModBusRTUServerDiscreteWrite(i, bool_input[i / 8][i % 8]);
    // }
    // for (int i = 0; i < MAX_ANALOG_INPUT; i++)
    // {

    //     ModbusRTUServer.PmmModBusRTUServerInputWrite(i, int_input[i]);
    // }
    // // Read changes from clients
    // for (int i = 0; i < MAX_DIGITAL_OUTPUT; i++)
    // {

    //     bool_output[i / 8][i % 8] = ModbusRTUServer.PmmModBusRTUServerCoilRead(i);
    // }

    // for (int i = 0; i < MAX_ANALOG_OUTPUT; i++)
    // {

    //     int_output[i] = ModbusRTUServer.PmmModBusRTUServerHoldingRead(i);
    // }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This product main functions                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
