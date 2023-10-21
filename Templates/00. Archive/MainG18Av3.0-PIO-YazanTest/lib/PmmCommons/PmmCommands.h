#ifndef PMMCOMMANDS_H
#define PMMCOMMANDS_H

#include <PmmTypes.h>
#include <PmmInternalRTC.h>

#include <PmmDS3231.h>

#define BowlingLed 5
#define StatekLed 4
// declare the existence of this global variable.

extern Product ThisProduct;
extern PMMIO PmmIO;
extern PmmInternalRTC PmmRTCInternal;
extern PmmDS3231 PmmRTCExternal;

extern uint8_t FACTORYRESET ;

// Common functions
extern void PmmStringToArray(string input);
extern String PMMIsAlive();


// ROM functions RomTarget => 0 : internal / 1 : ext eeprom / 2 : ext flash
extern String PmmWriteGeneralSettings(string Message , int RomTarget = 0);
extern String PmmReadGeneralSettings(uint8_t RomTarget = 0);

extern String PmmWriteSerialSettings(string Message , int Portnumber, int RomTarget = 0);
extern String PmmReadSerialSettings( int Portnumber, int RomTarget = 0);

extern String PmmWriteProtocol(string Message , int Portnumber, int RomTarget = 0);
extern String PmmReadProtocol( int Portnumber, int RomTarget = 0);

extern String PmmWriteTimerSettings(string Message, int RomTarget = 0);
extern String PmmReadTimersSettings(int RomTarget = 0);

extern String PmmWriteGerneralPurpose(string Message, int RomTarget = 0);
extern String PmmReadGerneralPurpose(int RomTarget = 0);

extern String PmmWriteDeviceCalibration(string Message,int PageNumber, int RomTarget = 0);
extern String PmmReadDeviceCalibration(int PageNumber,int RomTarget = 0 , long floatfactor = 100000);

extern void PmmReadAllSettings(int RomTarget = 0);

// Command

extern void StartCommandHttpServer();
extern void PMMReadCommands();
// extern void PMMWebCommunication();
extern String PMMCommnads(string readData);
extern String PMMReturnDataFromSerialUSB();
extern int BytesToInt(byte IntLowByte, byte IntHighByte);

// RTC
String SetInternalRTC(string Message);
String GetInternalRTC();
String SetExternalRTC(string Message);
String GetExternalRTC();


#endif