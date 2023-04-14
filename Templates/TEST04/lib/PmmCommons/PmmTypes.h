// #include "ProjectDef.h"

#include <string>
#include <vector>
using namespace std;

typedef bool bit;

typedef struct PmmGeneralSettings
{

    /* data */
    // system memorey

    // Device Information
    string Name = "";                  // INDEX => 0  , Device Name
    string Description = "";           // INDEX => 2  , Device Description
    int32_t SerialNumber = 0;          // INDEX => 4  , Device Serial Number
    int32_t FirstTimeRun = 0;          // INDEX => 8  , First Time Run (UTC/unix)
    int32_t LifeTime = 0;              // INDEX => 12 , Life Time (UTC/unix)
    int32_t NumberOfCycle = 0;         // INDEX => 16 , Number Of Cycle
    int32_t LastRunningTime = 0;       // INDEX => 20 , Last Running Time (UTC/unix)
    int32_t NumberOfRunningsTimes = 0; // INDEX => 24 , Number Of Runnings Times
    string SoftwareVersion = "";       // INDEX => 28 , Software Version
    string FirmwareVersion = "";       // INDEX => 30 , Software Version
    string HardwareVersion = "";       // INDEX => 32 , Software Version

    string ControllerType = "";       // INDEX => 46 , Controller Type
    float MinOperationTempreture = 0; // INDEX => 48
    float MaxOperationTempreture = 0; // INDEX => 52
    int16_t NumberOfInputs = 0;       // INDEX => 56
    int16_t NumberOfOutputs = 0;      // INDEX => 58
    int16_t NumberOfSerialPorts = 0;  // INDEX => 60
    int16_t NumberOfUDPPorts = 0;     // INDEX => 62
    float MinReadValue = 0;           // INDEX => 64
    float MaxReadValue = 0;           // INDEX => 68
    float OperationVoltage = 0;       // INDEX => 72
    float GeneralReadingsOffset = 0;  // INDEX => 76
    float GeneralReadingsFactor = 0;  // INDEX => 80
    int16_t TCPorRTU = 0;             // INDEX => 84

    // new variables
    string CPUType = "";
    string ProductFamily = "";
    string EnclosureType = "";

    bool ArduinoSupport = false;
    bool PlatformIOSupport = false;
    string OtherSupportName = "";

    bool HasEthernet = false;
    bool HasFiber = false;
    bool HasSwitch = false;
    bool HasExternalRTC = false;
    bool HasInternalRTC = false;
    bool HasEEprom = false;
    bool HasInternalFlash = false;
    bool HasExternalFlash = false;
    bool WebServer = false;       // INDEX => 34 , If true => web server   Else If false => no web server
    bool CaseType = false;        // INDEX => 35 , If true => metal Else If false => blastic
    bool FiberOption = false;     // INDEX => 36 , If true => fiber        Else If false => no fiber
    bool SingleModeFiber = false; // INDEX => 37 , If true => single mode  Else If false => no single mode
    bool MultiModeFiber = false;  // INDEX => 38 , If true => multi mode   Else If false => no multi mode
    bool GSM = false;             // INDEX => 39 , If true => GSM          Else If false => no GSM
    bool GPS = false;             // INDEX => 40 , If true => GPS          Else If false => no GPS
    bool Antenna = false;         // INDEX => 41 , If true => Antenna      Else If false => no Antenna
    bool ExternalRTCAva = false;  // INDEX => 42 , If true => External RTC Else If false => no External RTC
    bool InternalRTCAva = false;  // INDEX => 43 , If true => Internal RTC Else If false => no Internal RTC
    bool UDPOption = false;       // INDEX => 44 , If true => UDP          Else If false => no UDP
    bool GateWay = false;         // INDEX => 45 , If true => GateWay      Else If false => no GateWay

} PmmGeneralSettings;

