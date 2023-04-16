#ifndef PMMCOMMANDS_H 
#define PMMCOMMANDS_H

#include <PmmTypes.h>
#define BowlingLed 5
#define StatekLed 4
//declare the existence of this global variable.

extern Product ThisProduct;

 // Common functions
extern void PmmStringToArray(string input);
extern void PMMIsAlive();
extern void PmmConvertDecimalToBinary(int Dic);
//Internal flash ROM functions
extern void PmmWriteGeneralSettingsInternalFlash(string Message);
extern void PmmWriteRTUSettingsInternalFlash(string Message);
extern void PmmWriteTCPUDPSettingsInternalFlash(string Message);
extern void PmmWriteModbusSettingsInternalFlash(string Message);
extern void PmmWriteTimerSettingsInternalFlash(string Message);
extern void PmmReadGeneralSettingsInternalFlash();
extern void PmmReadRTUSettingsInternalFlash();
extern void PmmReadTCPUDPSettingsInternalFlash();
extern void PmmReadModbusSettingsInternalFlash();
extern void PmmReadTimersSettingsInternalFlash();


// Command
 extern void PMMInitializeEthernet(IPAddress ip, byte mac[]);
 extern string PMMReadCommands();
 extern string PMMCommnads(string readData);
 extern string PMMReturnDataFromSerialUSB();
 //string PMMReturnDataFromAPIHTTPHeader();

 extern void PmmReadAllSettingsInternalFlash();

 extern void PmmWriteGeneralSettingsEEPROM(string Message);


#endif