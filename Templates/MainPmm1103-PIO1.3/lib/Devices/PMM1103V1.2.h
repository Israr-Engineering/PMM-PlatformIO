
#ifndef PMM1103V12_H
#define PMM1103V12_H

#include "Arduino.h"
#include <PmmTypes.h>
#include <FlashStorage_SAMD.h>
#include <time.h>
#include <PmmInternalRTC.h>
#include <PmmSunCalculations.h>

#ifdef PMMGlobalFunctions
#include <PmmGlobalFunctions.h>
#endif

#pragma region "Variables Definition"
// Common INPUT Pins If any (Pin Mask)
static const uint8_t DILOSSPOWER = 10ul;  // PA20
static const uint8_t DIMANAUTO = 14ul;    // PA06
static const uint8_t DIMANEAST = 20ul;    // PA19
static const uint8_t DIMANWEST = 18ul;    // PA18
static const uint8_t DILSFEEDBACK = 16ul; // PA15
static const uint8_t DIPOSINT = 32ul;     // PA28
static const uint8_t DIPROG = 46ul;       // PA03
// Common Analoge INPUT Pins If any (Pin Mask)
static const uint8_t AIPOTENTIOMETER = 19ul; // (P2) PB02
static const uint8_t AITHERMOISTOR = 47ul;   // PA02
// Common OUTPUT Pins If any (Pin Mask)
static const uint8_t DORELAWEST = 25ul; // PA04
static const uint8_t DORELAEAST = 17ul; // PA05 (17ul or 2ul)
static const uint8_t DORFAN = 15ul;     // PA14
static const uint8_t DOLED = 9ul;       // PA13

// ToSave struct
typedef struct InRom
{
    // General
    u_int16_t ModBusID = 1;
    u_int16_t DeviceType = 0; // 0 => Slave , 1 => Master , ...
    u_int16_t DeviceMode = 0;
    u_int16_t Command = 0;
    u_int16_t UpdatePosTimer = 2; // Postion update timer in minutes
    // Location and datetime (31.53284475631496, 36.027797725634365)
    int Latitude = 3153;  // ×100| = 31.53
    int Longitude = 3602; // ×100| = 36.02
    int TimeZone = 3;
    int TolernceAngle = 100; // x100
    int MAXAngle = 6000;     // x100
    int MAXCalAngle = 2200;  // x100
    int MINAngle = -6000;    // x100
    int MINCalAngle = 15700; // x100
    int ParkAngle = 50;      // x100
    int ClibrationAngle = 0; // x100
    // WindSpeed Control
    int WindSpeedLimit = 0;        // x100
    int WindSpeedTimeoutIN = 15;   // secondes
    int WindSpeedTimeoutOUT = 100; // secondes
    // Extras for backtracking
    int TrackerWidth = 400;
    int Post2Post = 1100;

} InRom;
// Pmm1103 main struct
typedef struct PMM1103
{
    InRom InROM;
    u_int16_t UpdatePosTimer = 5; // Postion update timer in ms
    // Location and datetime (31.53284475631496, 36.027797725634365)
    float Latitude = 31.53;  // ×100| = 31.53
    float Longitude = 36.02; // ×100| = 36.02
    int TimeZone = 3;
    float TolernceAngle = 1;   // x100
    float MAXAngle = 55;       // x100
    float MAXCalAngle = 60;    // x100
    float MINAngle = -55;      // x100
    float MINCalAngle = -60;   // x100
    float ParkAngle = 50;      // x100
    float ClibrationAngle = 0; // x100
    // WindSpeed settings
    float WindSpeedLimit = 0;      // x100
    int WindSpeedTimeoutIN = 15;   // secondes
    int WindSpeedTimeoutOUT = 100; // secondes
    // RTC and dateTime
    long RTCTime = 0; // unix format (starting from 2000 year)
    time_t DateTime;
    u_int16_t RTCYear = 23;
    u_int16_t RTCMonth = 9;
    u_int16_t RTCDay = 1;
    u_int16_t RTCHours = 15;
    u_int16_t RTCMinutes = 3;
    u_int16_t RTCSeconds = 0;
    // Angle
    float MPURawAngle = 0;
    float MPUCalAngle = 0;
    float CalcTrueAngle = 0;
    float CalcTargetAngle = 0;
    float CalcTrueAngleNerl = 0;
    float BackTrackAngle = 0;
    float RemoteTargetAngle = 0; // x100
    // WindSpeed Real time
    int WindSpeed = 0;
    // Modes
    bool MANUALMODE = false;
    bool AUTOREMOTEMODE = false;
    bool AUTOLOCALMODE = false;
    bool ALARMMODE = false;
    bool EMERGENCYMODE = false;
    bool PROGRAMMINGMODE = false;
    bool CALIBRATIONMODE = false;
    bool PARKINGMODE = false;
    bool IsNotFirstStart = false;

    // ManualCommands
    bool ManCmdMoveEast = false;
    bool ManCmdMoveWest = false;

    // movement
    bool MoveEast = false;
    bool MoveWest = false;
    bool MANAUTOFlage = false;
    bool EastEnable = true;
    bool WestEnable = true;

    // Extras for backtracking
    float TrackerWidth = 400;
    float Post2Post = 1100;

    // timers
    uint32_t EMERGENCYTimer = 0;
    uint32_t EMERGENCYTimerIN = 0;
    bool EMERGENCYTimerINEnable = false;

    uint32_t LOCALREMOTETimer = 0;
    long RunAutoTimer = 0;

} PMM1103;
// Global Classes and structs
PmmModBus ModbusRTU02Client;
PmmModBus ModbusRTU01Server;
PMM1103 Pmmm1103;
PmmSunCalculations PmmSunCalc;
PmmInternalRTC PmmInternalRTCRT;

