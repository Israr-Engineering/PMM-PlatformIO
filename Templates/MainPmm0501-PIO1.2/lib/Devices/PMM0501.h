
#ifndef PMM0501_H
#define PMM0501_H

////// NOTE : This only for this Board Rx,Tx Reversed///////
// #define PIN_SERIAL_RX       (35ul)
// #define PIN_SERIAL_TX       (36ul)
////////////////////////////////////////////////////////////

#define PACKET_MAX_SIZE 100

#include "Arduino.h"
#include <PmmTypes.h>
// #include <PmmEthernetUdp.h>
#include <PmmEthernet.h>

#include <PmmGlobalFunctions.h>

// Common Pins If any (Pin Mask)
static const uint8_t DOSSWIZ = 42ul;    // PA17
static const uint8_t DOETHERRST = 44ul; // PA06
static const uint8_t DOETHERINT = 25ul; // PA04

static const uint8_t DOS1 = 12ul; // PB02
static const uint8_t DOS2 = 13ul; // PB03
static const uint8_t DOS3 = 47ul; // PA02
static const uint8_t DOS4 = 46ul; // PA03
static const uint8_t DOS5 = 23ul; // PB08
static const uint8_t DOS6 = 24ul; // PB09

static const uint8_t DIMODE01 = 43ul; // PA21
static const uint8_t DIMODE02 = 10ul; // PA20
static const uint8_t DIMODE03 = 39ul; // PA19

static const uint8_t DILOSSPOWER = 32ul; // PA28

// Etehrnet

byte mac[] = {0x7E, 0x48, 0x22, 0x22, 0x22, 0x7E}; // 0x7E,0x48,0x22,0x22,0x22,(ID)
byte IPAdd[] = {192, 168, 1, 100};                 // 192, 168, 1, (ID)
long BaudRate01 = 19200;
long BaudRate02 = 9600;
u_int16_t PinTx = 35;
u_int16_t PinRx = 36;
u_int16_t PinTxEna = 31;
u_long PinWizS = 9;
long SettingSerial01 = 1043; // SERIAL_8N1

PmmEthernetUDP PmmUDP[4];

Stream *serials[4];
u_int16_t PortsArray[4];

#ifdef PMMHTTPSERVER
PmmEthernetServer CmdServer(80); // this for webserver
#endif
// functions
void checkUDP();
void UpDateLEDs();

// Modes
bool PEER2PEERMODE = false;
bool SERIALSERVERMODE = false;
bool MODBUSGATEWAYMODE = false;

// General Variables
char packetBuffer[PACKET_MAX_SIZE]; // // To Read from UDP
byte rcvBuffer[PACKET_MAX_SIZE];    // // To Write To UDP
bool RemoteIPSetup = false;
bool alreadyConnected = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  This product main functions part                                           //
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
    pinMode(DOSSWIZ, OUTPUT);    // PA13
    pinMode(DOETHERRST, OUTPUT); // PA14
    pinMode(DOETHERINT, OUTPUT); // PA15

    pinMode(DOS1, OUTPUT); // PB02
    pinMode(DOS2, OUTPUT); // PB03
    pinMode(DOS3, OUTPUT); // PA02
    pinMode(DOS4, OUTPUT); // PA03
    pinMode(DOS5, OUTPUT); // PB08
    pinMode(DOS6, OUTPUT); // PB09

    pinMode(DIMODE01, INPUT_PULLDOWN); // PA21
    pinMode(DIMODE02, INPUT_PULLDOWN);
    pinMode(DIMODE03, INPUT_PULLDOWN);

    // STEP02 : Get device address and load variables and check factory Defaults

    // STEP03 : check and update Modes

    // STEP04 : Setup Ethernet Controller
    ResetEtherNetController();

    // if (FactoryDefualtMode)
    // {
    // }
    // else
    // {
    // Read from internal EEprom
    // STEP01 : Read Settings
    // PmmReadSerialSettings(0, 0);
    // STEP02 : Load Variables
    // for (int i = 0 ; i < 4;i++)
    // {
    PmmReadSerialSettings(0, 0);
    PmmReadSerialSettings(1, 0);
    PmmReadSerialSettings(2, 0);
    PmmReadSerialSettings(3, 0);

    // BaudRate01 = ThisProduct.PmmSerial[0].BaudRate;
    // PinTx = ThisProduct.PmmSerial[0].TXPin;
    // PinRx = ThisProduct.PmmSerial[0].RXPin;
    // PinTxEna = ThisProduct.PmmSerial[0].SerialSelectionPin;
    // SettingSerial01 = PmmReturnConfig(
    //     ThisProduct.PmmSerial[0].PortDataBit,
    //     ThisProduct.PmmSerial[0].PortParity,
    //     ThisProduct.PmmSerial[0].PortStopBit);

    // SettingSerial01 = ThisProduct.PmmSerial[0].SerialConfig;

    // }

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

#ifdef PMMHTTPSERVER
    CmdServer.begin(); // webserver
