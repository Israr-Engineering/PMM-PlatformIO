
#ifndef PMM0503_H
#define PMM0503_H

#define PACKET_MAX_SIZE 100

#include "Arduino.h"
#include <PmmTypes.h>
// #include <PmmEthernetUdp.h>
#include <PmmEthernet.h>

#include <PmmGlobalFunctions.h>

// Common Pins If any (Pin Mask)
static const uint8_t DOSSWIZ = 9ul;            // PA13
static const uint8_t DOETHERRST = 32ul;        // PA14
static const uint8_t DOETHERINT = 18ul;        // PA15
static const uint8_t DOTERMRESISTOR = 18ul;    // PB02
static const uint8_t DORS485SELECT = 18ul;     // PB03
static const uint8_t DOHALFDUBLIX = 18ul;      // PA02
static const uint8_t DOETHERNETCONTROL = 18ul; // PA03

static const uint8_t DILOSSPOWER = 19ul; // PA28

// Etehrnet

byte mac[] = {0x7E, 0x48, 0x22, 0x22, 0x22, 0x7E}; // 0x7E,0x48,0x22,0x22,0x22,(ID)
byte IPAdd[] = {192, 168, 1, 100};                 // 192, 168, 1, (ID)
long BaudRate01 = 19200;
long BaudRate02 = 9600;
u_int16_t PinTx = 35;
u_int16_t PinRx = 36;
u_int16_t PinTxEna = 31;
u_long PinWizS = 9;
u_long SettingSerial01 = 1043; // SERIAL_8N1

PmmEthernetUDP PmmUDP;
PmmEthernetServer cmdServer(23);

// functions
void checkUDP();

//Modes
bool PEER2PEERMODE = false;
bool SERIALSERVERMODE = false;

// General Variables
char packetBuffer[PACKET_MAX_SIZE]; // // To Read from UDP
byte rcvBuffer[PACKET_MAX_SIZE];    // // To Write To UDP
bool RemoteIPSetup = false;
bool alreadyConnected = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This product main functions part                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ResetEtherNetController()
{
    // Reset Ethernet controller
    pinMode(DOETHERRST, OUTPUT);
    digitalWrite(DOETHERRST, LOW);
    delay(10);
    digitalWrite(DOETHERRST, HIGH);
}

void hardwareInit()
{
    // STEP01 : Prepare IOs

    // other pins setup
    pinMode(DILOSSPOWER, INPUT_PULLDOWN);
    pinMode(DOSSWIZ, OUTPUT);           // PA13
    pinMode(DOETHERRST, OUTPUT);        // PA14
    pinMode(DOETHERINT, OUTPUT);        // PA15
    pinMode(DOTERMRESISTOR, OUTPUT);    // PB02
    pinMode(DORS485SELECT, OUTPUT);     // PB03
    pinMode(DOHALFDUBLIX, OUTPUT);      // PA02
    pinMode(DOETHERNETCONTROL, OUTPUT); // PA03

    // STEP02 : Get device address and load variables and check factory Defaults

    // STEP03 : check and update Modes

    // STEP04 : Setup Ethernet Controller
    ResetEtherNetController();

    // if (FactoryDefualtMode)
    // {
    //     tmpID = Pmm1003ID;
    //     if (MasterIOMode)
    //         tmpID++;
    // }
    // else
    // {
    // Read from internal EEprom
    // STEP01 : Read Settings
    PmmReadSerialSettings(0, 0);
    // STEP02 : Load Variables
    BaudRate01 = ThisProduct.PmmSerial[0].BaudRate;
    PinTx = ThisProduct.PmmSerial[0].TXPin;
    PinRx = ThisProduct.PmmSerial[0].RXPin;
    PinTxEna = ThisProduct.PmmSerial[0].SerialSelectionPin;
    SettingSerial01 = PmmReturnConfig(
        ThisProduct.PmmSerial[0].PortDataBit,
        ThisProduct.PmmSerial[0].PortParity,
        ThisProduct.PmmSerial[0].PortStopBit);

    IPAdd[0] = ThisProduct.PmmSerial[0].IpAddress01;
    IPAdd[1] = ThisProduct.PmmSerial[0].IpAddress02;
    IPAdd[2] = ThisProduct.PmmSerial[0].IpAddress03;
    IPAdd[3] = ThisProduct.PmmSerial[0].IpAddress04;

    // mac[0]  ==> fixed (0x7E)
    // mac[1]  ==> fixed (0x48)
    mac[2] = ThisProduct.PmmSerial[0].MACAddress01;
    mac[3] = ThisProduct.PmmSerial[0].MACAddress02;
    mac[4] = ThisProduct.PmmSerial[0].MACAddress03;
    mac[5] = ThisProduct.PmmSerial[0].MACAddress04;
    //}
    // Now Start Ethernet controller

    IPAddress ip(IPAdd[0], IPAdd[1], IPAdd[2], IPAdd[3]);
    Ethernet.init(DOSSWIZ); // for WIZ5100 sspin
    Ethernet.begin(mac, ip);
    Serial.begin(BaudRate01, SettingSerial01);
    PmmUDP.begin(ThisProduct.PmmSerial[0].Port01);
    // server.begin(); // port 80

    // STEP05 : UpDate modes
    if (ThisProduct.PmmSerial[0].PeerToPeer)
    {
        PEER2PEERMODE = true;
        SERIALSERVERMODE = false;
    }
    else if (ThisProduct.PmmSerial[0].SerialServer)
    {
        PEER2PEERMODE = false;
        SERIALSERVERMODE = true;
    }

    // STEP06 : Select Serial interface type
    if (ThisProduct.PmmSerial[0].Interface == 422)
    {
        digitalWrite(DORS485SELECT,LOW);
        digitalWrite(DOHALFDUBLIX,LOW);

    }
    else if (ThisProduct.PmmSerial[0].Interface == 232)
    {
       digitalWrite(DORS485SELECT,LOW);
        digitalWrite(DOHALFDUBLIX,HIGH);
    }
    else //RS485
    {
        digitalWrite(DORS485SELECT,HIGH);
        digitalWrite(DOHALFDUBLIX,HIGH);
    }
    

    // STEP7: Initialize WatchDog timer
    // ignore if in debug mode
    // DIDEBUGMODE_Status = digitalRead(DIDEBUGMODE);
    // if (!DIDEBUGMODE_Status)
    // {
    //     if (MasterIOMode & FactoryDefualtMode & ModbusTCPMode)
    //         PmmWatchDoggy.setup(WDT_HARDCYCLE2S);
    //     else if (MasterIOMode & FactoryDefualtMode & !ModbusTCPMode)
    //         PmmWatchDoggy.setup(WDT_HARDCYCLE2S);
    //     else if (MasterIOMode & !FactoryDefualtMode)
    //         PmmWatchDoggy.setup(WDT_HARDCYCLE4S);
    //     else
    //         PmmWatchDoggy.setup(WDT_HARDCYCLE8S);
    // }
}

