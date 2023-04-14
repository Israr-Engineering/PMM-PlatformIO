#include <PmmFlashStorage.h>
#include "PmmTypes.h"

//#include <sstream>
#include <string>
//#include <vector>
using namespace std;

// Old functions
void PmmStringToArray(string input);
string PMMIsAlive();
void PmmConvertDecimalToBinary(int Dic);
// New functions
void PmmWriteGeneralSettings(string Message);
void PmmWriteRTUSettings(string Message);
void PmmReadGeneralSettings();
void PmmReadRTUSettings();

// void SetProductSettings(string Message);
// String GetProductSettings();
// void SetTCPSettings(string Message);
// String GetTCPSettings();