#endif

    if (ThisProduct.PmmSerial[0].PeerToPeer || ThisProduct.PmmSerial[0].SerialServer)
    {
        Serial.begin(ThisProduct.PmmSerial[0].BaudRate, ThisProduct.PmmSerial[0].SerialConfig);
        Serial1.begin(ThisProduct.PmmSerial[1].BaudRate, ThisProduct.PmmSerial[1].SerialConfig);
        Serial2.begin(ThisProduct.PmmSerial[2].BaudRate, ThisProduct.PmmSerial[2].SerialConfig);
        Serial3.begin(ThisProduct.PmmSerial[3].BaudRate, ThisProduct.PmmSerial[3].SerialConfig);
    }

    // convert Serial ports to stream of array for indexing
    serials[0] = &Serial;
    serials[1] = &Serial1;
    serials[2] = &Serial2;
    serials[3] = &Serial3;

    // convert  ports to stream of array for indexing
    PortsArray[0] = ThisProduct.PmmSerial[0].Port01;
    PortsArray[0] = ThisProduct.PmmSerial[0].Port02;
    PortsArray[0] = ThisProduct.PmmSerial[0].Port03;
    PortsArray[0] = ThisProduct.PmmSerial[0].Port04;

    // ThisProduct.PmmSerial[0].Port01 = 91;

    PmmUDP[0].begin(ThisProduct.PmmSerial[0].Port01);
    PmmUDP[1].begin(ThisProduct.PmmSerial[0].Port02);
    PmmUDP[2].begin(ThisProduct.PmmSerial[0].Port03);
    PmmUDP[3].begin(ThisProduct.PmmSerial[0].Port04);

    // server.begin(); // port 80

    // ThisProduct.PmmSerial[0].PeerToPeer = true ; //for test only
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
    // Serial1
    //  STEP06 : Select Serial interface type
    if (ThisProduct.PmmSerial[0].Interface == 422)
    {
        // digitalWrite(DORS485SELECT,LOW);
        // digitalWrite(DOHALFDUBLIX,LOW);
    }
    else if (ThisProduct.PmmSerial[0].Interface == 232)
    {
        //    digitalWrite(DORS485SELECT,LOW);
        //     digitalWrite(DOHALFDUBLIX,HIGH);
    }
    else // RS485
    {
        // digitalWrite(DORS485SELECT,HIGH);
        // digitalWrite(DOHALFDUBLIX,HIGH);
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
    for (int i = 0; i < 4; i++)
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
            int packetSize = PmmUDP[i].parsePacket(); // Size Of Fiber Packet
            if (packetSize)
            {
                PmmUDP[i].read(packetBuffer, PACKET_MAX_SIZE);
                PmmUDP[i].clearWriteError();
                PmmUDP[i].flush();
                serials[i]->write(packetBuffer, packetSize); // Write in Serial

                // Serial.write(packetBuffer, packetSize); // Write in Serial
                //  SerialUSB.write(packetBuffer, packetSize);    // For debuging
                memset(packetBuffer, 0, sizeof packetBuffer); // Empty packetBuffer
            }

            // STEP03 From Serial to fiber
            // if (Serial.available() > 0)
            if (serials[i]->available() > 0)
            {
                // Serial.setTimeout(50);
                serials[i]->setTimeout(50);
                // int msgLength = Serial.readBytes(rcvBuffer, 64);
                int msgLength = serials[i]->readBytes(rcvBuffer, 64);
                SerialUSB.write(rcvBuffer, msgLength);
                PmmUDP[i].beginPacket(RemotIP, ThisProduct.PmmSerial[0].Port01 /*DevicePort*/);
                PmmUDP[i].write(rcvBuffer, msgLength);
                PmmUDP[i].endPacket();
                PmmUDP[i].clearWriteError();
                PmmUDP[i].flush();
            }
        }
        else if (SERIALSERVERMODE)
        {
            PmmEthernetServer cmdServer(PortsArray[i]);
            // STEP01: wait for a new client:
            PmmEthernetClient client = cmdServer.available();

            char c;

            // STEP02: transfer all bytes from client to Serial
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
                    serials[i]->write(c);
                }
            }

            // STEP03: transfer all bytes to the cmdServer
            //  (all connected clients will recieve it)
            if (serials[i]->available() > 0)
            {
                c = serials[i]->read();
                cmdServer.write(c);
            }
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
    // // if there's data available, read a packet
    // int packetSize = PmmUDP.parsePacket();
    // if (packetSize)
    // {

    //     // read the packet into packetBufffer
    //     PmmUDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    //     packetBuffer[packetSize] = '\0';

    //     if (strcmp(packetBuffer, "?") == 0)
    //     {
    //         // send a reply, to the IP address and port that sent us the packet we received
    //         PmmUDP.beginPacket(PmmUDP.remoteIP(), PmmUDP.remotePort());
    //         PmmUDP.write(ReplyBuffer);
    //         PmmUDP.endPacket();
    //     }
    //}
}

#ifdef PMMHTTPSERVER

void ViewSettingsPage(PmmEthernetClient client);

String PmmReturnAPIFromHTTPHeader()
{
    // SerialUSB.println("Test incoming message ..");

    String APIData = "";
    String Data = "";
    String readString = " ";
    PmmEthernetClient client = server.available();
    if (client)
    {
        SerialUSB.println("Client reciveing");
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();

                if (readString.length() < 1200)
                {
                    readString += c;
                }
                if (c == '\n')
                {
                    if (readString.indexOf("api?message") >= 0)
                    {
                        APIData = readString.substring(readString.indexOf("api?message=") + 12, readString.indexOf("HTTP/1.1"));
                        string cmd = APIData.c_str();
                        Data = PmmCommnads(cmd);

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close"); // the connection will be closed after completion of the response
                        // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println();
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println(Data);
                        client.println("</head>");
                        client.println("<html>");

                        client.stop();
                        // SerialUSB.println(readString);
                        // SerialUSB.println(APIData);
                    }
                    else
                    {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close"); // the connection will be closed after completion of the response
                        // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println();
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");
                        client.println("<head>");

                        ViewSettingsPage(client);

                        client.println("</body>");
                        client.println("</html>");

                        client.stop();
                    }
                }
            }
        }
    }
    if (Data != "")
        Data = "Result : " + Data;
    return Data;
}

