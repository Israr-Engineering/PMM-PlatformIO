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

    // system memorey
    // Device Information
    int DeviceName = 0620;              //(00)
    u_int32_t SerialNumber = 5000;      //(01) (02)
    u_int32_t FirstTimeRun = 0;         // (03) (04)
    u_int32_t LifeTime = 0;             //(05) (06)
    u_int32_t NumberOfCycles = 0;       // (07) (08)
    u_int32_t LastRunningTime = 0;      //(09) (10)
    u_int32_t NumberOfRunningTimes = 0; // (11) (12)
    int SoftwareVersion = 1;            //(13) // 12 => 1.2
    int FirmwareVersion = 1;            //(14) // 12 => 1.2
    int HardwareVersion = 1;            //(15) // 23 => 2.3
    int ControlerType = 0;              //(16)

    // int Options01 ; //(17)
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

    // int Options02 ; //(18)

    bit Canprint = false;     // bit for bebug mode
    bit Ext01Pac9535 = true; // true if extesion board support Pac9535 chip
    bit Ext02Pac9535 = false;
    bit Ext03Pac9535 = false;
    bit I2CServer = false;
    bit spare125 = false;
    bit spare126 = false;
    bit spare127 = false;

    bit spare128 = false;
    bit spare129 = false;
    bit spare130 = false;
    bit spare131 = false;
    bit spare132 = false;
    bit spare133 = false;
    bit spare134 = false;
    bit spare135 = false;

    int Ext01Name = 625; //(19) 00 => zero = No extesion board
    // int Ext01Address = 0;//(20)
    uint8_t Ext01Address01 = 32;// 0x20
    uint8_t Ext01Address02 = 51;// 0x21
    int Ext02Name = 0; //(21)
    // int Ext01Address = 0;//(22)
    uint8_t Ext02Address01 = 0;
    uint8_t Ext02Address02 = 0;
    int Ext03Name = 0; //(23)
    // int Ext01Address = 0;//(24)
    uint8_t Ext03Address01 = 0;
    uint8_t Ext03Address02 = 0;
    int I2CServerAddress = 0; //(25)

    int Spare26 = 0; //(26)
    int Spare27 = 0; //(27)
    int Spare28 = 0; //(29)
    int Spare29 = 0; //(30)
    int Spare30 = 0; //(31)

    int SettingsRef = 1; //(28) // 0 =>Internal Flash,1=>EEPROM,2=>External Flash

} PMMGeneral;

typedef struct PMMProtocol
{
    /* data */
    // system memorey

    // int Spare00 ;
    bit IsModBus = true;
    bit IsCanBus = false;
    bit IsProfiBus = false;
    bit IsProfiNet = false;
    bit IsBACnet = false;
    bit IsDLMS = false;
    bit IsMBus = false;
    bit IsOPC = false;

    bit Spare08 = false;
    bit Spare09 = false;
    bit Spare10 = false;
    bit Spare11 = false;
    bit Spare12 = false;
    bit Spare13 = false;
    bit Spare14 = false;
    bit IsRunning = false; // status only, No write just read

    // ModBus Settings
    bit ModBusRTU = true;
    bit ModBusTCP = false;
    bit ModBusSlave = true;
    bit ModBusMaster = false;
    bit ReadCoilsStatus = true;
    bit ReadInputStatus = true;
    bit ReadHoldingRegisters = true;
    bit ReadInputRegisters = true;

    bit WriteSingleCoil = false;
    bit WriteSingleRegister = false;
    bit WriteMultipleCoils = false;
    bit WriteMultipleRegisters = false;
    bit CoilsStatus = true;
    bit InputStatus = true;
    bit HoldingRegisters = true;
    bit InputRegisters = true;

    int StartingAddressCoilsStatus = 0; // (02)
    int StartingAddressInputStatus = 0; // (03)

    int StartingAddressHoldingRegisters = 0; //(04)
    int StartingAddressInputRegisters = 0;   //(05)
    int QuantityCoilsStatus = 128;             // (06)
    int QuantityInputStatus = 128;             // (07)

    int QuantityHoldingRegisters = 128; // (08)
    int QuantityInputRegisters = 128;   // (09)
    int FunctionCode = 01;            // (10)
    int IODataOrder = 1;              // (11)

    int IODataType = 1;           // (12)
    int PollInterval = 1000;         // (13)
    int SlaveID = 1;              // (14)
    int ConnectionTimeout = 5000; //(15)

    int MaxRetry = 5; //(16)
    int Spare17 = 0;  //(17)
    int Spare18 = 0;  //(18)
    int Spare19 = 0;  //(19)

    int Spare20 = 0; //(20)
    int Spare21 = 0; //(21)
    int Spare22 = 0; //(22)
    int Spare23 = 0; //(23)

    int Spare24 = 0; //(24)
    int Spare25 = 0; //(25)
    int Spare26 = 0; //(26)
    int Spare27 = 0; //(27)

    int Spare28 = 0; //(28)
    int Spare29 = 0; //(29)
    int Spare30 = 0; //(30)
    int Spare31 = 0; //(31)

} PMMProtocol;

