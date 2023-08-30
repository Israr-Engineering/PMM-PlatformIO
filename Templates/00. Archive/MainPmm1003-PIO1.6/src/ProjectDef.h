#ifndef PROJECTDEF_H
#define PROJECTDEF_H


//defines
#define PMMTypes
#define PMMCommands
#define PMMGlobalFunctions
//#define PMMhasExternalRTC
#define PMMInternalRTC
#define PMMModbus
//#define PMMSunCalculations
//#define PMMExtEEPROM
//#define PMMFlashMemory
//#define PMMUDP
//#define PMMMPU
//#define PMMBNO
//#define PMMI2C
//#define PmmHX711LoadCell
#define PmmPCA9535
#define PmmWatchDog
//#define PMMHasExtensions
//#define PMMTIMERS
//#define PMMGENERALPURPOSE
//#define PMMCALIBRATION
//#define PMMPOWERMANAGMENT
//#define PMMREADALLSETTINGS
#define PMMNUMOFSERIALINTERFACES 2
#define PMMMCUWITHCONFIG
//#define PMMHTTPSERVER
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

//Always Include this libraries 
// #include <PmmGlobalFunctions.h>
//#include <PmmCommands.h>


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









