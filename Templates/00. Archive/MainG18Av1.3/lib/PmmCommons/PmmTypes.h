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
    int DeviceName = 0620; //(00)
    long SerialNumber = 5000; //(01) (02)
    long FirstTimeRun = 0;// (03) (04)
    long LifeTime = 0;//(05) (06)
    long NumberOfCycles = 0;// (07) (08)
    long LastRunningTime = 0;//(09) (10)
    long NumberOfRunningTimes = 0;// (11) (12)
    int SoftwareVersion = 1;//(13) // 12 => 1.2
    int FirmwareVersion = 1;//(14) // 12 => 1.2
    int HardwareVersion = 1;//(15) // 23 => 2.3
    int ControlerType = 0;//(16)

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
    
    bit Canprint = false; // bit for bebug mode
    bit spare121 = false;
    bit spare122 = false;
    bit spare123 = false;
    bit spare124 = false;
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

    int NumberOfInputs = 0;//(19)
    int NumberOfOutputs = 0;//(20)
    int NumberOfSerials = 0;//(21)
    int NumberOfUDPPorts = 0;//(22)
    int MinReadValue = 0;//(23)
    int MaxReadValue = 0;//(24)
    int OprationVoltage = 0;//(25)
    int GeneralReadingsOffset = 0;//(26)
    int GeneralReadingsFactor = 0;//(27)

    int SettingsRef = 1;//(28) // 1 =>Internal Flash,2=>External Flash,3=>EEPROM

    int Spare29 = 0;//(29)
    int Spare30 = 0;//(30)
    int Spare31 = 0;//(31)

} PMMGeneral;

typedef struct PMMProtocol
{
    /* data */
    // system memorey

    // int Spare00 ;
    bit IsModBus = false;
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
    bit IsRunning = false; //status only, No write just read

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

    int StartingAddressCoilsStatus = 0; // (02)
    int StartingAddressInputStatus = 0; // (03)

    int StartingAddressHoldingRegisters = 0; //(04)
    int StartingAddressInputRegisters = 0; //(05)
    int QuantityCoilsStatus = 0; // (06)
    int QuantityInputStatus = 0; // (07)

    int QuantityHoldingRegisters = 0; // (08)
    int QuantityInputRegisters = 0; // (09)
    int FunctionCode = 01;// (10)
    int IODataOrder = 1; // (11)

    int IODataType = 1; // (12)
    int PollInterval = 0; // (13)
    int SlaveID = 1; // (14)
    int ConnectionTimeout = 5000; //(15)

    int MaxRetry = 5; //(16)
    int Spare17 = 0; //(17)
    int Spare18 = 0; //(18)
    int Spare19 = 0; //(19)

    int Spare20 = 0; //(20)
    int Spare21 = 0; //(21)
    int Spare22 = 0; //(22)
    int Spare23 = 0; //(23)

    int Spare24 = 0;//(24)
    int Spare25 = 0;//(25)
    int Spare26 = 0;//(26)
    int Spare27 = 0;//(27)

    int Spare28 = 0;//(28)
    int Spare29 = 0;//(29)
    int Spare30 = 0;//(30)
    int Spare31 = 0;//(31)
    
} PMMProtocol;

