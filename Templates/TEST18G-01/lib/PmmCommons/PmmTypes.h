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
    long NumberOfCycles = 0;
    long LastRunningTime = 0;
    long NumberOfRunningTimes = 0;
    int SoftwareVersion = 1; // 12 => 1.2
    int FirmwareVersion = 1; // 12 => 1.2
    int HardwareVersion = 1; // 23 => 2.3
    int ControlerType = 0;

    bit ItHasEthernet = false; // add this
    bit ItHasSwitch = false;
    bit ItHasExtEEPROM = false;
    bit ItHasExtFlash = false;
    bit ItHasSerial = false;
    bit ItHasWebServer = false;
    bit ItHasFiber = false;
    bit LoRA = false;

    bit Zidbee = false;
    bit GSM = false;
    bit GPS = false;
    bit Antenna = false;
    bit ExternalRTC = false;
    bit InternalRTC = false;
    bit UDPOption = false;
    bit GateWay = false;

    bit IsModBus = false;
    bit IsCanBus = false;
    bit IsProfiBus = false;
    bit IsProfiNet = false;
    bit IsBACnet = false;
    bit IsDLMS = false;
    bit IsMBus = false;
    bit IsOPC = false;

    bit Canprint = false; // bit for bebug mode

    bit spare01 = false;
    bit spare02 = false;
    bit spare03 = false;
    bit spare04 = false;
    bit spare05 = false;
    bit spare06 = false;
    bit spare07 = false;

    int NumberOfInputs = 0;
    int NumberOfOutputs = 0;
    int NumberOfSerials = 0;
    int NumberOfUDPPorts = 0;
    int MinReadValue = 0;
    int MaxReadValue = 0;
    int OprationVoltage = 0;
    int GeneralReadingsOffset = 0;
    int GeneralReadingsFactor = 0;

    int SettingsRef = 1; // 1 =>Internal Flash,2=>External Flash,3=>EEPROM

    int Spare01 = 0;
    int Spare02 = 0;

} PMMGeneral;

typedef struct PMMModBusPort
{
    /* data */

    // system memorey

    // ModBus Settings
    bit ModBusRTU = false;
    bit ModBusTCP = false;
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
    bit CoilsStatus = false;
    bit InputStatus = false;
    bit HoldingRegisters = false;
    bit InputRegisters = false;

    int StartingAddressCoilsStatus = 0; // 02
    int StartingAddressInputStatus = 0; // 03
    int StartingAddressHoldingRegisters = 0; //04
    int StartingAddressInputRegisters = 0; //05
    int StartingAddressWriteSingleCoil = 0; // 06
    int StartingAddressWriteSingleRegister = 0; // 07
    int StartingAddressWriteMultipleCoils = 0; // 08
    int StartingAddressWriteMultipleRegisters = 0; //09
    int QuantityCoilsStatus = 0; // 10
    int QuantityInputStatus = 0; // 11
    int QuantityHoldingRegisters = 0; // 12
    int QuantityInputRegisters = 0; // 13
    int QuantityWriteMultipleCoils = 0; // 14
    int QuantityWriteMultipleRegisters = 0; // 15
    int FunctionCode = 01;// 16
    int IODataOrder = 1; // 17
    int IODataType = 1; // 18
    int PollInterval = 0; // 19
    int SlaveID = 1; // 20
    int PortFourConnectionTimeout = 5000; // 21
    int PortFourMaxRetry = 5; // 22

    int Spare23 = 0;
    int Spare24 = 0;
    int Spare25 = 0;
    int Spare26 = 0;
    int Spare27 = 0;
    int Spare28 = 0;
    int Spare29 = 0;
    int Spare30 = 0;
    int Spare31 = 0;
    int Spare32 = 0;


} PMMModBusPort;


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
    int MacAddress06 = 60;
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

    int Spare01 = 0;

    // Modbus
    PMMModBusPort PmmModBus;

} PMMTCPUDP;



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

    int Spare01 = 0;
    int Spare02 = 0;
    int Spare03 = 0;
    int Spare04 = 0;
    int Spare05 = 0;
    int Spare06 = 0;
    int Spare07 = 0;
    int Spare08 = 0;
    int Spare09 = 0;
    int Spare10 = 0;
    int Spare11 = 0;
    int Spare12 = 0;
    int Spare13 = 0;
    int Spare14 = 0;
    int Spare15 = 0;
    int Spare16 = 0;
    int Spare17 = 0;
    int Spare18 = 0;
    int Spare19 = 0;
    int Spare20 = 0;
    int Spare21 = 0;
    int Spare22 = 0;
    int Spare23 = 0;
    int Spare24 = 0;
    int Spare25 = 0;
    int Spare26 = 0;
    int Spare27 = 0;
    int Spare28 = 0;
    int Spare29 = 0;
    int Spare30 = 0;
    int Spare31 = 0;

} PMMTimer;