typedef struct PMMTimer
{
    // system memorey

    // Timers Settings ALL TIMERS BITS ARE READONLY
    // int Spare00 ; //(00)
    bit Cycle = false;     // Readonly
    bit FirstScan = false; // Readonly
    bit OneMS = false;     // Readonly
    bit TenMS = false;     // Readonly

    bit M30ms = false;  // Readonly
    bit OneSec = false; // Readonly
    bit M30Sec = false; // Readonly
    bit OneMin = false; // Readonly

    bit M30Min = false;    // Readonly
    bit OneHour = false;   // Readonly
    bit OneDay = false;    // Readonly
    bit OneMonnth = false; // Readonly

    bit OneYear = false;   // Readonly
    bit Spare0012 = false; // Readonly
    bit Spare0013 = false; // Readonly
    bit Spare0014 = false; // Readonly

    // Main loop timer ,default = 1 second
    u_int32_t ScanTimer = 1000; //(01) (02)
    // Configration loop timer,default = 0.5 sec
    u_int32_t ConfigTimer = 500; //(03) (04)
    // Communication Update timer ,default = 0.5 sec
    u_int32_t CommUpdateTimer = 500; //(05) (06)

    int Spare[24] = {0};

} PMMTimer;

typedef struct PMMDeviceCalibration
{
    /*
    this is a special settings for the device itself
    */
    float Header = 0 ; // Headert to make sure EEprom is working , not for read
    float Calebrate[15] = {0};

} PMMDeviceCalibration;

typedef struct GerneralPurpose
{
    /*
    this is a special settings for the device itself
    */
    int Header = 0 ; // Headert to make sure EEprom is working , not for read
    int Spare[31] = {0};

} GerneralPurpose;

typedef struct PMMSERIAL
{
    u_int8_t PortStopBit = 10; // 0
    u_int8_t PortDataBit = 8; // 1
    u_int8_t PortParity = 3;  //  2 
    u_int8_t PortName = 1;    // 3
    //SERIAL_8N1

    long SerialConfig = 1043; //(4)(5)  : 23 u_int16_t config = (databitLong | parityLong | stopbitLong);Arduino Stanard

    long BaudRate = 19200; // (6)(7)

    int TXPin = 35;             // (8)
    int RXPin = 36;             // (9)
    int SerialSelectionPin = 31; // (10)
    int SerialPort = 1;         // (11)
    int Interface = 485;        // (12) (232,422,485,)

    byte MACAddress01 = 0x22; // (13) 0x7E,0x48,0x22,0x22,0x22,(ID)
    byte MACAddress02 = 0x22; // (14)
    byte MACAddress03 = 0x22; // (15)
    byte MACAddress04 = 0x22; // (16)
    // byte MACAddress05 = 0x48 ; // fixed
    // byte MACAddress06 = 0x7E ; // fixed

    uint8_t IpAddress01 = 192; // (17)
    uint8_t IpAddress02 = 168; // (18)
    uint8_t IpAddress03 = 1;   // (19)
    uint8_t IpAddress04 = 110; // (20)

    uint8_t SubnetMask01 = 255; // (21)
    uint8_t SubnetMask02 = 255; // (22)
    uint8_t SubnetMask03 = 255; // (23)
    uint8_t SubnetMask04 = 0; // (24)

    uint8_t GateWay01 = 192; // (15)
    uint8_t GateWay02 = 168; // (26)
    uint8_t GateWay03 = 1;   // (27)
    uint8_t GateWay04 = 1;   // (28)

    byte DNS101 = 8; // (29)
    byte DNS102 = 8; // (30)
    byte DNS103 = 8; // (31)
    byte DNS104 = 8; // (32)

    byte DNS201 = 8; // (33)
    byte DNS202 = 8; // (34)
    byte DNS203 = 0; // (35)
    byte DNS204 = 0; // (36)

    // u_int32_t Port = 0; //
    uint16_t Port01 = 502; //(37)(38)
    uint16_t Port02 = 503; //(39)
    uint16_t Port03 = 504; //(40)
    uint16_t Port04 = 505; //(41)

    int Spare28 = 0; //(42)
    int Spare29 = 0; //(43)
    int Spare30 = 0; //(44)

    //(45)
    bit Enabled = false;
    bit spare3101 = false;
    bit spare3102 = false;
    bit spare3103 = false;
    bit spare3104 = false;
    bit spare3105 = false;
    bit spare3106 = false;
    bit spare3107 = false;

    //(46)
    bit spare3108 = false;
    bit spare3109 = false;
    bit spare3110 = false;
    bit spare3111 = false;
    bit spare3112 = false;
    bit spare3113 = false;
    bit IsEthernet = false; // if not means normal Serial
    bit IsRunning = false;  // Read only

    // modbus
    PMMProtocol PmmProtocols;
} PMMSERIAL;