void UpDateLEDs()
{
}



void updateInputBuffers()
{
    if (PEER2PEERMODE)
    {
        // STEP01 : Generate RemotIP one time
        // if (!RemoteIPSetup)
        // {
            IPAddress RemotIP(IPAdd[0], IPAdd[1], IPAdd[2], ThisProduct.PmmSerial[0].RemoteIP);
            RemoteIPSetup = true;
        // }       
        // STEP02:  From Fiber to Serial
        int packetSize = PmmUDP.parsePacket(); // Size Of Fiber Packet
        if (packetSize)
        {
            PmmUDP.read(packetBuffer, PACKET_MAX_SIZE);
            PmmUDP.clearWriteError();
            PmmUDP.flush();
            Serial.write(packetBuffer, packetSize); // Write in Serial
            // SerialUSB.write(packetBuffer, packetSize);    // For debuging
            memset(packetBuffer, 0, sizeof packetBuffer); // Empty packetBuffer
        }

        // STEP03 From Serial to fiber
        if (Serial.available() > 0)
        {
            Serial.setTimeout(50);
            int msgLength = Serial.readBytes(rcvBuffer, 64);
            SerialUSB.write(rcvBuffer, msgLength);
            PmmUDP.beginPacket(RemotIP, ThisProduct.PmmSerial[0].Port01 /*DevicePort*/);
            PmmUDP.write(rcvBuffer, msgLength);
            PmmUDP.endPacket();
            PmmUDP.clearWriteError();
            PmmUDP.flush();
        }
    }
    else if (SERIALSERVERMODE)
    {
        //STEP01: wait for a new client:
        PmmEthernetClient client = cmdServer.available();

        // checkUDP();

        char c;

        //STEP02: transfer all bytes from client to Serial
        if (client)
        {
            if (!alreadyConnected)
            {
                // clean out the input buffer:
                client.flush();
                alreadyConnected = true;
            }

            if (client.available() > 0)
            {
                c = client.read();
                Serial.write(c);
            }
        }

        //STEP03: transfer all bytes to the cmdServer
        // (all connected clients will recieve it)
        if (Serial.available() > 0)
        {
            c = Serial.read();
            cmdServer.write(c);
        }
    }
}

void updateOutputBuffers()
{

    // UpDateLEDs();
}

void GenerateAlarm()
{
    //  Set AlarmFlag if ....
}

void syncModbusBuffers()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This product main functions                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// buffers for receiving and sending data
// char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; // buffer to hold incoming packet,
char ReplyBuffer[] = "Serial gate v1.1"; // a string to send back

void checkUDP()
{
    // if there's data available, read a packet
    int packetSize = PmmUDP.parsePacket();
    if (packetSize)
    {

        // read the packet into packetBufffer
        PmmUDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        packetBuffer[packetSize] = '\0';

        if (strcmp(packetBuffer, "?") == 0)
        {
            // send a reply, to the IP address and port that sent us the packet we received
            PmmUDP.beginPacket(PmmUDP.remoteIP(), PmmUDP.remotePort());
            PmmUDP.write(ReplyBuffer);
            PmmUDP.endPacket();
        }
    }
}

#endif