typedef struct PmmSerialSettings
{
    /* data */
    // system memorey

    // Serial Settings
    string PortOneName = "";          // INDEX => 100
    int16_t PortOneBaudRate = 9600;   // INDEX => 102
    int16_t PortOneStopBits = 1;      // INDEX => 104
    int16_t PortOneDataBits = 8;      // INDEX => 106
    string PortOneParity = "None";    // INDEX => 108
    int32_t PortOneConnTimeOut = 0;   // INDEX => 110
    int8_t PortOneMaxRetries = 8;     // INDEX => 114
    bool PortOneInterface = false;    // INDEX => 115 , If true => RS485 Else If false RS232
    string PortOTowName = "";         // INDEX => 116
    int16_t PortTowBaudRate = 9600;   // INDEX => 118
    int16_t PortTowStopBits = 1;      // INDEX => 120
    int16_t PortTowDataBits = 8;      // INDEX => 122
    string PortTowParity = "None";    // INDEX => 124
    int32_t PortTowConnTimeOut = 0;   // INDEX => 126
    int8_t PortTowMaxRetries = 8;     // INDEX => 130
    bool PortTowInterface = false;    // INDEX => 131 , If true => RS485 Else If false RS232
    string PortThreeName = "";        // INDEX => 132
    int16_t PortThreeBaudRate = 9600; // INDEX => 134
    int16_t PortThreeStopBits = 1;    // INDEX => 136
    int16_t PortThreeDataBits = 8;    // INDEX => 138
    string PortThreeParity = "None";  // INDEX => 140
    int32_t PortThreeConnTimeOut = 0; // INDEX => 142
    int8_t PortThrewwMaxRetries = 8;  // INDEX => 146
    bool PortThreeInterface = false;  // INDEX => 147 , If true => RS485 Else If false RS232
    string PortFourName = "";         // INDEX => 148
    int16_t PortFourBaudRate = 9600;  // INDEX => 150
    int16_t PortFourStopBits = 1;     // INDEX => 152
    int16_t PortFourDataBits = 8;     // INDEX => 154
    string PortFourParity = "None";   // INDEX => 156
    int32_t PortFourConnTimeOut = 0;  // INDEX => 158
    int8_t PortFourMaxRetries = 8;    // INDEX => 162
    bool PortFourInterface = false;   // INDEX => 163 , If true => RS485 Else If false RS232

    // Serail Connectors
    bool SerialOneEnabled = false;
    bool SerialTowEnabled = false;
    bool SerialThreeEnabled = false;
    bool SerialFourEnabled = false;
    string SerialOneType = "";
    string SerialTowType = "";
    string SerialThreeType = "";
    string SerialFourType = "";

} PmmSerialSettings;

typedef struct PmmTCPUDPSettings
{
    /* data */
    // system memorey

    // TCP & UDP Settings
    string MacAddressEthOne = "";      // INDEX => 200
    string MacAddressEthTwo = "";      // INDEX => 204
    string IPAddressEthOne = "";       // INDEX => 208
    string RemoteIPAddressEthOne = ""; // INDEX => 212
    string SubnetMaskEthOne = "";      // INDEX => 216
    string DNSOneEthOne = "";          // INDEX => 220
    string DNSTwoEthOne = "";          // INDEX => 224
    string GatewayEthOne = "";         // INDEX => 228
    int32_t TimeOutConnEthOne = 0;     // INDEX => 232
    int16_t MaxRetriesEthOne = 0;      // INDEX => 236
    string IPAddressEthTwo = "";       // INDEX => 234
    string RemoteIPAddressEthTwo = ""; // INDEX => 238
    string SubnetMaskEthTwo = "";      // INDEX => 242
    string DNSOneEthTwo = "";          // INDEX => 246
    string DNSTowEthTwo = "";          // INDEX => 250
    string GatewayEthTwo = "";         // INDEX => 254
    int32_t TimeOutConnEthTwo = 0;     // INDEX => 258
    int16_t MaxRetriesEthTwo = 0;      // INDEX => 262
    int16_t UDPPortOne = 0;            // INDEX => 264
    int16_t UDPPortTwo = 0;            // INDEX => 266
    int16_t UDPPortThree = 0;          // INDEX => 268
    int16_t UDPPortFour = 0;           // INDEX => 270

    // new variables
    string PreferredDNSServer;
    string AlternateDNSServer;
} PmmTCPUDPSettings;

