
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

// Common INPUT Pins If any (Pin Mask)
static const uint8_t DILOSSPOWER = 10ul;  // PA20
static const uint8_t DIMANAUTO = 3ul;    // PA06
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
static const uint8_t DOLED = 9ul;      // PA13
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
PmmSunCalculations PmmSunCalc;

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


void hardwareInit()
{
    // STEP01 : Prepare IOs
    pinMode(DILOSSPOWER, INPUT);
    pinMode(DIMANAUTO, INPUT);
    pinMode(DIMANEAST, INPUT);
    pinMode(DIMANWEST, INPUT);
    pinMode(DILSFEEDBACK, INPUT);
    pinMode(DIPOSINT, INPUT);
    pinMode(DIPROG, INPUT_PULLUP);

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
    
    ModbusRTU01Server.PmmModBusRTUServerSetup(Pmmm1103.ModBusID, SettingSerial01, 9600, 35, 36, 31, 1); //  35,36,31
    ModbusRTU01Server.PmmModBusRTUServerconfigure(true, 0, 24, true, 0, 32, true, 0, 32, true, 0, 24);
    if (Pmmm1103.DeviceType == 1) // if master device config serial02 as client to read time from GPS if any 
    {
       ModbusRTU02Client.PmmModBusRTUClientSetup(SettingSerial01, 9600, 0, 1, 4, 2); //   PA10,PA11.PA07
    }    

    // STEP04 : Initialize MPU then read positions

    PmmSunCalc.PmmMPUSetup();
    //Pmmm1103.MPUTrueAngle = PmmSunCalc.UpdateCalculations() * .01 ;

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

/*
float PMMCalculateTC()
{
//Calculate B
int16_t dayNumber = PMMReturnDayOfYear();
    float B = (360.0 / 365.0) * (dayNumber - 81);
    B = B * PI / 180.0;

//Calculate EOT
float EOT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);

//Calculate LTSM
float LTSM = 15.0 * GetTimeZoneFromEEprom();

//Calculate TC
float TC = 4 * (GetLatitudeFromEEprom() - LTSM) + EOT;
return(TC);
}
*/
/*
float GetSunAngle(uint16_t TimeZone,float   Longitude, float Latitude ,int16_t dayNumber ,int currentTimeInMinute)
{
//uint16_t TimeZone=GetTimeZoneFromEEprom();
//float   Longitude=GetLongitudeFromEEprom();
//float Latitude=GetLatitudeFromEEprom();
//Calculate B
//int16_t dayNumber = PMMReturnDayOfYear();
    float B = (360.0 / 365.0) * (dayNumber - 81);
    B = B * PI / 180.0;

//Calculate EOT
float EOT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);

//Calculate LTSM
float LTSM = 15.0 * TimeZone;

//Calculate TC
float TC = 4 * (Longitude - LTSM) + EOT;

//Calculate LST
//int currentTimeInMinute =  Pmmm1103.RTCMinutes ; // PMMReturnInMinute();
float LST = currentTimeInMinute + TC;

//Calculate HRA
float HRA = 15 * (LST) / 60 + 180;
if (HRA > 180)
HRA = HRA - 360;
float HRARAD=DEG_TO_RAD*HRA;
// SerialUSB.println();
// SerialUSB.print("HRA :");
// SerialUSB.println(HRA);
int Angleinvers=1;
if (HRA<0)
    {
Angleinvers=-1;
    }
//Calculate Elevation
float DeclinationRAD = (23.45 * sin(B))*DEG_TO_RAD;
double latRAD = Latitude * PI / 180; // convert to Radians
double Elevation = asin((sin(DeclinationRAD) * sin(latRAD)) + (cos(latRAD) * cos(DeclinationRAD) * cos(HRARAD)));
double ElevationRAD = Elevation;
Elevation = Elevation * (180 / PI);

//Calculate Azimuth
float AzimuthRAD = acos((sin(DeclinationRAD) * cos(latRAD) - cos(DeclinationRAD) * sin(latRAD) * cos(HRARAD)) / cos(ElevationRAD));
float Azimuth=AzimuthRAD*RAD_TO_DEG;
//Calculate Zenith
float ZenithRAD = DEG_TO_RAD*(90 - Elevation);

//Calculate true angle
 float Ys = 180-Azimuth; // Ys
    float Yt = 360;
    Ys = DEG_TO_RAD*Ys;
    Yt = DEG_TO_RAD*Yt;
    float a = cos(Yt) * sin(ZenithRAD) * sin(Ys) - sin(Yt) * sin(ZenithRAD) * cos(Ys);
    float b = cos(ZenithRAD);
  
    float TrueAngle = Angleinvers* atan(a / b);
    TrueAngle =TrueAngle * RAD_TO_DEG;
    // float Max_Limit=GetMaxLimitFromEEprom();
    // float Min_Limit=GetMinLimitFromEEprom();
    // // if(TrueAngle>Max_Limit)
    // // {TrueAngle=Max_Limit;}
    // // if(TrueAngle<Min_Limit)
    // // {TrueAngle=Min_Limit;}
    return TrueAngle ;
}
*/

/*
uint32_t SunSet(int16_t dayNumber,float lat)
{
    //int16_t dayNumber = PMMReturnDayOfYear();
    float B = (360.0 / 365.0) * (dayNumber - 81);
    B = B * PI / 180.0;
    float DeclinationRAD = (23.45 * sin(B))*DEG_TO_RAD;
    float latRAD=lat*DEG_TO_RAD;
    DateTime RTCDateTiem = PMMRTCNOW();
    DateTime TimeAt12Am = DateTime(RTCDateTiem.year(), RTCDateTiem.month(), RTCDateTiem.day() + 1, 0, 0, 0);
    float tmp;
    tmp = ((double)1 / 15 * acos(tan(DeclinationRAD) * tan(latRAD)));
    tmp = tmp * 180 / PI;
    float TC =PMMCalculateTC();
    int sunset = TimeAt12Am.unixtime() - tmp * 60 * 60 - TC * 60;
    return sunset;
}

uint32_t SunRise()
{
    int16_t dayNumber = PMMReturnDayOfYear();
    float B = (360.0 / 365.0) * (dayNumber - 81);
    B = B * PI / 180.0;
    float DeclinationRAD = (23.45 * sin(B))*DEG_TO_RAD;
    float latRAD=GetLatitudeFromEEprom()*DEG_TO_RAD;
    DateTime RTCDateTiem = PMMRTCNOW();
    DateTime TimeAt12Am = DateTime(RTCDateTiem.year(), RTCDateTiem.month(), RTCDateTiem.day(), 0, 0, 0);
    float tmp;
    tmp = ((double)1 / 15 * acos(tan(DeclinationRAD) * tan(latRAD)));
    tmp = tmp * 180 / PI;
    float TC = PMMCalculateTC();
    // int sunset = TimeAt12Am.unixtime() - tmp * 60 * 60 - TC * 60;  // + PMM1103TimeZone * 60 * 60;
    int sunrise = TimeAt12Am.unixtime() + tmp * 60 * 60 - TC * 60; //+ PMM1103TimeZone * 60 * 60;
    return sunrise;
}

*/



#endif
