// #include "ProjectDef.h"

#ifndef PMMTYPES_H 
#define PMMTYPES_H

#include <string>
#include <vector>
using namespace std;

typedef bool bit;

enum PmmModBusFunction
{
    CoilStatus = 01,
    InputStatus = 02,
    HoldingRegister = 03,
    InputRegister = 04,
    WriteSingleCoil = 05,
    WriteSingleRegister = 06,
    WriteMultipleCoils = 15,
    WriteMultipleRegisters = 16
};

// ----------------------- New Settings ---------------------------------------------------

typedef struct PMMGeneral
{

    /* data */
    // system memorey

    // Device Information
    int DeviceName = 0620;
    long SerialNumber = 5000;
    long FirstTimeRun = 0;
    long LifeTime = 0;
    long NumberOfCycles =0;
    long LastRunningTime = 0;
    long NumberOfRunningTimes =0 ;
    int SoftwareVersion = 1; // 12 => 1.2
    int FirmwareVersion = 1; // 12 => 1.2
    int HardwareVersion = 1; // 23 => 2.3
    bit WebServer = false;
    bit CaseType = false;
    bit FiberOption = false;
    bit SingleModeFiber = false;
    bit MultiModeFiber = false;
    bit GSM = false;
    bit GPS = false;
    bit Antenna = false;
    bit ExternalRTC = false;
    bit InternalRTC = false;
    bit UDPOption = false;
    bit GateWay = false;
    int ControlerType = 0;
    int MinOprationTemperature = 0;
    int MaxOprationTemperature = 0;
    int NumberOfInputs = 0;
    int NumberOfOutputs = 0;
    int NumberOfSerials = 0;
    int NumberOfUDPPorts = 0;
    int MinReadValue = 0;
    int MaxReadValue = 0;
    int OprationVoltage = 0;
    int GeneralReadingsOffset = 0;
    int GeneralReadingsFactor = 0;

} PMMGeneral;

typedef struct PMMRTU
{
    int PortOneName = 1;
    int PortOneBaudRate = 9600;
    int PortOneStopBit = 1;
    int PortOneDataBit = 8;
    int PortOneParity = 0; // 0 => Non , 1 => Odd , 2 => Even
    int PortOneConnectionTimeout = 5000;
    int PortOneMaxRetryRTU = 5;
    int PortOneInterface = 485;
    int PortTwoName = 1;
    int PortTwoBaudRate = 9600;
    int PortTwoStopBit = 1;
    int PortTwoDataBit = 8;
    int PortTwoParity = 0; // 0 => Non , 1 => Odd , 2 => Even
    int PortTwoConnectionTimeout = 5000;
    int PortTwoMaxRetryRTU = 5;
    int PortTwoInterface = 485;
    int PortThreeName = 1;
    int PortThreeBaudRate = 9600;
    int PortThreeStopBit = 1;
    int PortThreeDataBit = 8;
    int PortThreeParity = 0; // 0 => Non , 1 => Odd , 2 => Even
    int PortThreeConnectionTimeout = 5000;
    int PortThreeMaxRetryRTU = 5;
    int PortThreeInterface = 485;
    int PortFourName = 1;
    int PortFourBaudRate = 9600;
    int PortFourStopBit = 1;
    int PortFourDataBit = 8;
    int PortFourParity = 0; // 0 => Non , 1 => Odd , 2 => Even
    int PortFourConnectionTimeout = 5000;
    int PortFourMaxRetryRTU = 5;
    int PortFourInterface = 485;

} PMMRTU;

typedef struct PMMTCPUDP
{
    int MacAddress01 = 10;
    int MacAddress02 = 20;
    int MacAddress03 = 30;
    int MacAddress04 = 40;
    int MacAddress05 = 50;
    int IPAddress01 = 192;
    int IPAddress02 = 168;
    int IPAddress03 = 1;
    int IPAddress04 = 100;
    int RemoteIPAddress01 = 192;
    int RemoteIPAddress02 = 168;
    int RemoteIPAddress03 = 1;
    int RemoteIPAddress04 = 100;
    int NetMask01 = 255;
    int NetMask02 = 255;
    int NetMask03 = 255;
    int NetMask04 = 255;
    int DNSSOneServer01 = 8;
    int DNSSOneServer02 = 8;
    int DNSSOneServer03 = 8;
    int DNSSOneServer04 = 8;
    int DNSTwoServer01 = 8;
    int DNSTwoServer02 = 8;
    int DNSTwoServer03 = 0;
    int DNSTwoServer04 = 0;
    int ConnectionTimeoutTCP = 5000;
    int MaxRetryTCP = 5;
    int UDPPortOne = 90;
    int UDPPortTwo = 91;
    int UDPPortThree = 92;
    int UDPPortFour = 93;

} PMMTCPUDP;

typedef struct PMMModBus
{
    /* data */

    // system memorey

    // ModBus Settings
    bit ModBusRTU = false;
    bit ModBusTCP = false;
    bit ModBusUDP = false;
    bit ModBusSlave = false;
    bit ModBusMaster = false;
    bit ReadCoilsStatus = false;
    bit ReadInputStatus = false;
    bit ReadHoldingRegisters = false;
    bit ReadInputRegisters = false;
    bit WriteSingleCoil = false;
    bit WriteSingleRegister = false;
    bit WriteMultipleCoils = false;
    bit WriteMultipleRegisters = false;
    int StartingAddressCoilsStatus = 0;
    int StartingAddressInputStatus = 0;
    int StartingAddressHoldingRegisters = 0;
    int StartingAddressInputRegisters = 0;
    int StartingAddressWriteSingleCoil = 0;
    int StartingAddressWriteSingleRegister = 0;
    int StartingAddressWriteMultipleCoils = 0;
    int StartingAddressWriteMultipleRegisters = 0;
    int QuantityCoilsStatus = 0;
    int QuantityInputStatus = 0;
    int QuantityHoldingRegisters = 0;
    int QuantityInputRegisters = 0;
    int QuantityWriteMultipleCoils = 0;
    int QuantityWriteMultipleRegisters = 0;
    int FunctionCode = 01;
    int IODataOrder = 1;
    int IODataType = 1;
    int PollInterval = 0;
    int SlaveID = 1;
    int StartingAddress = 0;
    int Quantity = 0;

} PMMModBus;

typedef struct PMMTimer
{
    /* data */

    // system memorey

    // Timers Settings
    bit CycleTimer = false;   // INDEX => 500 ,Flip Flop Timer Every Cycle
    bit OneMSTimer = false;   // INDEX => 500.125 ,Flip Flop Timer Every One MiliSeconds
    bit TenMSTimer = false;   // INDEX => 500.250 ,Flip Flop Timer Every Ten MiliSeconds
    bit OneSecTimer = false;  // INDEX => 500.375 ,Flip Flop Timer Every One Second
    bit OneMinTimer = false;  // INDEX => 500.500 ,Flip Flop Timer Every One Minute
    bit OneHouTimer = false;  // INDEX => 500.625 ,Flip Flop Timer Every One Hour
    bit OneMonTimer = false;  // INDEX => 500.750 ,Flip Flop Timer Every One Month
    bit OneYearTimer = false; // INDEX => 500.875 ,Flip Flop Timer Every One Year

} PMMTimer;

typedef struct Product
{

    PMMGeneral PmmGeneral;
    PMMRTU PmmRTU;
    PMMTCPUDP PmmTCPUDP;
    PMMModBus PmmModbus;
    PMMTimer PmmTimers;

} Product;

#endif
