#include <PmmFlashStorage.h>
#include "PmmTypes.h"

//#include <sstream>
#include <string>
//#include <vector>
using namespace std;




//std::vector<string> PMMStringToArray(string csvStr, char delimiter);
void PMMWriteIntoFlashAllSettings(string Message);
void PMMWriteIntoFlashGeneralSettings(string Message);
void PMMWriteIntoFlashSerialSettings(string Message);
void PMMWriteIntoFlashTCPSettings(string Message);
void PMMWriteIntoFlashOptionsAndPinsSettings(string Message);
string PMMReadFromFlashAllSettings();
void PmmStringToArray(string input);


void SetTCPSettings(string Message);
String GetTCPSettings();