// Functions
void MappingRegisters();
void RunAutoMode();

// General Variables
uint16_t tempRead = 0;
int ErrorCounter = 0;
uint16_t OutRelayImage = 0x0;
uint16_t OutInputsImage = 0x0;

#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This product main functions part                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void hardwareInit()
{

    // Serial1
    //  STEP01 : Prepare IOs
    pinMode(DILOSSPOWER, INPUT);
    pinMode(DIMANAUTO, INPUT_PULLUP);
    pinMode(DIMANEAST, INPUT_PULLUP);
    pinMode(DIMANWEST, INPUT_PULLUP);
    pinMode(DILSFEEDBACK, INPUT);
    pinMode(DIPOSINT, INPUT);
    pinMode(DIPROG, INPUT_PULLUP);

    pinMode(DORELAWEST, OUTPUT);
    pinMode(DORELAEAST, OUTPUT);

    digitalWrite(DORELAWEST, LOW);
    digitalWrite(DORELAEAST, LOW);

    pinMode(DORFAN, OUTPUT);
    pinMode(DOLED, OUTPUT);

    // STEP02 : Get device address and load variables and check factory Defaults
    PmmInternalEEPROM.get(0, Pmmm1103.InROM);
    if ((Pmmm1103.InROM.ModBusID > 31) | (Pmmm1103.InROM.ModBusID < 0)) // Mostly first start
    {
        PMM1103 DefaultSettings;
        PmmInternalEEPROM.put(0, DefaultSettings.InROM);
        NVIC_SystemReset();
    }

    // STEP03 : Initialize mdbus RTU Server and client
    // Server
    ModbusRTU01Server.PmmModBusRTUServerSetup(Pmmm1103.InROM.ModBusID, SERIAL_8N1, 9600, 35, 36, 31, 1); //  35,36,31
    ModbusRTU01Server.PmmModBusRTUServerconfigure(true, 0, 24, true, 0, 32, true, 0, 32, true, 0, 24);
    if (Pmmm1103.InROM.DeviceType == 1) // if master device config serial02 as client to read time from GPS if any
    {
        ModbusRTU02Client.PmmModBusRTUClientSetup(SERIAL_8N1, 9600, 0, 1, 4, 2); //   PA10,PA11.PA07
    }

    MappingRegisters();
    ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(3, 1967); // update holding registers from input reg order

    // STEP04 : Initialize MPU then you can read positions
    PmmSunCalc.PmmMPUSetup();

    // STEP05 : update internal RTC time
    PmmInternalRTCRT.begin();

    // STEP06 : Ready to Do SunClac based on time

    // STEP7: Initialize WatchDog timer
    if (!Pmmm1103.PROGRAMMINGMODE)
    {
        PmmWatchDoggy.setup(WDT_HARDCYCLE8S);
    }
}