typedef struct PmmModBusSettings
{
    /* data */

    // system memorey

    // ModBus Settings
    bool ModBusRTU = false;                            // INDEX => 268 , If true => Modbus RTU     Else If false => Not Modbus RTU
    bool ModBusTCP = false;                            // INDEX => 269 , If true => Modbus TCP     Else If false => Not Modbus TCP
    bool ModBusUDP = false;                            // INDEX => 270 , If true => Modbus UDP     Else If false => Not Modbus UDP
    bool ModBusSlave = false;                          // INDEX => 271 , If true => Modbus Slave   Else If false => Not Modbus Slave
    bool ModBusMaster = false;                         // INDEX => 272 , If true => Modbus Master  Else If false => Not Modbus Master
    bool ReadCoilsStatus = false;                      // INDEX => 273 , If true => Read Coils Status  Else If false => Not Read Coils Status
    bool ReadInputStatus = false;                      // INDEX => 274 , If true => Read Input Status  Else If false => Not Read Input Status
    bool ReadHoldingRegisters = false;                 // INDEX => 275 , If true => Read Holding Registers  Else If false => Not Read Holding Registers
    bool ReadInputRegisters = false;                   // INDEX => 276 , If true => Read Input Registers  Else If false => Not Read Input Registers
    bool WriteSingleCoil = false;                      // INDEX => 277 , If true => Write Single Coil  Else If false => Not Write Single Coil
    bool WriteSingleRegister = false;                  // INDEX => 278 , If true => Write Single Register  Else If false => Not Write Single Register
    bool WriteMultipleCoils = false;                   // INDEX => 279 , If true => Write Multiple Coils  Else If false => Not Write Multiple Coils
    bool WriteMultipleRegisters = false;               // INDEX => 280 , If true => Write Multiple Registers  Else If false => Not Write Multiple Registers
    int16_t StartingAddressCoilsStatus = 0;            // INDEX => 281
    int16_t StartingAddressInputStatus = 0;            // INDEX => 283
    int16_t StartingAddressHoldingRegisters = 0;       // INDEX => 285
    int16_t StartingAddressInputRegisters = 0;         // INDEX => 287
    int16_t StartingAddressWriteSingleCoil = 0;        // INDEX => 289
    int16_t StartingAddressWriteSingleRegister = 0;    // INDEX => 291
    int16_t StartingAddressWriteMultipleCoils = 0;     // INDEX => 293
    int16_t StartingAddressWriteMultipleRegisters = 0; // INDEX => 295
    int16_t QuantityCoilsStatus = 0;                   // INDEX => 297
    int16_t QuantityInputStatus = 0;                   // INDEX => 299
    int16_t QuantityHoldingRegisters = 0;              // INDEX => 301
    int16_t QuantityInputRegisters = 0;                // INDEX => 303
    int16_t QuantityWriteMultipleCoils = 0;            // INDEX => 305
    int16_t QuantityWriteMultipleRegisters = 0;        // INDEX => 307
    int16_t FunctionCode = 01;                         // INDEX => 309 , 01 / 02 / 03 / 04 / 05 / 06 / 15 / 16
    string IODataOrder = "";                           // INDEX => 311
    string IODataType = "";                            // INDEX => 313
    int32_t PollInterval = 0;                          // INDEX => 315
    int16_t SlaveID = 1;                               // INDEX => 319
    int32_t StartingAddress = 0;                       // INDEX => 321
    int16_t Quantity = 0;                              // INDEX => 325
} PmmModBusSettings;

typedef struct PmmTimersSettings
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
} PmmTimersSettings;

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
    int16_t PortOneBaudRate = 9600;
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
    int DNSSOneerver01 = 8;
    int DNSSOneerver02 = 8;
    int DNSSOneerver03 = 8;
    int DNSSOneerver04 = 8;
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

typedef struct PMMSettingsTimer
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
} PMMSettingsTimer;

/*
typedef struct PmmHardwareOption
{
    bit HasEthernet = false;
    bit HasSwitch = false;
    bit HasFiber = false;
    bit HasExternalRTC = false;
    bit HasInternalRTC = false;
    bit HasEEprom = false;
    bit HasInternalFlash = false;
    bit HasExternalFlash = false;
    bit Com1Enable = false;
    bit Com2Enable = false;
    bit Com3Enable = false;
    bit Com4Enable = false;
    int Com1Interface = 485;
    int Com2Interface = 485;
    int Com3Interface = 485;
    int Com4Interface = 485;

} PmmHardwareOption;

typedef struct PmmI2CExtensions
{
    // Extension Boards
    int I2CType = 0;
    int I2CSlaveAddress = 1;
    int BoardOne = 1;
    int BoardTwo = 1;
    int BoardThree = 1;
    int BoardFour = 1;
    int BoardOneAddress1 = 1;
    int BoardOneAddress2 = 1;
    int BoardTowAddress1 = 1;
    int BoardTowAddress2 = 1;
    int BoardThreeAddress1 = 1;
    int BoardThreeAddress2 = 1;
    int BoardFourAddress1 = 1;
    int BoardFourAddress2 = 1;

} PmmI2CExtensions;

typedef struct PmmIOPins
{


    // system memorey
    int Pin01 = 1;
    int Pin02 = 1;
    int Pin03 = 1;
    int Pin04 = 1;
    int Pin05 = 1;
    int Pin06 = 1;
    int Pin07 = 1;
    int Pin08 = 1;
    int Pin09 = 1;
    int Pin10 = 1;
    int Pin11 = 1;
    int Pin12 = 1;
    int Pin13 = 1;
    int Pin14 = 1;
    int Pin15 = 1;
    int Pin16 = 1;
    int Pin17 = 1;
    int Pin18 = 1;
    int Pin19 = 1;
    int Pin20 = 1;
    int Pin21 = 1;
    int Pin22 = 1;
    int Pin23 = 1;
    int Pin24 = 1;
} PmmIOPins;
*/

typedef struct Product
{

    PMMGeneral PmmGeneral;
    PMMRTU PmmRTU;
    PMMTCPUDP PmmTCPUDP;
    PMMModBus PmmModbus;
    PMMSettingsTimer PmmTimers;

} Product;