typedef struct Product
{
    // General Flages
    bool EthernetRunning = false;
    bool SerialRunning = false;
    bool WireRunning = false;
    bool I2CRunning = false;
    bool SPIRunning = false;
    bool ModbusRTUServerRunning = false;
    bool ModbusTCPServerRunning = false;
    bool ModbusRTUClientRunning = false;
    bool ModbusTCPClientRunning = false;
    bool InternalRTCRunning = false;
    bool ExternalRTCRunning = false;
    bool FirstTimeStart = true ;
    
    bool ExternalEEpromRunning = false;

    // Settings Stucts
    PMMGeneral PmmGeneral;
    PMMSERIAL PmmSerial[PMMNUMOFSERIALINTERFACES]; // PmmSerial[0] is the ethernet port
    #ifdef PMMTIMERS
    PMMTimer PmmTimers;
    #endif
    #ifdef PMMCALIBRATION
    PMMDeviceCalibration PmmCalibrationPage[4];
    #endif
    #ifdef PMMGENERALPURPOSE
    GerneralPurpose PmmGerneralPurpose;
    #endif

} Product;

// typedef struct PMMIO
// {
//     // // represent descret InputStatus in modbus
//     //int InputsByte[120];
//     // // image for internal use
//     //int InputsBytePrev[120];
//     // // represent coils bits in modbus
//     // bit OutputsByte[1920];
//     // // image for internal use
//     // bit OutputsBytePrev[1920];

//     // represent Input registers in modbus
//     int Inputs[128];
//     // image for internal use
//     int InputsPrev[128];
//     // represent hodling registers in modbus
//     int Outputs[128];
//     // image for internal use
//     int OutputsPrev[128];

// } PMMIO;




/*********************/
/*  OpenPLC Variables and defs   */
/*  IEC Types defs   */
/*********************/

typedef uint8_t  IEC_BOOL;

typedef int8_t    IEC_SINT;
typedef int16_t   IEC_INT;
typedef int32_t   IEC_DINT;
typedef int64_t   IEC_LINT;

typedef uint8_t    IEC_USINT;
typedef uint16_t   IEC_UINT;
typedef uint32_t   IEC_UDINT;
typedef uint64_t   IEC_ULINT;

typedef uint8_t    IEC_BYTE;
typedef uint16_t   IEC_WORD;
typedef uint32_t   IEC_DWORD;
typedef uint64_t   IEC_LWORD;

typedef float    IEC_REAL;
typedef double   IEC_LREAL;


#define MAX_DIGITAL_INPUT          16
#define MAX_DIGITAL_OUTPUT         16
#define MAX_ANALOG_INPUT           6
#define MAX_ANALOG_OUTPUT          6


//OpenPLC Buffers

 IEC_BOOL bool_input[MAX_DIGITAL_INPUT/8][8] = {0};
 IEC_BOOL bool_output[MAX_DIGITAL_OUTPUT/8][8] = {0};
 IEC_UINT int_input[MAX_ANALOG_INPUT] = {0};
 IEC_UINT int_output[MAX_ANALOG_OUTPUT] = {0};


#endif
