
// PMMTypes
#define PMMTypes
// PMMCommands
#define PMMCommands
// PMMGlobalFunctions
#define PMMGlobalFunctions
// External  RTC
//#define PMMExternalRTC
// Internal RTC
//#define PMMInternalRTC
// ModbusRTUServer
//#define PMMModbusRTUServer
// ModbusLightRTUServer
//#define PMMModbusLightRTUServer
// ModbusRTUClient
//#define PMMModbusRTUClient
// ModbusTCPServer
//#define PMMModbusTCPServer
// Sun Calculations
//#define PMMSunCalculations
// EEPROM
//#define PMMEEPROM
// Flash Memory
#define PMMFlashMemory
// UDP
//#define PMMUDP
/// MPU
//#define PMMMPU
// BNO
//#define PMMBNO
//I2C 
//#define PMMI2C
//Load Cell
//#define PmmLoadCell
//PmmPCA9535
//#define PmmPCA9535
//Watch Dog
//#define PmmWatchDog

// 01. Includes
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
#include <PmmPID_v1.h>
#include <PmmRS485Lib.h>
#include <PmmEthernet.h>
#include <PmmTimeLib.h>

#ifdef PMMExternalRTC
#include <PmmDS3231.h>
#endif

#ifdef PMMInternalRTC
#include <PmmInternalRTC.h>
#endif

#if defined(PMMModbusRTUServer) || defined(PMMModbusRTUClient) || defined(PMMModbusTCPServer)
#include <PmmArduinoModbus.h>
#endif

#ifdef PMMEEPROM
#include <PmmExternalEEPROM.h>
#endif

#ifdef PMMFlashMemory
#include <PmmFlashAsEEPROM.h>
#include <PmmFlashStorage.h>
#endif

#ifdef PMMUDP
#include <PmmEthernetUDP.h>
#endif

#ifdef PMMBNO
#include <PmmBNO055.h>
#endif

#ifdef PMMMPU
#include <PmmMPU6050_6Axis_MotionApps612.h>
#endif

#ifdef PmmLoadCell
#include "PmmHX711.h"
#endif

#ifdef PMMI2C2000
#include <PmmEthernet2000.h>
#endif

#ifdef PmmPCA9535
#include <PmmPCA9535.h>
#endif

#ifdef PmmWatchDog
#include <PmmWDTZero.h>
#endif

// 02. Using
using namespace std;
using std::basic_string;
using std::char_traits;

typedef bool bit;



