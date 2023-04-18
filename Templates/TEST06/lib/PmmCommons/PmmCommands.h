#ifndef PMMCOMMANDS_H
#define PMMCOMMANDS_H

#include <PmmTypes.h>

#define BowlingLed 5
#define StatekLed 4
// declare the existence of this global variable.

extern Product ThisProduct;


// Common functions
extern void PmmStringToArray(string input);
extern String PMMIsAlive();
extern void PmmConvertDecimalToBinary(int Dic);
extern void PmmSetEthernetSettings();

// Internal flash ROM functions
extern void PmmWriteGeneralSettingsInternalFlash(string Message);
extern void PmmWriteRTUSettingsInternalFlash(string Message);
extern void PmmWriteTCPUDPSettingsInternalFlash(string Message);
extern void PmmWriteModbusSettingsInternalFlash(string Message);
extern void PmmWriteTimerSettingsInternalFlash(string Message);
extern String PmmReadGeneralSettingsInternalFlash();
extern String PmmReadRTUSettingsInternalFlash();
extern String PmmReadTCPUDPSettingsInternalFlash();
extern String PmmReadModbusSettingsInternalFlash();
extern String PmmReadTimersSettingsInternalFlash();
extern void PmmReadAllSettingsInternalFlash();

// Command
extern void PMMInitializeEthernet(IPAddress ip, byte mac[]);

extern void StartCommandHttpServer();
extern void StartEEprom();

extern void PMMReadCommands();
extern String PMMCommnads(string readData);
extern String PMMReturnDataFromSerialUSB();
extern String PMMReturnDataFromAPIHTTPHeader();
extern void PMMSendDataHTTPClient(String Data);

// EEPROM
extern void PmmWriteGeneralSettingsEEPROM(string Message);
extern void PmmWriteRTUSettingsEEPROM(string Message);
extern void PmmWriteTCPUDPSettingsEEPROM(string Message);
extern void PmmWriteModbusSettingsEEPROM(string Message);
extern void PmmWriteTimerSettingsEEPROM(string Message);
extern String PmmReadGeneralSettingsEEPROM();
extern String PmmReadRTUSettingsEEPROM();
extern String PmmReadTCPUDPSettingsEEPROM();
extern String PmmReadModbusSettingsEEPROM();
extern String PmmReadTimersSettingsEEPROM();
extern void PmmReadAllSettingsEEPROM();

#endif