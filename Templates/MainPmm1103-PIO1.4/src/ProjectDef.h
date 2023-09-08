#ifndef PROJECTDEF_H
#define PROJECTDEF_H

#define TESTMODEONLY
//defines
#define PMMTypes
#define PMMCommands
//#define PMMGlobalFunctions       // if you need global functions 
//#define PMMhasExternalRTC          // Enable External RTC
#define PMMInternalRTC            // Enable Internal RTC
#define PMMModbus                 // Enable WatchDog Modbus
//#define PMMSunCalculations         // Enable Sun Angle Calculation
//#define PMMExtEEPROM
//#define PMMFlashMemory
//#define PMMUDP                    // Enable MPU I2c Reading
//#define PMMMPU                    // This is for advance using of MPU chip
//#define PMMBNO
//#define PMMI2C
//#define PmmHX711LoadCell
//#define PmmPCA9535
#define PmmWatchDog                // Enable WatchDog timer 
//#define PMMHasExtensions
//#define PMMTIMERS
//#define PMMGENERALPURPOSE
//#define PMMCALIBRATION
//#define PMMPOWERMANAGMENT
//#define PMMREADALLSETTINGS
#define PMMNUMOFSERIALINTERFACES 2 // Number of serial ports including Ethernet
//#define PMMMCUWITHCONFIG           // Enable Configration Thread 
//#define PMMHTTPSERVER              // Enable Http Server
//#define PMMCOMMTHREAD





#if defined PMMModbus
#include <PmmModbus.h>
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

#ifdef PmmHX711LoadCell
#include "PmmHX711.h"
PmmHX711 MyLoadCell;
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

#ifdef PMMSunCalculations
#include <PmmSunCalculations.h>
#endif

#ifdef PMMHasExtensions
#include <PMM06XXExtesions.h>
#endif

// 01. Includes
// #include <String>
// #include <array>
// #include <cstdlib>
// #include <chrono>
// #include <ctime>
// #include <cmath>
// #include <iomanip>
// #include <sstream>
#include <random>

#include <Wire.h>
#include <PmmSPI.h>
#include <PmmScheduler.h>
#include <PmmPID_v1.h>
#include <PmmRS485Lib.h>
#include <PmmEthernet.h>
#include <PmmTimeLib.h>

// 02. Using
using namespace std;
// using std::basic_string;
// using std::char_traits;

typedef bool bit;

// Global Functions 
void PMMConfiguration();
void PMMCommunication();
void PMMTimers();





//Global Variables
long MainLoopTimer = 0;
long ConfigurationTimer = 0;
long CommunicationTimer = 0;


#endif