typedef struct PMMTimer
{
    /* data */

    // system memorey

    // Timers Settings ALL TIMERS BITS ARE READONLY
    // int Spare00 ; //(00)
    bit CycleTimer = false;   // INDEX => 500 ,Flip Flop Timer Every Cycle
    bit OneMSTimer = false;   // INDEX => 500.125 ,Flip Flop Timer Every One MiliSeconds
    bit TenMSTimer = false;   // INDEX => 500.250 ,Flip Flop Timer Every Ten MiliSeconds
    bit OneSecTimer = false;  // INDEX => 500.375 ,Flip Flop Timer Every One Second
    bit OneMinTimer = false;  // INDEX => 500.500 ,Flip Flop Timer Every One Minute
    bit OneHouTimer = false;  // INDEX => 500.625 ,Flip Flop Timer Every One Hour
    bit OneMonTimer = false;  // INDEX => 500.750 ,Flip Flop Timer Every One Month
    bit OneYearTimer = false; // INDEX => 500.875 ,Flip Flop Timer Every One Year

    bit Spare0008 = false;
    bit Spare0009 = false;
    bit Spare0010 = false;
    bit Spare0011 = false;
    bit Spare0012 = false;
    bit Spare0013 = false;
    bit Spare0014 = false;
    bit Spare0015 = false; 

    //Main loop timer ,default = 1 second
    long ScanTimer = 1000; //(01) (02) 
    // Configration loop timer,default = 0.5 sec 
    long ConfigTimer = 500; //(03) (04)
    //Communication Update timer ,default = 0.5 sec
    long CommUpdateTimer = 500;//(05) (06)
    
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
    // long SerialConfigDetails ; (0) (1)
    u_int8_t PortStopBit = 1; // 0
    u_int8_t PortDataBit = 8; // 1
    u_int8_t PortParity = 0; //  2 : 0 => Non , 1 => Odd , 2 => Even
    u_int8_t PortName = 1;

    long SerialConfig = 8; //(2)(3)  : 23 u_int16_t config = (databitLong | parityLong | stopbitLong);Arduino Stanard

    long BaudRate = 9600; // (4)(5)

    int TXPin = 35; // (6)
    int RXPin = 36;// (7)
    int SerialSelectionPin = 1; // (8)
    int SerialPort = 1; // (9)
    int Interface = 485; // (10) (232,422,485,)

    //u_int32_t MACAddress = 0; // (11)(12)
    byte MACAddress01 = 192 ;
    byte MACAddress02 = 192 ;
    byte MACAddress03 = 192 ;
    byte MACAddress04 = 192 ;
    //byte MACAddress05 = 192 ; // fixed
    //byte MACAddress06 = 192 ; // fixed

    //u_int32_t IpAddress = 0 ; // (13) (14)
    byte IpAddress01 = 192 ;
    byte IpAddress02 = 168 ;
    byte IpAddress03 = 0 ;
    byte IpAddress04 = 200 ;
    
    //u_int32_t SubnetMask = 0 ; // (15) (16)
    byte SubnetMask01 = 255 ;
    byte SubnetMask02 = 255 ;
    byte SubnetMask03 = 255 ;
    byte SubnetMask04 = 255 ;

    //u_int32_t GateWay = 0; // (17) (18)
    byte GateWay01 = 192 ;
    byte GateWay02 = 168 ;
    byte GateWay03 = 0 ;
    byte GateWay04 = 1 ;

    //u_int32_t DNS1 = 0; // (19) (20)
    byte DNS101 = 8 ;
    byte DNS102 = 8 ;
    byte DNS103 = 8 ;
    byte DNS104 = 8 ;

    u_int32_t DNS2 = 0; // (21) (22)
    byte DNS201 = 8 ;
    byte DNS202 = 8 ;
    byte DNS203 = 0 ;
    byte DNS204 = 0 ;

    //u_int32_t Port = 0; //  
    uint16_t Port01 = 502 ;//(23)
    uint16_t Port02 = 503 ;//(24)
    uint16_t Port03 = 504 ;//(25)
    uint16_t Port04 = 505 ;//(26)

    
    int Spare27 = 0;
    int Spare28 = 0;
    int Spare29 = 0;
    int Spare30 = 0;

    //int Spare31 = 0;
    bit Enabled = false;
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
    bit IsEthernet = false; // if not means normal Serial
    bit IsRunning = false; //Read only
    
    // modbus
    PMMProtocol PmmProtocols;
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
    PMMSERIAL PmmSerial[4]; // PmmSerial[0] is the ethernet port
    PMMTimer PmmTimers;
    PMMDeviceCalibration PmmCalibrationPage[2];
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