typedef struct PMMDeviceCalibration
{
    /*
    this is a spescial settings for the device itself
    */
    float Calebrate00 = 0;
    float Calebrate01 = 0;
    float Calebrate02 = 0;
    float Calebrate03 = 0;
    float Calebrate04 = 0;
    float Calebrate05 = 0;
    float Calebrate06 = 0;
    float Calebrate07 = 0;
    float Calebrate08 = 0;
    float Calebrate09 = 0;
    float Calebrate10 = 0;
    float Calebrate11 = 0;
    float Calebrate12 = 0;
    float Calebrate13 = 0;
    float Calebrate14 = 0;
    float Calebrate15 = 0;
   

} PMMDeviceCalibration;

typedef struct GerneralPurpose
{
    /*
    this is a spescial settings for the device itself
    */
    int Spare00 = 0;
    int Spare01 = 0;
    int Spare02 = 0;
    int Spare03 = 0;
    int Spare04 = 0;
    int Spare05 = 0;
    int Spare06 = 0;
    int Spare07 = 0;
    int Spare08 = 0;
    int Spare09 = 0;
    int Spare10 = 0;
    int Spare11 = 0;
    int Spare12 = 0;
    int Spare13 = 0;
    int Spare14 = 0;
    int Spare15 = 0;
    int Spare16 = 0;
    int Spare17 = 0;
    int Spare18 = 0;
    int Spare19 = 0;
    int Spare20 = 0;
    int Spare21 = 0;
    int Spare22 = 0;
    int Spare23 = 0;
    int Spare24 = 0;
    int Spare25 = 0;
    int Spare26 = 0;
    int Spare27 = 0;
    int Spare28 = 0;
    int Spare29 = 0;
    int Spare30 = 0;
    int Spare31 = 0;

} GerneralPurpose;

typedef struct PMMSERIAL
{
    int PortStopBit = 1; // 0
    int PortDataBit = 8; // 1
    int PortParity = 0; //  2 : 0 => Non , 1 => Odd , 2 => Even
    int SerialConfig = 8; // 3 : 23 u_int16_t config = (databitLong | parityLong | stopbitLong);

    long BaudRate = 9600; // 4

    int TXPin = 35; // 5
    int RXPin = 36;// 6
    int SerialSelectionPin = 1; // 7
    int SerialPort = 1; // 8
    int PortFourInterface = 485; // 9

    int Spare10 = 0;
    int Spare11 = 0;
    int Spare12 = 0;
    int Spare13 = 0;
    int Spare14 = 0;
    int Spare15 = 0;
    int Spare16 = 0;
    int Spare17 = 0;
    int Spare18 = 0;
    int Spare19 = 0;
    int Spare20 = 0;
    int Spare21 = 0;
    int Spare22 = 0;
    int Spare23 = 0;
    int Spare24 = 0;
    int Spare25 = 0;
    int Spare26 = 0;
    int Spare27 = 0;
    int Spare28 = 0;
    int Spare29 = 0;
    int Spare30 = 0;

    //int Spare31 = 0;
    bit spare3100 = false;
    bit spare3101 = false;
    bit spare3102 = false;
    bit spare3103 = false;
    bit spare3104 = false;
    bit spare3105 = false;
    bit spare3106 = false;
    bit spare3107 = false;

    bit spare3108 = false;
    bit spare3109 = false;
    bit spare3110 = false;
    bit spare3111 = false;
    bit spare3112 = false;
    bit spare3113 = false;
    bit spare3114 = false;
    bit spare3115 = false;
    

    // modbus
    PMMModBusPort PmmModBus;


}PMMSERIAL;


typedef struct Product
{

    // General Flages
    bool EthernetRunning = false;
    bool SerialRunning = false;
    bool WireRunning = false;
    bool I2CRunning = false;
    bool SPIRunning = false;
    bool ModbusRTUServerRunning = false ;
    bool ModbusTCPServerRunning = false ;
    bool ModbusRTUClientRunning = false ;
    bool ModbusTCPClientRunning = false ;
    bool InternalRTCRunning = false ;
    bool ExternalRTCRunning = false ;

    // Settings Stucts
    PMMGeneral PmmGeneral;

    PMMTCPUDP PmmTCPUDP;
    
    PMMSERIAL PmmSerial01;
    PMMSERIAL PmmSerial02;
    PMMSERIAL PmmSerial03;
    PMMSERIAL PmmSerial04;
     
    PMMTimer PmmTimers;

    PMMDeviceCalibration PmmCalibrationPage01;
    PMMDeviceCalibration PmmCalibrationPage02;
    
    GerneralPurpose PmmGerneralPurpose;

} Product;

typedef struct PMMIO
{



// represent descret InputStatus in modbus
byte InputsByte[120];
// image for internal use
byte InputsBytePrev[120];
// represent coils bits in modbus
byte OutputsByte[120];
// image for internal use
byte OutputsBytePrev[120];

// represent Input registers in modbus
int Inputs[120];
// image for internal use
int InputsPrev[120];
// represent hodling registers in modbus
int Outputs[120];
// image for internal use
int OutputsPrev[120];

}PMMIO;


#endif