void PmmReadHttpCommands()
{
    ThisProduct.EthernetRunning = true;

    if (ThisProduct.EthernetRunning == true)
    {
        String tmpString = PmmReturnAPIFromHTTPHeader(); // http://192.168.1.107/api?message=900,001,000

        if (tmpString != "")
        {
            SerialUSB.print("Recived Http command : ");
            SerialUSB.println(tmpString);
        }
    }
}

void ViewSettingsPage(PmmEthernetClient client)
{
    client.println("<title>PMM-Easy Config</title> <style>");
    client.println(".footer{position: fixed; bottom: 5px; color: var(--contentTextColor); text-align: center; font-size: 13px; padding: 2px; -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none}.footer p{display: contents}body{font-family: Arial, sans-serif; margin: 0; padding: 0; overflow-x: hidden;}#navbar{background-color: #339898; color: #fff; padding: 10px; height: 44px; width: 100%;}");
    client.println("#leftbar{background-color: #339898; color: #fff; width: 190px; padding: 10px; position: absolute; top: 0; left: 0; height: 100%; margin-top: 64px;}#content{margin-left: 210px; padding: 20px;}.content-div{background-color: #eee; padding: 10px; margin-bottom: 10px; height: 400px; display: none;}span{color: #fff;}ul{list-style: none;}li{padding: 12px; width: 92%;}#div1, #div2, #div3{display: none;}");
    client.println(".button{text-align: center; text-decoration: none; display: inline-block; cursor: pointer; width: 40px; height: 40px;}.button2{text-align: center; text-decoration: none; display: inline-block; cursor: pointer; width: 80px; height: 40px;}textarea{width: 89%; height: 40px; padding: 8px 8px; box-sizing: border-box; border: 2px solid #ccc; border-radius: 4px; font-size: 12px; resize: none;}");
    client.println(".ddl{width: 192px; height: 28px; box-sizing: border-box; border: 2px solid #ccc;}.input{width: 192px; height: 28px; box-sizing: border-box; border: 2px solid #ccc;}.input.udpport{width: 45px; position: static;}.button3{width: 140px; height: 28px; position: absolute;}.container{display: block; position: relative; cursor: pointer; font-size: 16px; -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none;}");
    client.println(".container input{position: absolute; opacity: 0; cursor: pointer; height: 0; width: 0;}.checkmark{position: absolute; top: -6px; left: 307px; height: 25px; width: 25px; background-color: #ffffff; border: solid #339898;}.container:hover input~.checkmark{background-color: #339898;}.container input:checked~.checkmark{background-color: #339898;}.checkmark:after{content: ''; position: absolute; display: none;}");
    client.println(".container input:checked~.checkmark:after{display: block;}.container .checkmark:after{left: 9px; top: 5px; width: 5px; height: 10px; border: solid white; border-width: 0 3px 3px 0; -webkit-transform: rotate(45deg); -ms-trasform: rotate(45deg); transform: rotate(45deg);}.Divs.active{background-color: #fff; color: #339898;}#LeftBarContent{display: none;}.Group_Item{display: block; margin-bottom: 16px;}.Group_Item_lbl{width: 300px; display: inline-block;}");
    client.println(".Group_Item_input{width: 300px; display: inline-block;}</style></head>");
    
    client.println("<body> <div id='navbar'> <h1 style='position: absolute;top: -6px;left: 26px;'>PMM</h1> <h4 style='position: absolute;top: 7px;left: 112px;'>Easy Config</h4> </div>");
    client.println("<div id='leftbar'> <ul style='margin-left: -10%;' id='LeftBarContent'> <li class='Divs' onclick='GoToDiv(\"home\");' id='home_btn'> <span id='home_span' style='font-size: 14px;'> Home</span> </li>");
    client.println("<li class='Divs' onclick='GoToDiv(\"div1\");' id='div1_btn'> <span id='div1_span' style='font-size: 14px;'> Ethernet</span> </li>");
    client.println("<li class='Divs' onclick='GoToDiv(\"div2\");' id='div2_btn'> <span id='div2_span' style='font-size: 14px;'> Serials</span> </li>");
    client.println("<li class='Divs' onclick='GoToDiv(\"div3\");' id='div3_btn'> <span id='div3_span' style='font-size: 14px;'> Operation Mode</span> </li>");
    client.println("<button class='button3' onclick='ReadFromController();' style='margin-top: 10%;'>Upload</button><br><br><button class='button3' onclick='WriteToController();' style='margin-top: 10%;'>Download</button><br><br>");
    client.println("<button class='button3' onclick='ResetMCU();' style='margin-top: 10%;'>Reset MCU</button><br><br></ul> <br><br><ul style='margin-left: -10%;' id=''> <input style='width: 140px;left: 15%;' type='Password' class='input' id='Password_txt' name='Password' value='1976'><br>");
    client.println("<button class='button3' onclick='Login();' style='margin-top: 10%;' id='Login_btn'>Login</button> <button class='button3' onclick='Logout();' style='margin-top: 10%;display: none;' id='Logout_btn'>Logout</button><br><br></ul> <div class='footer'> <p>&#169;2014-</p><span id='thisYear'> </span><br>");
    client.println("<p>PMM LLC. All rights reserved.</p><br><a style='text-decoration: none;color: #fff;' href='https://pmm-usa.us/'>www.pmm-usa.us</a> </div></div><div id='content'> ");
    client.println("<div class='content-div' id='home' style='display: inline;position: absolute;width: 86%;'> <h2>Home</h2> ");
    //Hardware info --------------------------------------------------------
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label>Product Name</label> </div><div class='Group_Item_lbl'> <label>PMM 0501</label> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label>Serial Number</label> </div><div class='Group_Item_lbl'> <label>20230923</label> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label>Hardware Version</label> </div><div class='Group_Item_lbl'> <label>V 1.0</label> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label>Software Version</label> </div><div class='Group_Item_lbl'> <label>V 1.0</label> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label>Current IP Address</label> </div><div class='Group_Item_lbl'> <label id='CurrentIPAddress'></label> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label>Number Of Serials</label> </div><div class='Group_Item_lbl'> <label id='NumberOfSerials'>4</label> </div></div></div>");
    //----------------------------------------------------------------------
    client.println("<div class='content-div' id='div1' style='display: inline;position: absolute;width: 86%;'> <h2>Ethernet</h2> ");
    client.println("<div class='Group_Item'> <label class='container'>Enabled <input type='checkbox' id='Enabled_Ethernet'> <span class='checkmark'></span> </label> </div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='IPAddress'>IP Address</label> </div><div class='Group_Item_input'> <input type='text' class='input' id='IPAddress' name='IPAddress' onchange='GetIPAddress();' value='192.168.1.100'> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='NetMask'>Net Mask</label> </div><div class='Group_Item_input'> <input type='text' class='input' id='NetMask' name='NetMask' value='255.255.255.0'> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='PreferredDNS'>Preferred DNS</label> </div><input type='text' class='input' id='PreferredDNS' name='PreferredDNS' value='8.8.8.8'> </div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='AlternateDNS'>Alternate DNS</label> </div><div class='Group_Item_input'> <input type='text' class='input' id='AlternateDNS' name='AlternateDNS' value='8.8.4.4'> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='Gateway'>Gateway</label> </div><div class='Group_Item_input'> <input type='text' class='input' id='Gateway' name='Gateway' value='192.168.1.1'> </div></div>");
    client.println("<div class='Group_I tem'> <div class='Group_Item_lbl'> <label for='MacAddress'>Mac Address</label> </div><div class='Group_Item_input'> <label for='MacAddress' style='left: 24%;top: 40%;'>48:7E</label> <input type='text' class='input' id='MacAddress' name='MacAddress' value='72:64:64:32' style='width: 122px; margin-left: 22px;'> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='UDPPort' style='margin-right: 142px;'>UDP Port</label> </div><div class='Group_Item_input'> <input type='number' class='input udpport' id='UDPPort' name='UDPPort' style='' value='502'> <input type='number' class='input udpport' id='UDPPort2' name='UDPPort2' style='' value='503'> <input type='number' class='input udpport' id='UDPPort3' name='UDPPort3' style='' value='504'> <input type='number' class='input udpport' id='UDPPort4' name='UDPPort4' style='' value='505'> </div></div></div>");
    client.println("<div class='content-div' id='div2' style='display: inline;position: absolute;width: 86%;'> <h2>Serials</h2> ");
    client.println("<div class='Group_Item'> <label class='container'>Enabled <input type='checkbox' id='Enabled_Serial'> <span class='checkmark'></span> </label> </div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'><label for='SerialPort'>Serial Port</label></div><div class='Group_Item_input'> <select id='SerialPort' class='ddl'></select> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='BaudRate'>Baud Rate</label> </div><div class='Group_Item_input'> <select id='BaudRate' class='ddl'> <option value='300'>300</option> <option value='600'>600</option> <option value='1200'>1200</option> <option value='2400'>2400</option> <option value='4800'>4800</option> <option value='9600'>9600</option> <option value='14400'>14400</option> <option value='19200'>19200</option> <option value='38400'>38400</option> <option value='56000'>56000</option> <option value='57600'>57600</option> <option value='115200'>115200</option> </select> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='DataBit'>Data Bit</label> </div><div class='Group_Item_input'> <select id='DataBit' class='ddl'> <option value='5'>5</option> <option value='6'>6</option> <option value='7'>7</option> <option value='8'>8</option> </select> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='StopBit'>Stop Bit</label> </div><div class='Group_Item_input'> <select id='StopBit' class='ddl'> <option value='1'>1</option> <option value='2'>2</option> </select> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='Parity'>Parity</label> </div><div class='Group_Item_input'> <select id='Parity' class='ddl'> <option value='None'>None</option> <option value='Odd'>Odd</option> <option value='Even'>Even</option> </select> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='Interface'>Interface</label> </div><div class='Group_Item_input'> <select id='Interface' class='ddl'> <option value='485'>RS485</option> <option value='422'>RS422</option> <option value='232'>RS232</option> </select> </div></div>");
    client.println("<div class='Group_Item'> <div class='Group_Item_lbl'> <label for='PortID'>Port ID</label> </div><div class='Group_Item_input'> <input type='number' class='input' id='PortID' name='PortID' style='width:100px;' value='1'> <label style='margin-left: 40px;'>(1-30)</label> </div></div></div>");
    client.println("<div class='content-div' id='div3' style='display: inline;position: absolute;width: 86%;'> <h2>Operation Mode</h2> ");
    client.println("<div class='Group_Item'><input type='radio' id='SerialServer' name='Operation_Mode' value='SerialServer' checked><label for='SerialServer'>Serial Server</label><br><p style='margin-left: 3%;'>multi ethernet client can communicate with serial port on this device</p></div>");
    client.println("<div class='Group_Item'><input type='radio' id='PeerToPeer' name='Operation_Mode' value='PeerToPeer'><label for='PeerToPeer'>Peer To Peer</label><br><label for='RemoteIP' style='top: 6px;position: relative;margin-left: 42px;'>Remote IP : </label> <label style='top: 6px;position: relative;' id='IpAddress_txt'>192.168.1.</label> <input type='number' min='1' class='input' id='RemoteIP' name='RemoteIP' style='width:50px;margin-left: 10px;position: absolute;' value='200'> </div></div></div>");
    
    client.println("<script>var d=new Date(); var y=d.getFullYear(); document.getElementById('thisYear').textContent=y; let CommandStr=''; let SerialConfig=1043; var ip=(window.location.href).split('//')[1]; document.getElementById('CurrentIPAddress').textContent=ip.slice(0, -1);AppendSerials(); ReadFromController2(); CheckLogin(); ");
    client.println("function AppendSerials(){for(var e=parseInt(document.getElementById(\"NumberOfSerials\").textContent),t=0;t<e;t++){var r=document.createElement(\"option\");r.text=\"Serial 0\"+t,r.value=\"Serial0\"+t,document.getElementById(\"SerialPort\").appendChild(r)}}");
    client.println("function CheckLogin(){if (localStorage.getItem('Login')=='true'){document.getElementById('Password_txt').style.display='none'; document.getElementById('Login_btn').style.display='none'; document.getElementById('Logout_btn').style.display='block'; document.getElementById('LeftBarContent').style.display='block'; GoToDiv('home'); ReadFromController();}else if (localStorage.getItem('Login')=='false'){document.getElementById('Password_txt').style.display='block'; document.getElementById('Login_btn').style.display='block'; document.getElementById('Logout_btn').style.display='none'; document.getElementById('home').style.display='none'; document.getElementById('div1').style.display='none'; document.getElementById('div2').style.display='none'; document.getElementById('div3').style.display='none'; document.getElementById('LeftBarContent').style.display='none';}}");
    client.println("function Login(){var pass=document.getElementById('Password_txt').value; var pinCode=''; const xhr=new XMLHttpRequest(); const url=window.location.href + '/api?message=961,000,000'; xhr.open('GET', url, true); xhr.onreadystatechange=function (){if (xhr.readyState===4 && xhr.status===200){const responseText=xhr.responseText; pinCode=responseText;}}; xhr.send(); if (pass=='1976' || pass==pinCode){document.getElementById('Password_txt').style.display='none'; document.getElementById('Login_btn').style.display='none'; document.getElementById('Logout_btn').style.display='block'; document.getElementById('LeftBarContent').style.display='block'; GoToDiv('home'); ReadFromController(); localStorage.setItem('Login', 'true');}else{document.getElementById('Password_txt').style.display='block'; document.getElementById('Login_btn').style.display='block'; document.getElementById('Logout_btn').style.display='none'; document.getElementById('home').style.display='none'; document.getElementById('div1').style.display='none'; document.getElementById('div2').style.display='none'; document.getElementById('div3').style.display='none'; document.getElementById('LeftBarContent').style.display='none'; localStorage.setItem('Login', 'false');}}");
    client.println("function Logout(){document.getElementById('Password_txt').style.display='block'; document.getElementById('Login_btn').style.display='block'; document.getElementById('Logout_btn').style.display='none'; document.getElementById('LeftBarContent').style.display='none'; document.getElementById('home').style.display='none'; document.getElementById('div1').style.display='none'; document.getElementById('div2').style.display='none'; document.getElementById('div3').style.display='none'; localStorage.setItem('Login', 'false');}");
    client.println("function GetIPAddress(){var ipAddress=document.getElementById('IPAddress').value; var ipAddressArr=ipAddress.split('.'); var LastIP=ipAddressArr[0] + '.' + ipAddressArr[1] + '.' + ipAddressArr[2] + '.'; document.getElementById('IpAddress_txt').textContent=LastIP;}");
    client.println("function GoToDiv(div){");
    client.println("if (div=='home'){document.getElementById('home').style.display='block'; document.getElementById('div1').style.display='none'; document.getElementById('div2').style.display='none'; document.getElementById('div3').style.display='none'; document.getElementById('home_btn').classList.add('active'); document.getElementById('div1_btn').classList.remove('active'); document.getElementById('div2_btn').classList.remove('active'); document.getElementById('div3_btn').classList.remove('active'); document.getElementById('home_span').style.color='#339898'; document.getElementById('div1_span').style.color='#ffffff'; document.getElementById('div2_span').style.color='#ffffff'; document.getElementById('div3_span').style.color='#ffffff';}");
    client.println("else if (div=='div1'){document.getElementById('home').style.display='none'; document.getElementById('div1').style.display='block'; document.getElementById('div2').style.display='none'; document.getElementById('div3').style.display='none'; document.getElementById('home_btn').classList.remove('active'); document.getElementById('div1_btn').classList.add('active'); document.getElementById('div2_btn').classList.remove('active'); document.getElementById('div3_btn').classList.remove('active'); document.getElementById('home_span').style.color='#ffffff'; document.getElementById('div1_span').style.color='#339898'; document.getElementById('div2_span').style.color='#ffffff'; document.getElementById('div3_span').style.color='#ffffff';}");
    client.println("else if (div=='div2'){document.getElementById('home').style.display='none'; document.getElementById('div1').style.display='none'; document.getElementById('div2').style.display='block'; document.getElementById('div3').style.display='none'; document.getElementById('home_btn').classList.remove('active'); document.getElementById('div1_btn').classList.remove('active'); document.getElementById('div2_btn').classList.add('active'); document.getElementById('div3_btn').classList.remove('active'); document.getElementById('home_span').style.color='#ffffff'; document.getElementById('div1_span').style.color='#ffffff'; document.getElementById('div2_span').style.color='#339898'; document.getElementById('div3_span').style.color='#ffffff';}");
    client.println("else if (div=='div3'){document.getElementById('home').style.display='none'; document.getElementById('div1').style.display='none'; document.getElementById('div2').style.display='none'; document.getElementById('div3').style.display='block'; document.getElementById('home_btn').classList.remove('active'); document.getElementById('div1_btn').classList.remove('active'); document.getElementById('div2_btn').classList.remove('active'); document.getElementById('div3_btn').classList.add('active'); document.getElementById('home_span').style.color='#ffffff'; document.getElementById('div1_span').style.color='#ffffff'; document.getElementById('div2_span').style.color='#ffffff'; document.getElementById('div3_span').style.color='#339898';}}");
    client.println("function convertIPToDecimal(b1, b2, b3, b4){var binaryString=b4.toString(2).padStart(8, '0') + b3.toString(2).padStart(8, '0') + b2.toString(2).padStart(8, '0') + b1.toString(2).padStart(8, '0'); return parseInt(binaryString, 2).toString();}function convertMacToDecimal(byte1, byte2, byte3, byte4){var macBytes=[byte1, byte2, byte3, byte4]; var binaryString=''; for (var i=3; i >=0; i--){binaryString +=macBytes[i].toString(2).padStart(8, '0');}return parseInt(binaryString, 2).toString();}");
    client.println("function numberToByte(number){if (number >=0 && number <=255){return number & 0xFF;}else{return null;}}function convertBoolToString(val){var result='0'; if (val !==null && val !==undefined){val=val.toString().toLowerCase(); if (val==='false' || val==='0'){result='0';}else if (val==='true' || val==='1'){result='1';}}return result;}");
    client.println("function WriteToController(){var sparity=document.getElementById('Parity').value; let parity=1; if (sparity==='None') parity=3; if (sparity==='Odd') parity=2; ");
    client.println("var Val1=convertIPToDecimal( parseInt(document.getElementById('StopBit').value), parseInt(document.getElementById('DataBit').value), parseInt(parity), 1 ); var Val2=convertMacToDecimal( numberToByte(document.getElementById('MacAddress').value.split(':')[0]), numberToByte(document.getElementById('MacAddress').value.split(':')[1]), numberToByte(document.getElementById('MacAddress').value.split(':')[2]), numberToByte(document.getElementById('MacAddress').value.split(':')[3]) ); ");
    client.println("var Val3=convertIPToDecimal( parseInt((document.getElementById('IPAddress').value).split('.')[0]), parseInt((document.getElementById('IPAddress').value).split('.')[1]), parseInt((document.getElementById('IPAddress').value).split('.')[2]), parseInt((document.getElementById('IPAddress').value).split('.')[3]) ); var Val4=convertIPToDecimal( parseInt((document.getElementById('NetMask').value).split('.')[0]), parseInt((document.getElementById('NetMask').value).split('.')[1]), parseInt((document.getElementById('NetMask').value).split('.')[2]), parseInt((document.getElementById('NetMask').value).split('.')[3]) ); ");
    client.println("var Val5=convertIPToDecimal( parseInt((document.getElementById('Gateway').value).split('.')[0]), parseInt((document.getElementById('Gateway').value).split('.')[1]), parseInt((document.getElementById('Gateway').value).split('.')[2]), parseInt((document.getElementById('Gateway').value).split('.')[3]) ); var Val6=convertIPToDecimal( parseInt((document.getElementById('PreferredDNS').value).split('.')[0]), parseInt((document.getElementById('PreferredDNS').value).split('.')[1]), parseInt((document.getElementById('PreferredDNS').value).split('.')[2]), parseInt((document.getElementById('PreferredDNS').value).split('.')[3]) ); ");
    client.println("var Val7=convertIPToDecimal( parseInt((document.getElementById('AlternateDNS').value).split('.')[0]), parseInt((document.getElementById('AlternateDNS').value).split('.')[1]), parseInt((document.getElementById('AlternateDNS').value).split('.')[2]), parseInt((document.getElementById('AlternateDNS').value).split('.')[3]) ); SerialConfig=calcSerialConfig( parseInt(document.getElementById('DataBit').value), document.getElementById('Parity').value, parseInt(document.getElementById('StopBit').value) ); var PeerToPeer='0'; var SerialServer='0'; var ele=document.getElementsByName(\"Operation_Mode\"),Operation_Mode=\"PeerToPeer\";for(i=0;i<ele.length;i++)ele[i].checked&&(Operation_Mode=ele[i].value); if (Operation_Mode=='PeerToPeer'){PeerToPeer='1';}if (Operation_Mode=='SerialServer'){SerialServer='1';}var EnabledSerial='0'; var EnabledEthernet='0'; var Enabled_Ethernet=document.getElementById('Enabled_Ethernet').checked; var Enabled_Serial=document.getElementById('Enabled_Serial').checked; if (Enabled_Ethernet){EnabledEthernet='1';}if (Enabled_Serial){EnabledSerial='1';}");
    client.println("var Bits=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString('0'); Bits +=convertBoolToString(PeerToPeer); Bits +=convertBoolToString(SerialServer); Bits +=convertBoolToString(EnabledSerial); Bits +=convertBoolToString(EnabledEthernet);");
    client.println("var val='111'; var serialport=document.getElementById('SerialPort').value; switch (serialport){case 'Serial00': val='110'; break; case 'Serial01': val='112'; break; case 'Serial02': val='114'; break; case 'Serial03': val='116'; break;}");
    client.println("var CmdWriteEthernet_Serial_00=val + ',000,'; CmdWriteEthernet_Serial_00 +=Val1 + ','; CmdWriteEthernet_Serial_00 +=SerialConfig + ','; CmdWriteEthernet_Serial_00 +=document.getElementById('BaudRate').value + ','; CmdWriteEthernet_Serial_00 +='35,'; CmdWriteEthernet_Serial_00 +='36,'; CmdWriteEthernet_Serial_00 +='1,'; CmdWriteEthernet_Serial_00 +='1,'; CmdWriteEthernet_Serial_00 +=document.getElementById('Interface').value + ','; CmdWriteEthernet_Serial_00 +=Val2 + ','; ");
    client.println("CmdWriteEthernet_Serial_00 +=Val3 + ','; CmdWriteEthernet_Serial_00 +=Val4 + ','; CmdWriteEthernet_Serial_00 +=Val5 + ','; CmdWriteEthernet_Serial_00 +=Val6 + ','; CmdWriteEthernet_Serial_00 +=Val7 + ','; CmdWriteEthernet_Serial_00 +=document.getElementById('UDPPort').value + ','; CmdWriteEthernet_Serial_00 +=document.getElementById('UDPPort2').value + ','; CmdWriteEthernet_Serial_00 +=document.getElementById('UDPPort3').value + ','; CmdWriteEthernet_Serial_00 +=document.getElementById('UDPPort4').value + ','; CmdWriteEthernet_Serial_00 +=document.getElementById('PortID').value + ','; CmdWriteEthernet_Serial_00 +='0,'; ");
    client.println("CmdWriteEthernet_Serial_00 +=document.getElementById('RemoteIP').value + ','; CmdWriteEthernet_Serial_00 +=parseInt(Bits, 2); console.log (CmdWriteEthernet_Serial_00); SendRequest(CmdWriteEthernet_Serial_00); ResetMCU();}");
    client.println("function ReadFromController(){var val='111'; var serialport=document.getElementById('SerialPort').value; switch (serialport){case 'Serial00': val='111'; break; case 'Serial01': val='113'; break; case 'Serial02': val='115'; break; case 'Serial03': val='117'; break;}CommandStr=val + ',000,000'; const xhr=new XMLHttpRequest(); const url=window.location.href + '/api?message=' + CommandStr; xhr.open('GET', url, true); xhr.onreadystatechange=function (){if (xhr.readyState===4 && xhr.status===200){const responseText=xhr.responseText; SetDataToFild(responseText); console.log (responseText);}}; xhr.send();}");
    client.println("function ReadFromController2(){const xhr=new XMLHttpRequest(); const url=window.location.href + '/api?message=111,000,000'; xhr.open('GET', url, true); xhr.onreadystatechange=function (){if (xhr.readyState===4 && xhr.status===200){const responseText=xhr.responseText; SetDataToFild(responseText);}}; xhr.send();}");
    client.println("function ResetMCU(){CommandStr='948,001,000'; SendRequest(CommandStr);}");
    client.println("function SendRequest(CommandStr){const xhr=new XMLHttpRequest(); const url=window.location.href + '/api?message=' + CommandStr; xhr.open('GET', url, true); xhr.onreadystatechange=function (){if (xhr.readyState===4 && xhr.status===200){const responseText=xhr.responseText; console.log(responseText);}}; xhr.send();}");
    client.println("function calcSerialConfig(sdatabit, sparity, sstopbit){let databitLong=0x1; let parityLong=0; let stopbitLong=0x400; let parity=1; if (sparity==='None') parity=3; if (sparity==='Odd') parity=2; let databit=parseInt(sdatabit); let stopbit=parseInt(sstopbit); stopbit=stopbit * 10; if (parity===1){parityLong=0x1;}else if (parity===2){parityLong=0x2;}else if (parity===3){parityLong=0x3;}else if (parity===4){parityLong=0x4;}else if (parity===5){parityLong=0x5;}else if (parity===6){parityLong=0xF;}if (stopbit===10){stopbitLong=0x10;}else if (stopbit===15){stopbitLong=0x20;}else if (stopbit===20){stopbitLong=0x30;}else{stopbitLong=0xF0;}if (databit===5){databitLong=0x100;}else if (databit===6){databitLong=0x200;}else if (databit===7){databitLong=0x300;}else if (databit===8){databitLong=0x400;}else{databitLong=0xF00;}let config=databitLong | parityLong | stopbitLong; return config;}");
    client.println("function SetSerialConfig(){SerialConfig=calcSerialConfig( parseInt(document.getElementById('DataBit').value), document.getElementById('Parity').value, parseInt(document.getElementById('StopBit').value) );}const selectElement1=document.getElementById('DataBit'); selectElement1.addEventListener('change', function (){SetSerialConfig();}); const selectElement2=document.getElementById('Parity'); selectElement2.addEventListener('change', function (){SetSerialConfig();}); const selectElement3=document.getElementById('StopBit'); selectElement3.addEventListener('change', function (){SetSerialConfig();});");
    client.println("function ConvertDecimalToIP(decimalValue){let decimalNumber=parseInt(decimalValue); let binaryString=decimalNumber.toString(2).padStart(32, '0'); let ip=new Array(4); ip[3]=parseInt(binaryString.substring(24, 32), 2); ip[2]=parseInt(binaryString.substring(16, 24), 2); ip[1]=parseInt(binaryString.substring(8, 16), 2); ip[0]=parseInt(binaryString.substring(0, 8), 2); return ip;}");
    client.println("function ConvertDecimalToMac(decimalValue){let decimalNumber=BigInt(decimalValue); let binaryString=decimalNumber.toString(2).padStart(32, '0'); let macBytes=new Uint8Array(4); for (let i=0; i < 4; i++){macBytes[i]=parseInt(binaryString.substring(i * 8, (i + 1) * 8), 2);}return [macBytes[3], macBytes[2], macBytes[1], macBytes[0]];}");
    client.println("function ConvertDecimalToBool(value){let binary=value.toString(2).padStart(16, '0'); return binary;}");
    client.println("function ConvertStringToBool(value){var result = false; if(value =='1'){result=true;}else{result=false;} return result;}");
    client.println("function SetDataToFild(CommResponse){var values=(CommResponse.replace(/<[^>]*>/g, '')).split(','); SetSerialSettings(values[0]); SerialConfig=parseInt(values[1]); document.getElementById('BaudRate').value=parseInt(values[2]); document.getElementById('Interface').value=parseInt(values[7]); let macBytes=ConvertDecimalToMac(values[8]); document.getElementById('MacAddress').value=macBytes[0] + ':' + macBytes[1] + ':' + macBytes[2] + ':' + macBytes[3]; ");
    client.println("let ipAddress=ConvertDecimalToIP(values[9]); document.getElementById('IPAddress').value=ipAddress[3] + '.' + ipAddress[2] + '.' + ipAddress[1] + '.' + ipAddress[0]; let subnetMask=ConvertDecimalToIP(values[10]); document.getElementById('NetMask').value=subnetMask[3] + '.' + subnetMask[2] + '.' + subnetMask[1] + '.' + subnetMask[0]; let gateway=ConvertDecimalToIP(values[11]); document.getElementById('Gateway').value=gateway[3] + '.' + gateway[2] + '.' + gateway[1] + '.' + gateway[0]; let dns1=ConvertDecimalToIP(values[12]); document.getElementById('PreferredDNS').value=dns1[3] + '.' + dns1[2] + '.' + dns1[1] + '.' + dns1[0]; let dns2=ConvertDecimalToIP(values[13]); document.getElementById('AlternateDNS').value=dns2[3] + '.' + dns2[2] + '.' + dns2[1] + '.' + dns2[0]; document.getElementById('UDPPort').value=parseInt(values[14]); document.getElementById('UDPPort2').value=parseInt(values[15]); document.getElementById('UDPPort3').value=parseInt(values[16]); document.getElementById('UDPPort4').value=parseInt(values[17]); document.getElementById('PortID').value=parseInt(values[18]); document.getElementById('RemoteIP').value=parseInt(values[20]); ");
    client.println("let bitsString=ConvertDecimalToBool(parseInt(values[21])); var BitStringArr = bitsString.split(''); console.log(BitStringArr); let Enabled_Ethernet=document.getElementById('Enabled_Ethernet'); if (ConvertStringToBool(BitStringArr[15])== true){Enabled_Ethernet.checked=true;}else{Enabled_Ethernet.checked=false;}let Enabled_Serial=document.getElementById('Enabled_Ethernet'); ");
    client.println("if (ConvertStringToBool(BitStringArr[14]) == true){Enabled_Serial.checked=true;}else{Enabled_Serial.checked=false;}var Operation_Mode='SerialServer'; if (ConvertStringToBool(BitStringArr[13])== true){Operation_Mode='SerialServer';}if (ConvertStringToBool(BitStringArr[12])== true){Operation_Mode='PeerToPeer';}let radioButtons=document.querySelectorAll(`input[type='radio'][name='${Operation_Mode}']`); radioButtons.forEach(function (radioButton){radioButton.checked=true;});}");
    client.println("function SetSerialSettings(intValue) {let int32Value = parseInt(intValue); let int8Array = new Array(4);int8Array[0] = (int32Value >> 24) & 0xFF;int8Array[1] = (int32Value >> 16) & 0xFF;int8Array[2] = (int32Value >> 8) & 0xFF;int8Array[3] = int32Value & 0xFF;document.getElementById('StopBit').value = int8Array[3];document.getElementById('DataBit').value = int8Array[2];let parity = 'Even';if (int8Array[1] == 3) parity = 'None';if (int8Array[1] == 2) parity = 'Odd';document.getElementById('Parity').value = parity;}");
    client.println("</script>");

}

#endif

#endif
