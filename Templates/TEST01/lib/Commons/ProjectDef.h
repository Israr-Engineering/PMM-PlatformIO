
#pragma region DEFINE

// PMMTypes
#define PMMTypes
// PMMCommands
#define PMMCommands
// PMMGlobalFunctions
#define PMMGlobalFunctions

// External  RTC
#define PMMExternalRTC
// Internal RTC
#define PMMInternalRTC
// ModbusRTUServer
#define PMMModbusRTUServer
// ModbusLightRTUServer
#define PMMModbusLightRTUServer
// ModbusRTUClient
#define PMMModbusRTUClient
// ModbusTCPServer
#define PMMModbusTCPServer
// Sun Calculations
#define PMMSunCalculations
// EEPROM
#define PMMEEPROM
// Flash Memory
#define PMMFlashMemory
// UDP
//#define PMMUDP
/// MPU
#define PMMMPU
// BNO
#define PMMBNO

//I2C 
#define PMMI2C


#pragma endregion


#pragma region INCLUDES

// 01. Includes
//#include <iostream>
#include <string>
#include <String>
#include <array>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <random>

#include <Wire.h>
#include <PmmSPI.h>

#include <PmmScheduler.h>
#include <PmmExternalEEPROM.h>
#include <PmmSPISerialFlash.h>
#include <PmmBNO055.h>
#include <PmmInternalRTC.h>
#include <PmmPID_v1.h>
#include <PmmDS3231.h>
//#include <PmmSunCalculations.h>
#ifdef PMMI2C
#include <PmmRS485Lib.h>
#include <PmmArduinoModbus.h>
#include <PmmFlashAsEEPROM.h>
#include <PmmFlashStorage.h>

#include <PmmEthernet.h>
#include <PmmEthernetUDP.h>
#endif

#include <PmmHTTPRequest.hPP>
#include <PmmMPU6050_6Axis_MotionApps612.h>



// 02. Using
using namespace std;
using std::basic_string;
using std::char_traits;

#pragma endregion

//#pragma region VARIABLES

typedef bool bit;



//#pragma endregion