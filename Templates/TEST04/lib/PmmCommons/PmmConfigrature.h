#include <PmmFlashStorage.h>
#include "PmmTypes.h"

//#include <sstream>
#include <string>
//#include <vector>
using namespace std;




//Old functions 
void PMMWriteIntoFlashAllSettings(string Message);
void PMMWriteIntoFlashGeneralSettings(string Message);
void PMMWriteIntoFlashSerialSettings(string Message);
void PMMWriteIntoFlashTCPSettings(string Message);
void PMMWriteIntoFlashOptionsAndPinsSettings(string Message);
String PMMReadFromFlashAllSettings();


// new functions in use 
void PmmStringToArray(string input);
void SetProductSettings(string Message);
String GetProductSettings();
void SetTCPSettings(string Message);
String GetTCPSettings();