void updateInputBuffers()
{
    // STEP00 : modes
    Pmmm1103.PROGRAMMINGMODE = !digitalRead(DIPROG);
    Pmmm1103.MANUALMODE = digitalRead(DIMANAUTO);
    Pmmm1103.ManCmdMoveEast = !digitalRead(DIMANEAST);
    Pmmm1103.ManCmdMoveWest = !digitalRead(DIMANWEST);

    if (Pmmm1103.InROM.DeviceMode == 1)
    {
        Pmmm1103.CALIBRATIONMODE = true;
    }
    else
    {
        Pmmm1103.CALIBRATIONMODE = false;
    }

    // STEP01 : RTC Time
    Pmmm1103.RTCYear = PmmInternalRTCRT.getYear();
    Pmmm1103.RTCMonth = PmmInternalRTCRT.getMonth();
    Pmmm1103.RTCDay = PmmInternalRTCRT.getDay();
    Pmmm1103.RTCHours = PmmInternalRTCRT.getHours();
    Pmmm1103.RTCMinutes = PmmInternalRTCRT.getMinutes();
    Pmmm1103.RTCSeconds = PmmInternalRTCRT.getSeconds();

    Pmmm1103.RTCTime = PmmInternalRTCRT.getY2kEpoch();

    Pmmm1103.DateTime = PmmSunCalc.PMMSetAnDatetime(Pmmm1103.RTCYear + 30, Pmmm1103.RTCMonth, Pmmm1103.RTCDay, Pmmm1103.RTCHours, Pmmm1103.RTCMinutes, Pmmm1103.RTCSeconds);

    // STEP02 : SunCalc

    PmmSunCalc.UpdateCalculations(Pmmm1103.DateTime, Pmmm1103.Latitude, Pmmm1103.Longitude, Pmmm1103.TimeZone);

    Pmmm1103.CalcTrueAngle = PmmSunCalc.TrueAngle;
    Pmmm1103.CalcTrueAngleNerl = PmmSunCalc.TrueAngleNerl;
    Pmmm1103.BackTrackAngle = PmmSunCalc.BackTrackerAngle;

    // STEP03 : MPU Angle
    Pmmm1103.MPURawAngle = PmmSunCalc.PmmMPUReadRawAngle();
    Pmmm1103.MPUCalAngle = PmmSunCalc.PmmMPUReadCalAngle(Pmmm1103.MINAngle, Pmmm1103.MAXAngle, Pmmm1103.MINCalAngle, Pmmm1103.MAXCalAngle);
    // Pmmm1103.MPUCalAngle = PmmSunCalc.PmmMPUReadCalAngle(-60.00, 60.00, 157.00, 22.3);

    // STEP04  : other updates
}

void updateOutputBuffers()
{
    // STEP01 : update Auto / Manual
    if (Pmmm1103.MANUALMODE)
    {
        if (Pmmm1103.MANAUTOFlage == false)
        {
            Pmmm1103.MANAUTOFlage = true;
            Pmmm1103.MoveEast = false;
            Pmmm1103.MoveWest = false;
        }

        if (Pmmm1103.ManCmdMoveEast)
        {
            Pmmm1103.MoveEast = true;
            Pmmm1103.MoveWest = false;
        }
        else
            Pmmm1103.MoveEast = false;

        if (Pmmm1103.ManCmdMoveWest)
        {
            Pmmm1103.MoveEast = false;
            Pmmm1103.MoveWest = true;
        }
        else
            Pmmm1103.MoveWest = false;
    }
    else // this for AUTOMODE
    {
        if (Pmmm1103.MANAUTOFlage == true) // check Changing from Manual to Auto
        {
            Pmmm1103.MANAUTOFlage = false;
            Pmmm1103.MoveEast = false;
            Pmmm1103.MoveWest = false;
        }

        // if(Pmmm1103.CALIBRATIONMODE == false)
        // update every one second
        // if ((millis() - Pmmm1103.RunAutoTimer) > 1000)
        // {
            RunAutoMode();
            Pmmm1103.RunAutoTimer = millis();
        //}
    }

    // STEP02 : check max-min limit tolerances
    if (Pmmm1103.MPUCalAngle > Pmmm1103.MAXAngle)
        Pmmm1103.EastEnable = false; // block East direction
    if (Pmmm1103.MPUCalAngle < Pmmm1103.MINAngle)
        Pmmm1103.WestEnable = false; // block west direction

    // STEP03 : Do Real output
    if (Pmmm1103.MoveEast && !Pmmm1103.MoveWest && Pmmm1103.EastEnable && (Pmmm1103.MPUCalAngle < Pmmm1103.MAXAngle) && Pmmm1103.CALIBRATIONMODE == false)
    {
        digitalWrite(DORELAEAST, HIGH);
        digitalWrite(DORELAWEST, LOW);

        Pmmm1103.WestEnable = true; // unblock west direction
    }
    else if (!Pmmm1103.MoveEast && Pmmm1103.MoveWest && Pmmm1103.WestEnable && (Pmmm1103.MPUCalAngle > Pmmm1103.MINAngle) && Pmmm1103.CALIBRATIONMODE == false)
    {
        digitalWrite(DORELAWEST, HIGH);
        digitalWrite(DORELAEAST, LOW);

        Pmmm1103.EastEnable = true; // unblock East direction
    }
    else if (Pmmm1103.MoveEast && !Pmmm1103.MoveWest && Pmmm1103.CALIBRATIONMODE == true)
    {
        digitalWrite(DORELAEAST, HIGH);
        digitalWrite(DORELAWEST, LOW);
    }
    else if (!Pmmm1103.MoveEast && Pmmm1103.MoveWest && Pmmm1103.CALIBRATIONMODE == true)
    {
        digitalWrite(DORELAWEST, HIGH);
        digitalWrite(DORELAEAST, LOW);
    }
    else
    {
        digitalWrite(DORELAWEST, LOW);
        digitalWrite(DORELAEAST, LOW);
    }

    // Update other things
    // STEP05 : check max-min limit tolerances again
    if (Pmmm1103.MPUCalAngle > Pmmm1103.MAXAngle)
        Pmmm1103.EastEnable = false; // block East direction
    if (Pmmm1103.MPUCalAngle < Pmmm1103.MINAngle)
        Pmmm1103.WestEnable = false; // block west direction
}

