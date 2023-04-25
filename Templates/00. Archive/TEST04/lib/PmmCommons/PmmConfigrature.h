#include <PmmFlashStorage.h>
#include "PmmTypes.h"

//#include <sstream>
#include <string>
//#include <vector>
using namespace std;

// Old functions
void PmmStringToArray(string input);
void PMMIsAlive();
void PmmConvertDecimalToBinary(int Dic);
// New functions
void PmmWriteGeneralSettingsInternalFlash(string Message);
void PmmWriteRTUSettingsInternalFlash(string Message);
void PmmWriteTCPUDPSettingsInternalFlash(string Message);
void PmmWriteModbusSettingsInternalFlash(string Message);
void PmmWriteTimerSettingsInternalFlash(string Message);
void PmmReadGeneralSettingsInternalFlash();
void PmmReadRTUSettingsInternalFlash();
void PmmReadTCPUDPSettingsInternalFlash();
void PmmReadModbusSettingsInternalFlash();
void PmmReadTimersSettingsInternalFlash();
