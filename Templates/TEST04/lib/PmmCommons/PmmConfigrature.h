#include <PmmFlashStorage.h>
#include "PmmTypes.h"

#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct PmmGeneralSettings PMMGENERALSETTINGS;
struct PmmModBusSettings PMMMODBUSSETTINGS;
struct PmmTCPUDPSettings PMMTCPUDPSETTINGS;
struct PmmSerialSettings PMMSERIALSETTINGS;
struct PmmIOPins PMMIOPINS;
struct PmmExtensionsSettings PMMEXTENSIONSSETTINGS;


std::vector<string> PMMStringToArray(string csvStr, char delimiter);
void PMMWriteIntoFlashAllSettings(string Message);
void PMMWriteIntoFlashGeneralSettings(string Message);
void PMMWriteIntoFlashSerialSettings(string Message);
void PMMWriteIntoFlashTCPSettings(string Message);
void PMMWriteIntoFlashOptionsAndPinsSettings(string Message);
string PMMReadFromFlashAllSettings();