void GenerateAlarm()
{
    // STEP00 : DateTime Alarm
    // STEP01 : Movement Alarm
    // STEP02 : Actual Position Alarm
    // STEP03 : Communication Alarm
    // STEM04 : Data Validation Alarm (if any dont save to ROM)
    // STEP05 : General Alarm
}

void syncModbusBuffers()
{

    // STEP00 : HandShaking setup
    if (!int_output[30] == 0)
    {
        int_output[30] = 0;
        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(30, 0);
    }

    // STEP01 : Looping buffer
    for (int i = 0; i < MAX_DIGITAL_INPUT; i++)
    {

        ModbusRTU01Server.PmmModBusRTUServerDiscreteWrite(i, bool_input[i / 8][i % 8]);
    }
    for (int i = 0; i < MAX_ANALOG_INPUT; i++)
    {

        ModbusRTU01Server.PmmModBusRTUServerInputWrite(i, int_input[i]);
    }
    // Read changes from clients
    for (int i = 0; i < MAX_DIGITAL_OUTPUT; i++)
    {

        bool_output[i / 8][i % 8] = ModbusRTU01Server.PmmModBusRTUServerCoilRead(i);
    }

    for (int i = 0; i < MAX_ANALOG_OUTPUT; i++)
    {

        int_output[i] = ModbusRTU01Server.PmmModBusRTUServerHoldingRead(i);
    }

    // STEP02 : HandShaking result
    if (int_output[30] == 1967)
        Pmmm1103.LOCALREMOTETimer = 0;
    // STEP03 :  Mapping
    MappingRegisters();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This product Extra functions start here                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int AutoMoveCode = 400;
bool AutoMoveEnable = false;
bool AutoMoveNewCycle = false;

void RunAutoMode()
{
    // STEP00 : Check Auto modes

    Pmmm1103.AUTOLOCALMODE = (Pmmm1103.LOCALREMOTETimer > 10 /*60*/) ? true : false;
    Pmmm1103.AUTOREMOTEMODE = !Pmmm1103.AUTOLOCALMODE;

    if (Pmmm1103.AUTOREMOTEMODE)
    {
        AutoMoveCode = int_output[29];
        AutoMoveEnable = (AutoMoveCode == 500) ? true : false; // enable code from Master device
    }

    if ((Pmmm1103.RTCMinutes % Pmmm1103.UpdatePosTimer) == 0)
    {
        if (AutoMoveNewCycle == true)
        {
            AutoMoveEnable = true;
            AutoMoveNewCycle = false;
        }

        if (!Pmmm1103.PARKINGMODE || !Pmmm1103.EMERGENCYMODE)
        {
            if (Pmmm1103.AUTOLOCALMODE)
            {
                Pmmm1103.RemoteTargetAngle = Pmmm1103.CalcTrueAngle; // PmmSunCalc.TrueAngle;
                ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(24, int(Pmmm1103.RemoteTargetAngle * 100));
            }
        }
    }
    else
        AutoMoveNewCycle = true;

    // STEP01 : Park position
    if ((Pmmm1103.RTCHours >= hour(PmmSunCalc.Sunset) ) ||
         ((Pmmm1103.RTCHours <= hour(PmmSunCalc.Sunrise) )))
    {
        Pmmm1103.RemoteTargetAngle = Pmmm1103.ParkAngle;
        Pmmm1103.PARKINGMODE = true;
        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(24, int(Pmmm1103.RemoteTargetAngle * 100));
    }
    else
    {
        Pmmm1103.PARKINGMODE = false;
    }

    // STEP02 : Wind speed Emergency
    if (Pmmm1103.WindSpeed > Pmmm1103.WindSpeedLimit)
    {
        if (!Pmmm1103.EMERGENCYTimerINEnable)
        {
            Pmmm1103.EMERGENCYTimerINEnable = true;
            Pmmm1103.EMERGENCYTimerIN = Pmmm1103.WindSpeedTimeoutIN;
        }
        if (Pmmm1103.EMERGENCYMODE == false && Pmmm1103.WindSpeedTimeoutIN == 0)
        {
            Pmmm1103.EMERGENCYTimer = Pmmm1103.WindSpeedTimeoutOUT;
            Pmmm1103.EMERGENCYTimerINEnable = false;
            Pmmm1103.EMERGENCYMODE = true;
        }
    }
    else
    {
        Pmmm1103.EMERGENCYTimerINEnable = false;
    }

    if (Pmmm1103.EMERGENCYMODE == true)
    {
        Pmmm1103.RemoteTargetAngle = 0;
        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(24, int(Pmmm1103.RemoteTargetAngle * 100));
        if (Pmmm1103.EMERGENCYTimer == 0)
        {
            Pmmm1103.EMERGENCYMODE = false;
            Pmmm1103.EMERGENCYTimerINEnable = false;
        }
    }

    // STEP03 : Check limits
    if (Pmmm1103.RemoteTargetAngle > Pmmm1103.MAXAngle)
    {
        Pmmm1103.RemoteTargetAngle = Pmmm1103.MAXAngle;
    }
    if (Pmmm1103.RemoteTargetAngle < Pmmm1103.MINAngle)
    {

        Pmmm1103.RemoteTargetAngle = Pmmm1103.MINAngle;
    }

    // STEP04 : Update registers
    Pmmm1103.CalcTargetAngle = Pmmm1103.RemoteTargetAngle;

    // Accepting Tolerances))
    if (Pmmm1103.CalcTargetAngle <= (Pmmm1103.MPUCalAngle + Pmmm1103.TolernceAngle) &&
        Pmmm1103.CalcTargetAngle >= (Pmmm1103.MPUCalAngle - Pmmm1103.TolernceAngle))
    {
        Pmmm1103.MoveEast = false;
        Pmmm1103.MoveWest = false;
        AutoMoveEnable = false;
        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(29, 400);
    }

    //  STEP05 : Order to move
    if (AutoMoveEnable)
    {
        if (Pmmm1103.CalcTargetAngle > Pmmm1103.MPUCalAngle) // move to west +++
        {
            Pmmm1103.MoveEast = true;
            Pmmm1103.MoveWest = false;
            ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(29, 100);
        }
        else if (Pmmm1103.CalcTargetAngle < Pmmm1103.MPUCalAngle) // move to east ---
        {
            Pmmm1103.MoveEast = false;
            Pmmm1103.MoveWest = true;
            ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(29, 200);
        }

        else
        {
            AutoMoveEnable = false;
            Pmmm1103.MoveEast = false;
            Pmmm1103.MoveWest = false;
            ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(29, 300);
        }
    }

    
}

void MappingRegisters()
{
    // STEP00: Copy ROM to runtime values

    Pmmm1103.UpdatePosTimer = Pmmm1103.InROM.UpdatePosTimer; // Postion update timer in ms
    // Location and datetime (31.53284475631496, 36.027797725634365)
    Pmmm1103.Latitude = float(Pmmm1103.InROM.Latitude) / 100;   // ×100| = 31.53
    Pmmm1103.Longitude = float(Pmmm1103.InROM.Longitude) / 100; // ×100| = 36.02
    Pmmm1103.TimeZone = Pmmm1103.InROM.TimeZone;
    Pmmm1103.TolernceAngle = float(Pmmm1103.InROM.TolernceAngle) / 100;     // x100
    Pmmm1103.MAXAngle = float(Pmmm1103.InROM.MAXAngle) / 100;               // x100
    Pmmm1103.MAXCalAngle = float(Pmmm1103.InROM.MAXCalAngle) / 100;         // x100
    Pmmm1103.MINAngle = float(Pmmm1103.InROM.MINAngle) / 100;               // x100
    Pmmm1103.MINCalAngle = float(Pmmm1103.InROM.MINCalAngle) / 100;         // x100
    Pmmm1103.ParkAngle = float(Pmmm1103.InROM.ParkAngle) / 100;             // x100
    Pmmm1103.ClibrationAngle = float(Pmmm1103.InROM.ClibrationAngle) / 100; // x100
    // WindSpeed settings
    Pmmm1103.WindSpeedLimit = float(Pmmm1103.InROM.WindSpeedLimit) / 100; // x100
    Pmmm1103.WindSpeedTimeoutIN = Pmmm1103.InROM.WindSpeedTimeoutIN;      // in Seconds
    Pmmm1103.WindSpeedTimeoutOUT = Pmmm1103.InROM.WindSpeedTimeoutOUT;    // in Seconds
    // Extras for backtracking
    Pmmm1103.TrackerWidth = float(Pmmm1103.InROM.TrackerWidth) / 100;
    Pmmm1103.Post2Post = float(Pmmm1103.InROM.Post2Post) / 100;

    // STEP01: To Modbus => Input Reg => int_input
    int_input[0] = Pmmm1103.InROM.ModBusID;
    int_input[1] = Pmmm1103.InROM.DeviceType;
    int_input[2] = Pmmm1103.InROM.DeviceMode;
    ///////////////////////////////////////// int_input[3] = Pmmm1103.InROM.Command;
    int_input[4] = Pmmm1103.Latitude * 100;
    int_input[5] = Pmmm1103.Longitude * 100;
    int_input[6] = Pmmm1103.TimeZone;
    int_input[7] = Pmmm1103.TolernceAngle * 100;
    int_input[8] = Pmmm1103.MAXAngle * 100;
    int_input[9] = Pmmm1103.MAXCalAngle * 100;
    int_input[10] = Pmmm1103.MINAngle * 100;
    int_input[11] = Pmmm1103.MINCalAngle * 100;
    int_input[12] = Pmmm1103.ParkAngle * 100;
    int_input[13] = Pmmm1103.ClibrationAngle * 100;
    int_input[14] = Pmmm1103.WindSpeedLimit;
    int_input[15] = Pmmm1103.WindSpeedTimeoutIN;
    int_input[16] = Pmmm1103.WindSpeedTimeoutOUT;
    int_input[17] = Pmmm1103.WindSpeed;
    int_input[18] = Pmmm1103.RTCYear;
    int_input[19] = Pmmm1103.RTCMonth;
    int_input[20] = Pmmm1103.RTCDay;
    int_input[21] = Pmmm1103.RTCHours;
    int_input[22] = Pmmm1103.RTCMinutes;
    int_input[23] = Pmmm1103.RTCSeconds;
    int_input[24] = Pmmm1103.RemoteTargetAngle * 100;
    int_input[25] = Pmmm1103.CalcTargetAngle * 100;
    int_input[26] = Pmmm1103.CalcTrueAngle * 100;
    int_input[27] = Pmmm1103.CalcTrueAngleNerl * 100;
    int_input[28] = Pmmm1103.BackTrackAngle * 100;
    int_input[29] = Pmmm1103.MPURawAngle * 100;
    int_input[30] = Pmmm1103.MPUCalAngle * 100;
    int_input[31] = Pmmm1103.InROM.UpdatePosTimer;

    // Timers

    // STEP02: Discrete inputs
    bool_input[0][0] = Pmmm1103.MANUALMODE;
    bool_input[0][1] = Pmmm1103.AUTOREMOTEMODE;
    bool_input[0][2] = Pmmm1103.AUTOLOCALMODE;
    bool_input[0][3] = Pmmm1103.ALARMMODE;
    bool_input[0][4] = Pmmm1103.EMERGENCYMODE;
    bool_input[0][5] = Pmmm1103.PROGRAMMINGMODE;
    bool_input[0][6] = Pmmm1103.CALIBRATIONMODE;
    bool_input[0][7] = Pmmm1103.PARKINGMODE;
    /*
     *
     *
     */
    bool_input[1][6] = Pmmm1103.MoveEast;
    bool_input[1][7] = Pmmm1103.MoveWest;
    // Realtime IO
    bool_input[1][7] = digitalRead(DORELAEAST);
    bool_input[2][0] = digitalRead(DORELAWEST);
    bool_input[2][1] = digitalRead(DILOSSPOWER);
    bool_input[2][2] = digitalRead(DIMANAUTO);
    bool_input[2][3] = digitalRead(DIMANEAST);
    bool_input[2][4] = digitalRead(DIMANWEST);
    bool_input[2][5] = digitalRead(DILSFEEDBACK);
    bool_input[2][6] = digitalRead(DIPOSINT);
    bool_input[2][7] = digitalRead(DIPROG);

    // STEP03: From Modbus => Holding Reg => int_output
    Pmmm1103.RemoteTargetAngle = float(int_output[24]) / 100;
    Pmmm1103.WindSpeed = float(int_output[17]) / 100;

    // STEP04 : Commands
    // STEP04.1 : Generate Save to ROM Command
    if (int_output[3] == 820)
    {
        Pmmm1103.InROM.ModBusID = int_output[0];
        Pmmm1103.InROM.DeviceType = int_output[1];
        Pmmm1103.InROM.DeviceMode = int_output[2];
        Pmmm1103.InROM.Command = int_output[3];
        Pmmm1103.InROM.Latitude = (int16_t)int_output[4];
        Pmmm1103.InROM.Longitude = (int16_t)int_output[5];
        Pmmm1103.InROM.TimeZone = int_output[6];
        Pmmm1103.InROM.TolernceAngle = (int16_t)int_output[7];
        Pmmm1103.InROM.MAXAngle = (int16_t)int_output[8];
        Pmmm1103.InROM.MAXCalAngle = (int16_t)int_output[9];
        Pmmm1103.InROM.MINAngle = (int16_t)int_output[10];
        Pmmm1103.InROM.MINCalAngle = (int16_t)int_output[11];
        Pmmm1103.InROM.ParkAngle = (int16_t)int_output[12];
        Pmmm1103.InROM.ClibrationAngle = (int16_t)int_output[13];
        Pmmm1103.InROM.WindSpeedLimit = (int16_t)int_output[14];
        Pmmm1103.InROM.WindSpeedTimeoutIN = int_output[15];
        Pmmm1103.InROM.WindSpeedTimeoutOUT = int_output[16];
        /*
        .
        .
        */
        Pmmm1103.InROM.UpdatePosTimer = int_output[31];

        // Do Save to internal EEprom
        PmmInternalEEPROM.put(0, Pmmm1103.InROM);

        int_output[3] = 0;
        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(3, 0);
        // NVIC_SystemReset();
    }

    // STEP04.2 : Copy ROM TO Holding registers
    if (int_output[3] == 1967)
    {
        for (int i = 0; i < 25; i++)
        {
            ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(i, int_input[i]);
        }

        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(31, int_input[31]);
        int_output[3] = 0;
        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(3, 0);
    }

    // STEP04.3 : Reset MCU 1948
    if (int_output[3] == 1948)
    {
        NVIC_SystemReset();
        int_output[3] = 0;
        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(3, 0);
    }

    // STEP04.4 : Update internal clock 2003
    if (int_output[3] == 2003)
    {
        PmmInternalRTCRT.setYear(int_output[18]);
        PmmInternalRTCRT.setMonth(int_output[19]);
        PmmInternalRTCRT.setDay(int_output[20]);
        PmmInternalRTCRT.setHours(int_output[21]);
        PmmInternalRTCRT.setMinutes(int_output[22]);
        PmmInternalRTCRT.setSeconds(int_output[23]);

        int_output[3] = 0;
        ModbusRTU01Server.PmmModBusRTUServerHoldingWrite(3, 0);
    }
}

void ShowDebugSummary()
{
    SerialUSB.println("General : ");
    SerialUSB.print(" /My ID : ");
    SerialUSB.print(Pmmm1103.InROM.ModBusID);
    SerialUSB.print(" /Programming Mode : ");
    SerialUSB.print(Pmmm1103.PROGRAMMINGMODE);
    SerialUSB.print(" /Manual Mode : ");
    SerialUSB.print(Pmmm1103.MANUALMODE);
    SerialUSB.print("/Raw Angle : ");
    SerialUSB.print(Pmmm1103.MPURawAngle);
    SerialUSB.print(" /Calebrated Angle : ");
    SerialUSB.print(Pmmm1103.MPUCalAngle);
    SerialUSB.print(" /RT Max Angle : ");
    SerialUSB.println(Pmmm1103.MAXAngle);
    SerialUSB.print(" /RT Min Angle : ");
    SerialUSB.println(Pmmm1103.MINAngle);

    SerialUSB.println("SunCalc :");
    SerialUSB.print(" /DayOfYear = ");
    SerialUSB.print(/*year(Pmmm1103.DateTime) PmmSunCalc.PMMReturnDayOfYear(2023,9,2)*/ PmmSunCalc.DayOfYear);
    SerialUSB.print(" /B = ");
    SerialUSB.print(PmmSunCalc.B);
    SerialUSB.print(" /EOT = ");
    SerialUSB.print(PmmSunCalc.EOT);
    SerialUSB.print(" /Declination = ");
    SerialUSB.print(PmmSunCalc.Declination);
    SerialUSB.print(" /LSTM = ");
    SerialUSB.print(PmmSunCalc.LSTM);

    SerialUSB.print(" /TC = ");
    SerialUSB.print(PmmSunCalc.TC);
    SerialUSB.print(" /HRA = ");
    SerialUSB.print(PmmSunCalc.HRA);
    SerialUSB.print(" /HRARAD = ");
    SerialUSB.print(PmmSunCalc.HRARAD);
    SerialUSB.print(" /Zenith = ");
    SerialUSB.print(PmmSunCalc.Zenith);
    SerialUSB.print(" /Elevation = ");
    SerialUSB.print(PmmSunCalc.Elevation);
    SerialUSB.print(" /AzimuthRAD = ");
    SerialUSB.print(PmmSunCalc.AzimuthRAD);
    SerialUSB.print(" /Azimuth = ");
    SerialUSB.print(PmmSunCalc.Azimuth);
    SerialUSB.print(" /TrueAngle = ");
    SerialUSB.print(PmmSunCalc.TrueAngle);
    SerialUSB.print(" /TrueAngleNerl = ");
    SerialUSB.print(PmmSunCalc.TrueAngleNerl);
    SerialUSB.print(" /Backtracking Angle = ");
    SerialUSB.print(PmmSunCalc.BackTrackerAngle);

    SerialUSB.print(" /DateTimeUnix = ");
    SerialUSB.print(Pmmm1103.DateTime);
    String tmpstr = String(year(Pmmm1103.DateTime)) + "-" + String(month(Pmmm1103.DateTime)) + "-" + String(day(Pmmm1103.DateTime)) + " ";
    tmpstr = tmpstr + String(hour(Pmmm1103.DateTime)) + ":" + String(minute(Pmmm1103.DateTime)) + ":" + String(second(Pmmm1103.DateTime));
    SerialUSB.print(" /DateTime = ");
    SerialUSB.print(tmpstr);

    SerialUSB.print(" /SunriseUnix = ");
    SerialUSB.print(PmmSunCalc.Sunrise);

    SerialUSB.print(" /Sunrise = ");
    tmpstr = String(year(PmmSunCalc.Sunrise)) + "-" + String(month(PmmSunCalc.Sunrise)) + "-" + String(day(PmmSunCalc.Sunrise)) + " ";
    tmpstr = tmpstr + String(hour(PmmSunCalc.Sunrise)) + ":" + String(minute(PmmSunCalc.Sunrise)) + ":" + String(second(PmmSunCalc.Sunrise));
    SerialUSB.print(tmpstr);

    SerialUSB.print(" /SunsetUnix = ");
    SerialUSB.print(PmmSunCalc.Sunset);

    SerialUSB.print(" /Sunset = ");
    tmpstr = String(year(PmmSunCalc.Sunset)) + "-" + String(month(PmmSunCalc.Sunset)) + "-" + String(day(PmmSunCalc.Sunset)) + " ";
    tmpstr = tmpstr + String(hour(PmmSunCalc.Sunset)) + ":" + String(minute(PmmSunCalc.Sunset)) + ":" + String(second(PmmSunCalc.Sunset));
    SerialUSB.print(tmpstr);

    // DeclinationRAD =0 ;
    SerialUSB.print(" /DeclinationRAD = ");
    SerialUSB.print(PmmSunCalc.DeclinationRAD);

    SerialUSB.print(" /latRAD = ");
    SerialUSB.print(PmmSunCalc.latRAD);

    SerialUSB.println("");
}

#endif
