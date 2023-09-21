
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

// void ViewSettingsPage(PmmEthernetClient client)
// {
//     client.println("<title>PMM-Easy Config</title> <style>.footer{position:fixed;bottom:5px;color:var(--contentTextColor);text-align:center;font-size:13px;padding:2px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.footer p{display:contents}body{font-family:Arial,sans-serif;margin:0;padding:0;overflow-x:hidden}#navbar{background-color:#339898;color:#fff;padding:10px;height:44px;width:100%}");
//     client.println("#leftbar,#navbar{background-color:#339898;color:#fff}#leftbar,#navbar,span{color:#fff}.button,.button2{text-align:center;text-decoration:none;display:inline-block;cursor:pointer}.ddl,.input,textarea{box-sizing:border-box}.button,.button2,.container{cursor:pointer}.footer{position:fixed;bottom:5px;color:var(--contentTextColor);text-align:center;font-size:13px;padding:2px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.footer p{display:contents}body{font-family:Arial,sans-serif;margin:0;padding:0;overflow-x:hidden}#navbar{padding:10px;height:44px;width:100%}#leftbar{width:190px;padding:10px;position:absolute;top:0;left:0;height:100%;margin-top:64px}#content{margin-left:210px;padding:20px}.content-div{background-color:#eee;padding:10px;margin-bottom:10px;height:350px}ul{list-style:none}li{padding:12px;width:92%}#div2,#div3{display:none}.button{width:40px;height:40px}.button2{width:80px;height:40px}.container,.container input:checked~.checkmark:after{display:block}.button3,.ddl,.input{height:28px;position:absolute}textarea{width:89%;height:40px;padding:8px;border:2px solid #ccc;border-radius:4px;font-size:12px;resize:none}.ddl,.input{width:10%;left:24%;border:2px solid #ccc}.button3{width:140px}.container{position:relative;font-size:16px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.container input{position:absolute;opacity:0;cursor:pointer;height:0;width:0}.checkmark{position:absolute;top:-6px;left:13.5%;height:25px;width:25px;background-color:#eee}.container:hover input~.checkmark{background-color:#ccc}.container input:checked~.checkmark{background-color:#2196f3}.checkmark:after{content:'';");
//     client.println("position:absolute;display:none}.container .checkmark:after{left:9px;top:5px;width:5px;height:10px;border:solid #fff;border-width:0 3px 3px 0;-webkit-transform:rotate(45deg);-ms-trasform:rotate(45deg);transform:rotate(45deg)}.Divs.active{background-color:#fff;color:#339898}</style></head>");
//     client.println("<body><div id=\"navbar\">\n        <h1 style=\"position: absolute;top: -6px;left: 26px;\">PMM</h1>\n        <h4 style=\"position: absolute;top: 7px;left: 112px;\">Easy Config 0501</h4>\n    </div>\n    <div id=\"leftbar\">\n        <ul style=\"margin-left: -10%;\">\n            <li class=\"Divs\" onclick=\"GoToDiv('div1');\" id=\"div1_btn\">\n                <span id=\"div1_span\" style=\"font-size: 14px;\"> Ethernet (Serial 00)</span>\n            </li>\n            <li class=\"Divs\" onclick=\"GoToDiv('div2');\" id=\"div2_btn\">\n                <span id=\"div2_span\" style=\"font-size: 14px;\"> Serial (Serial 00)</span>\n            </li>\n            <li class=\"Divs\" onclick=\"GoToDiv('div3');\" id=\"div3_btn\">\n                <span id=\"div3_span\" style=\"font-size: 14px;\"> Operation Mode</span>\n            </li>\n            <button class=\"button3\" onclick=\"ReadFromController();\" style=\"margin-top: 10%;\">Upload</button><br><br>\n            <button class=\"button3\" onclick=\"WriteToController();\" style=\"margin-top: 10%;\">Download</button><br><br>\n            <button class=\"button3\" onclick=\"ResetMCU();\" style=\"margin-top: 10%;\">Reset MCU</button><br><br>\n        </ul>\n\n        <div class=\"footer\">\n            <p>&#169;2014-</p><span id=\"thisYear\"> </span><br>\n            <p>PMM LLC. All rights reserved.</p><br>\n            <a style=\"text-decoration: none;color: #fff;\" href=\"https://pmm-usa.us/\">www.pmm-usa.us</a>\n        </div>\n    </div>");
//     client.println("<div id='content'> <div class='content-div' id='div1'> <h2>Ethernet (Serial 00)</h2> <label class='container'>Enabled <input type='checkbox' id='Enabled_Ethernet'> <span class='checkmark'></span> </label><br><label for='IPAddress'>IP Address</label> <input type='text' class='input' id='IPAddress' name='IPAddress' value='192.168.1.100'><br><br><label for='NetMask'>Net Mask</label> <input type='text' class='input' id='NetMask' name='NetMask' value='255.255.255.0'><br><br><label for='PreferredDNS'>Preferred DNS</label> <input type='text' class='input' id='PreferredDNS' name='PreferredDNS' value='8.8.8.8'><br><br><label for='AlternateDNS'>Alternate DNS</label> <input type='text' class='input' id='AlternateDNS' name='AlternateDNS' value='8.8.4.4'><br><br><label for='Gateway'>Gateway</label> <input type='text' class='input' id='Gateway' name='Gateway' value='192.168.1.1'><br><br><label for='MacAddress'>Mac Address</label> <label for='MacAddress' style='position: absolute;left: 24%;top: 40%;'>48:7E</label> <input type='text' class='input' id='MacAddress' name='MacAddress' value='72:64:64:32' style='width: 7%;left: 27%;'><br><br><label for=\"UDPPort\" style=\"margin-right: 142px;\">UDP Port</label>\n            <input type=\"number\" class=\"input udpport\" id=\"UDPPort\" name=\"UDPPort\" style=\"\" value=\"502\">\n            <input type=\"number\" class=\"input udpport\" id=\"UDPPort2\" name=\"UDPPort2\" style=\"\" value=\"503\">\n            <input type=\"number\" class=\"input udpport\" id=\"UDPPort3\" name=\"UDPPort3\" style=\"\" value=\"504\">\n            <input type=\"number\" class=\"input udpport\" id=\"UDPPort4\" name=\"UDPPort4\" style=\"\" value=\"505\"><br><br></div>");
//     client.println("<div class='content-div' id='div2'> <h2>Serial (Serial 00)</h2> <label class='container'>Enabled <input type='checkbox' id='Enabled_Serial'> <span class='checkmark'></span>  <label for=\"SerialPort\">Serial Port</label>\n            <select id=\"SerialPort\" class=\"ddl\">\n                <option value=\"Serial00\">Serial 00</option>\n                <option value=\"Serial01\">Serial 01</option>\n                <option value=\"Serial02\">Serial 02</option>\n                <option value=\"Serial03\">Serial 03</option>\n            </select><br><br></label><br><label for='BaudRate'>Baud Rate</label> <select id='BaudRate' class='ddl'> <option value='300'>300</option> <option value='600'>600</option> <option value='1200'>1200</option> <option value='2400'>2400</option> <option value='4800'>4800</option> <option value='9600'>9600</option> <option value='14400'>14400</option> <option value='19200'>19200</option> <option value='38400'>38400</option> <option value='56000'>56000</option> <option value='57600'>57600</option> <option value='115200'>115200</option> </select><br><br><label for='DataBit'>Data Bit</label> <select id='DataBit' class='ddl'> <option value='5'>5</option> <option value='6'>6</option> <option value='7'>7</option> <option value='8'>8</option> </select><br><br><label for='StopBit'>Stop Bit</label> <select id='StopBit' class='ddl'> <option value='1'>1</option> <option value='2'>2</option> </select><br><br><label for='Parity'>Parity</label> <select id='Parity' class='ddl'> <option value='None'>None</option> <option value='Odd'>Odd</option> <option value='Even'>Even</option> </select><br><br><label for='Interface'>Interface</label> <select id='Interface' class='ddl'> <option value='485'>RS485</option> <option value='422'>RS422</option> <option value='232'>RS232</option> </select><br><br>");
//     client.println(" <label for='PortID'>Port ID</label> <input type='number' class='input' id='PortID' name='PortID' style='width:7%;' value='1'> <label style='margin-left: 16%;\n    position: absolute;\n    top: 43.5%;'>(1-30)</label><br><br></div>");
//     client.println("<div class='content-div' id='div3'> <h2>Operation Mode</h2><input type='radio' id='SerialServer' name='Operation_Mode' value='SerialServer' checked><label for='SerialServer'>Serial Server</label><br><p style='margin-left: 3%;'>multi ethernet client can communicate with serial port on this device</p><br><input type='radio' id='PeerToPeer' name='Operation_Mode' value='PeerToPeer'><label for='PeerToPeer'>Peer To Peer</label><br><label for='RemoteIP' style='top: 6px;position: relative;margin-left: 42px;'>Remote IP</label> <label style='top: 6px;position: relative;margin-left: 7%;'>192.168.1.</label> <input type='number' min='1' class='input' id='RemoteIP' name='RemoteIP' style='width:3%;left: 29%;' value='200'><br><br></div>");
//     client.println("<div class='content-div' id='div4' style='height: 75px;'> <h2>Command</h2> <div style='display: inline-block; position: absolute; margin-left: 10%; width: 80%; top: 51%;'> <textarea id='textarea_cmd'>Command...</textarea> </div></div></div>");
//     client.println("<script>var d = new Date();\n        var y = d.getFullYear();\n        document.getElementById(\"thisYear\").textContent = y;\n        let CommandStr = \"\";\n        let SerialConfig = 1043;\n        GoToDiv(\"div1\");\n        ReadFromController();\n        ");
//     client.println("function GoToDiv(div) {if (div == \"div1\") {document.getElementById(\"div1\").style.display = \"block\";document.getElementById(\"div2\").style.display = \"none\";document.getElementById(\"div3\").style.display = \"none\";document.getElementById(\"div1_btn\").classList.add(\"active\");document.getElementById(\"div2_btn\").classList.remove(\"active\");document.getElementById(\"div3_btn\").classList.remove(\"active\");document.getElementById(\"div1_span\").style.color = \"#339898\";document.getElementById(\"div2_span\").style.color = \"#ffffff\";document.getElementById(\"div3_span\").style.color = \"#ffffff\";}else if (div == \"div2\") {\n                document.getElementById(\"div1\").style.display = \"none\";document.getElementById(\"div2\").style.display = \"block\";document.getElementById(\"div3\").style.display = \"none\";document.getElementById(\"div1_btn\").classList.remove(\"active\");document.getElementById(\"div2_btn\").classList.add(\"active\");document.getElementById(\"div3_btn\").classList.remove(\"active\");document.getElementById(\"div1_span\").style.color = \"#ffffff\";document.getElementById(\"div2_span\").style.color = \"#339898\";document.getElementById(\"div3_span\").style.color = \"#ffffff\";\n            }\n            ");
//     client.println("else if (div == \"div3\") {document.getElementById(\"div1\").style.display = \"none\";document.getElementById(\"div2\").style.display = \"none\";document.getElementById(\"div3\").style.display = \"block\";document.getElementById(\"div1_btn\").classList.remove(\"active\");document.getElementById(\"div2_btn\").classList.remove(\"active\");document.getElementById(\"div3_btn\").classList.add(\"active\");document.getElementById(\"div1_span\").style.color = \"#ffffff\";document.getElementById(\"div2_span\").style.color = \"#ffffff\";document.getElementById(\"div3_span\").style.color = \"#339898\";}}");
//     client.println("function convertIPToDecimal(b1, b2, b3, b4) {\n            var binaryString =\n                b4.toString(2).padStart(8, '0') +\n                b3.toString(2).padStart(8, '0') +\n                b2.toString(2).padStart(8, '0') +\n                b1.toString(2).padStart(8, '0');\n            return parseInt(binaryString, 2).toString();\n        }\n        ");
//     client.println("function convertMacToDecimal(byte1, byte2, byte3, byte4) {\n            var macBytes = [byte1, byte2, byte3, byte4];\n\n            var binaryString = \"\";\n            for (var i = 3; i >= 0; i--) {\n                binaryString += macBytes[i].toString(2).padStart(8, '0');\n            }\n\n            return parseInt(binaryString, 2).toString();\n        }\n        ");
//     client.println("function numberToByte(number) {\n            if (number >= 0 && number <= 255) {\n                return number & 0xFF;\n            } else {\n                return null;\n            }\n        }\n        ");
//     client.println("function convertBoolToString(val) {\n            var result = \"0\";\n            if (val !== null && val !== undefined) {\n                val = val.toString().toLowerCase();\n                if (val === \"false\" || val === \"0\") {\n                    result = \"0\";\n                } else if (val === \"true\" || val === \"1\") {\n                    result = \"1\";\n                }\n            }\n            return result;\n        }\n        ");
//     client.println("function WriteToController() {\n            var sparity = document.getElementById(\"Parity\").value\n            let parity = 1;\n            if (sparity === \"None\") parity = 3;\n            if (sparity === \"Odd\") parity = 2;\n            var Val1 = convertIPToDecimal(\n                parseInt(document.getElementById(\"StopBit\").value),\n                parseInt(document.getElementById(\"DataBit\").value),\n                parseInt(parity),\n                1\n            );\n            var Val2 = convertMacToDecimal(\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[0]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[1]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[2]),\n                numberToByte(document.getElementById(\"MacAddress\").value.split(\":\")[3])\n            );\n            var Val3 = convertIPToDecimal(\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"IPAddress\").value).split(\".\")[3])\n            );\n            var Val4 = convertIPToDecimal(\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"NetMask\").value).split(\".\")[3]));");
//     client.println("var Val5 = convertIPToDecimal(parseInt((document.getElementById(\"Gateway\").value).split(\".\")[0]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[1]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[2]),parseInt((document.getElementById(\"Gateway\").value).split(\".\")[3]));\n            var Val6 = convertIPToDecimal(\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"PreferredDNS\").value).split(\".\")[3])\n            );\n            var Val7 = convertIPToDecimal(\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[0]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[1]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[2]),\n                parseInt((document.getElementById(\"AlternateDNS\").value).split(\".\")[3])\n            );\n\n            SerialConfig = calcSerialConfig(\n                parseInt(document.getElementById(\"StopBit\").value),\n                document.getElementById(\"Parity\").value,\n                parseInt(document.getElementById(\"StopBit\").value)\n            );\n            var PeerToPeer = \"0\";\n            var SerialServer = \"0\";\n            var Operation_Mode = document.querySelector(\"input[name='Operation_Mode']:checked\").value;\n            if (Operation_Mode == \"PeerToPeer\") { PeerToPeer = \"1\"; }\n            if (Operation_Mode == \"SerialServer\") { SerialServer = \"1\"; }\n            var EnabledSerial = \"0\";\n            var EnabledEthernet = \"0\";\n            var Enabled_Ethernet = document.getElementById(\"Enabled_Ethernet\").checked;\n            var Enabled_Serial = document.getElementById(\"Enabled_Serial\").checked;\n            if (Enabled_Ethernet) { EnabledEthernet = \"1\"; }\n         ");
//     client.println("if (Enabled_Serial) { EnabledSerial = \"1\"; }\n            var Bits = convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(\"0\");Bits += convertBoolToString(PeerToPeer);Bits += convertBoolToString(SerialServer);Bits += convertBoolToString(EnabledSerial);Bits += convertBoolToString(EnabledEthernet);");
//     client.println("var val = \"111\";\n            var serialport = document.getElementById(\"SerialPort\").value;\n            switch (serialport) {\n                case \"Serial00\":\n                    val = \"110\";\n                    break;\n                case \"Serial01\":\n                    val = \"112\";\n                    break;\n                case \"Serial02\":\n                    val = \"114\";\n                    break;\n                case \"Serial03\":\n                    val = \"116\";\n                    break;\n            }\n\n            var CmdWriteEthernet_Serial_00 = val+\",000,\";            CmdWriteEthernet_Serial_00 += Val1 + \",\";\n            CmdWriteEthernet_Serial_00 += SerialConfig + \",\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"BaudRate\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"35,\";\n            CmdWriteEthernet_Serial_00 += \"36,\";\n            CmdWriteEthernet_Serial_00 += \"1,\";\n            CmdWriteEthernet_Serial_00 += \"1,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"Interface\").value + \",\";\n            CmdWriteEthernet_Serial_00 += Val2 + \",\";\n            CmdWriteEthernet_Serial_00 += Val3 + \",\";\n            CmdWriteEthernet_Serial_00 += Val4 + \",\";\n            CmdWriteEthernet_Serial_00 += Val5 + \",\";\n            CmdWriteEthernet_Serial_00 += Val6 + \",\";\n            CmdWriteEthernet_Serial_00 += Val7 + \",\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"UDPPort\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"503,\";\n            CmdWriteEthernet_Serial_00 += \"504,\";\n            CmdWriteEthernet_Serial_00 += \"505,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"PortID\").value + \",\";\n            CmdWriteEthernet_Serial_00 += \"0,\";\n            CmdWriteEthernet_Serial_00 += document.getElementById(\"RemoteIP\").value + \",\";\n            CmdWriteEthernet_Serial_00 += parseInt(Bits, 2);\n            document.getElementById(\"textarea_cmd\").value = CmdWriteEthernet_Serial_00;\n       ");
//     client.println("CommandStr = CmdWriteEthernet_Serial_00;document.getElementById(\"textarea_cmd\").value = CommandStr;SendRequest(CommandStr);}");
//     client.println("function ReadFromController() {\n var val = \"111\";\n            var serialport = document.getElementById(\"SerialPort\").value;\n            switch (serialport) {\n                case \"Serial00\":\n                    val = \"111\";\n                    break;\n                case \"Serial01\":\n                    val = \"113\";\n                    break;\n                case \"Serial02\":\n                    val = \"115\";\n                    break;\n                case \"Serial03\":\n                    val = \"117\";\n                    break;\n            }\n\n            CommandStr = val + \",000,000\";\n            const xhr = new XMLHttpRequest();\n            const url = \"http://192.168.1.110/api?message=\" + CommandStr;\n            xhr.open('GET', url, true);\n            xhr.onreadystatechange = function () {\n                if (xhr.readyState === 4 && xhr.status === 200) {\n                    const responseText = xhr.responseText;\n                    SetDataToFild(responseText);\n                }\n            };\n            xhr.send();\n            document.getElementById(\"textarea_cmd\").value = CommandStr;\n        }\n        ");
//     client.println("function ResetMCU() {\n            CommandStr = \"948,001,000\";\n            document.getElementById(\"textarea_cmd\").value = CommandStr;\n            SendRequest(CommandStr);\n        }\n        ");
//     client.println("function SendRequest(CommandStr) {\n            const xhr = new XMLHttpRequest();\n            const url = \"http://192.168.1.110/api?message=\" + CommandStr;\n            xhr.open('GET', url, true);\n            xhr.onreadystatechange = function () {\n                if (xhr.readyState === 4 && xhr.status === 200) {\n                    const responseText = xhr.responseText;\n                    console.log(responseText);\n                }\n            };\n            xhr.send();\n        }\n        ");
//     client.println("function calcSerialConfig(sdatabit, sparity, sstopbit) {\n            let databitLong = 0x1;\n            let parityLong = 0;\n            let stopbitLong = 0x400;\n\n            let parity = 1;\n            if (sparity === \"None\") parity = 3;\n            if (sparity === \"Odd\") parity = 2;\n\n            let databit = parseInt(sdatabit);\n            let stopbit = parseInt(sstopbit);\n            stopbit = stopbit * 10;\n\n            if (parity === 1) {\n                parityLong = 0x1;\n            } else if (parity === 2) {\n                parityLong = 0x2;\n            } else if (parity === 3) {\n                parityLong = 0x3;\n            } else if (parity === 4) {\n                parityLong = 0x4;\n            } else if (parity === 5) {\n                parityLong = 0x5;\n            } else if (parity === 6) {\n                parityLong = 0xF;\n            }\n\n            if (stopbit === 10) {\n                stopbitLong = 0x10;\n            } else if (stopbit === 15) {\n                stopbitLong = 0x20;\n            } else if (stopbit === 20) {\n                stopbitLong = 0x30;\n            } else {\n                stopbitLong = 0xF0;\n            }\n\n            if (databit === 5) {\n                databitLong = 0x100;\n            } else if (databit === 6) {\n                databitLong = 0x200;\n            } else if (databit === 7) {\n                databitLong = 0x300;\n            } else if (databit === 8) {\n                databitLong = 0x400;\n            } else {\n                databitLong = 0xF00;\n            }\n\n            let config = databitLong | parityLong | stopbitLong;\n            return config;\n        }\n        ");
//     client.println("function SetSerialConfig() {\n            SerialConfig = calcSerialConfig(\n                parseInt(document.getElementById(\"DataBit\").value),\n                document.getElementById(\"Parity\").value,\n                parseInt(document.getElementById(\"StopBit\").value)\n            );\n        }\n        const selectElement1 = document.getElementById('DataBit');\n        selectElement1.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n        const selectElement2 = document.getElementById('Parity');\n        selectElement2.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n        const selectElement3 = document.getElementById('StopBit');\n        selectElement3.addEventListener('change', function () {\n            SetSerialConfig();\n        });\n\n        function ConvertDecimalToIP(decimalValue) {\n            let decimalNumber = parseInt(decimalValue);\n            let binaryString = decimalNumber.toString(2).padStart(32, '0');\n\n            let ip = new Array(4);\n            ip[3] = parseInt(binaryString.substring(24, 32), 2);\n            ip[2] = parseInt(binaryString.substring(16, 24), 2);\n            ip[1] = parseInt(binaryString.substring(8, 16), 2);\n            ip[0] = parseInt(binaryString.substring(0, 8), 2);\n\n            return ip;\n        }\n        function ConvertDecimalToMac(decimalValue) {\n            let decimalNumber = parseInt(decimalValue);\n            let binaryString = decimalNumber.toString(2).padStart(32, '0');\n\n            let macBytes = new Uint8Array(4);\n            for (let i = 0; i < 4; i++) {\n                macBytes[i] = parseInt(binaryString.substring(i * 8, (i + 1) * 8), 2);\n            }\n\n            return [macBytes[3], macBytes[2], macBytes[1], macBytes[0]];\n        }\n        ");
//     client.println("function ConvertDecimalToBool(value) {\n            let binary = value.toString(2).padStart(16, '0');\n            return binary;\n        }\n        ");
//     client.println("function ConvertStringToBool(value) {\n            return value === \"1\";\n        }\n        ");
//     client.println("function SetDataToFild(CommResponse) {\n            var values = (CommResponse.replace(/<[^>]*>/g, '')).split(\",\");\n\n            let Temp = ConvertDecimalToIP(values[0]);\n            document.getElementById(\"StopBit\").value = Temp[3];\n            document.getElementById(\"DataBit\").value = Temp[2];\n\n            let parity = \"Even\";\n            if (Temp[1] === 3) parity = \"None\";\n            if (Temp[1] === 2) parity = \"Odd\";\n            document.getElementById(\"Parity\").value = parity;\n\n            SerialConfig = parseInt(values[1]);\n\n            document.getElementById(\"BaudRate\").value = parseInt(values[2]);\n            document.getElementById(\"Interface\").value = parseInt(values[7]);\n\n            let macBytes = ConvertDecimalToMac(values[8]);\n            document.getElementById(\"MacAddress\").value = macBytes[0] + \":\" + macBytes[1] + \":\" + macBytes[2] + \":\" + macBytes[3];\n\n            let ipAddress = ConvertDecimalToIP(values[9]);\n            document.getElementById(\"IPAddress\").value = ipAddress[3] + \".\" + ipAddress[2] + \".\" + ipAddress[1] + \".\" + ipAddress[0];\n\n            let subnetMask = ConvertDecimalToIP(values[10]);\n            document.getElementById(\"NetMask\").value = subnetMask[3] + \".\" + subnetMask[2] + \".\" + subnetMask[1] + \".\" + subnetMask[0];\n\n            let gateway = ConvertDecimalToIP(values[11]);\n            document.getElementById(\"Gateway\").value = gateway[3] + \".\" + gateway[2] + \".\" + gateway[1] + \".\" + gateway[0];\n\n            let dns1 = ConvertDecimalToIP(values[12]);\n            document.getElementById(\"PreferredDNS\").value = dns1[3] + \".\" + dns1[2] + \".\" + dns1[1] + \".\" + dns1[0];\n\n            let dns2 = ConvertDecimalToIP(values[13]);\n            document.getElementById(\"AlternateDNS\").value = dns2[3] + \".\" + dns2[2] + \".\" + dns2[1] + \".\" + dns2[0];\n\n            document.getElementById(\"UDPPort\").value = values[14];\n              document.getElementById(\"UDPPort\").value = parseInt(values[14]);\n            ");
//     client.println("document.getElementById(\"UDPPort2\").value = parseInt(values[15]);\n            document.getElementById(\"UDPPort3\").value = parseInt(values[16]);\n            document.getElementById(\"UDPPort4\").value = parseInt(values[17]);\n            document.getElementById(\"RemoteIP\").value = values[20];\n");
//     client.println("let bitsString = ConvertDecimalToBool(parseInt(values[21]));\nlet Enabled_Ethernet = document.getElementById(\"Enabled_Ethernet\");\n            if (ConvertStringToBool(bitsString.substring(0, 1)) == \"1\") { Enabled_Ethernet.checked = true; } else { Enabled_Ethernet.checked = false; }\n            let Enabled_Serial = document.getElementById(\"Enabled_Ethernet\");\n            if (ConvertStringToBool(bitsString.substring(1, 2)) == \"1\") { Enabled_Serial.checked = true; } else { Enabled_Serial.checked = false; }\n\n            var Operation_Mode = \"SerialServer\";\n            if (ConvertStringToBool(bitsString.substring(2, 3)) == \"1\") { Operation_Mode = \"SerialServer\"; }\n            if (ConvertStringToBool(bitsString.substring(3, 4)) == \"1\") { Operation_Mode = \"PeerToPeer\"; }\n\n            let radioButtons = document.querySelectorAll(`input[type=\"radio\"][name=\"${Operation_Mode}\"]`);\n\n            radioButtons.forEach(function (radioButton) {\n                radioButton.checked = true;\n            });\n        }\n</script>");
// }

void ViewSettingsPage(PmmEthernetClient client)
{
    client.println("<title>PMM-Easy Config</title><style>");
    client.println(".footer{position:fixed;bottom:5px;color:var(--contentTextColor);text-align:center;font-size:13px;padding:2px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.footer p{display:contents}body{font-family:Arial,sans-serif;margin:0;padding:0;overflow-x:hidden}#navbar{background-color:#339898;color:#fff;padding:10px;height:44px;width:100%}#leftbar{background-color:#339898;color:#fff;width:190px;padding:10px;position:absolute;top:0;left:0;height:100%;margin-top:64px}#content{margin-left:210px;padding:20px}.content-div{background-color:#eee;padding:10px;margin-bottom:10px;height:400px;display:none}span{color:#fff}ul{list-style:none}li{padding:12px;width:92%}#div1,#div2,#div3{display:none}");
    client.println(".button{text-align:center;text-decoration:none;display:inline-block;cursor:pointer;width:40px;height:40px}.button2{text-align:center;text-decoration:none;display:inline-block;cursor:pointer;width:80px;height:40px}textarea{width:89%;height:40px;padding:8px;box-sizing:border-box;border:2px solid #ccc;border-radius:4px;font-size:12px;resize:none}.ddl{width:192px;height:28px;box-sizing:border-box;border:2px solid #ccc}.input{width:192px;height:28px;box-sizing:border-box;border:2px solid #ccc}.input.udpport{width:45px;position:static}.button3{width:140px;height:28px;position:absolute}.container{display:block;position:relative;cursor:pointer;font-size:16px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.container input{position:absolute;opacity:0;cursor:pointer;height:0;width:0}");
    client.println(".checkmark{position:absolute;top:-6px;left:307px;height:25px;width:25px;background-color:#339898;border:solid #339898}.container:hover input~.checkmark{background-color:#339898}.container input:checked~.checkmark{background-color:#339898}.checkmark:after{content:'';position:absolute;display:none}.container input:checked~.checkmark:after{display:block}.container .checkmark:after{left:9px;top:5px;width:5px;height:10px;border:solid #fff;border-width:0 3px 3px 0;-webkit-transform:rotate(45deg);-ms-trasform:rotate(45deg);transform:rotate(45deg)}.Divs.active{background-color:#fff;color:#339898}#LeftBarContent{display:none}.Group_Item{display:block;margin-bottom:16px}.Group_Item_lbl{width:300px;display:inline-block}.Group_Item_input{width:300px;display:inline-block}</style></head>");
    client.println("<body><div id='navbar'><h1 style='position: absolute;top: -6px;left: 26px;'>PMM</h1><h4 style='position: absolute;top: 7px;left: 112px;'>Easy Config</h4></div>");
    client.println("<div id='leftbar'><ul style='margin-left: -10%;' id='LeftBarContent'><li class='Divs' onclick=\"GoToDiv('div1');\" id='div1_btn'><span id='div1_span' style='font-size: 14px;'>Ethernet</span></li><li class='Divs' onclick=\"GoToDiv('div2');\" id='div2_btn'><span id='div2_span' style='font-size: 14px;'>Serials</span></li><li class='Divs' onclick=\"GoToDiv('div3');\" id='div3_btn'><span id='div3_span' style='font-size: 14px;'>Operation Mode</span></li><button class='button3' onclick=\'ReadFromController();\' style='margin-top: 10%;'>Upload</button><br><br><button class='button3' onclick='WriteToController();' style='margin-top: 10%;'>Download</button><br><br><button class='button3' onclick='ResetMCU();' style='margin-top: 10%;'>Reset MCU</button><br><br></ul><br><br>");
    client.println("<ul style='margin-left: -10%;' id=''><input style='width: 140px;left: 15%;' type='Password' class='input' id='Password_txt' name='Password' value='1976'><br><button class='button3' onclick=\'Login();\' style='margin-top: 5%;' id='Login_btn'>Login</button><button class='button3' onclick=\'Logout();\' style='margin-top: 5%;display: none;' id='Logout_btn'>Logout</button><br><br></ul><div class='footer'><p>&#169;2014-</p><span id='thisYear'></span><br><p>PMM LLC. all rights reserved.</p><br><a style='text-decoration: none;color: #fff;' href='https://pmm-usa.us/'>www.pmm-usa.us</a></div></div>");
    client.println("<div id='content'><div class='content-div' id='div1' style='display: inline;position: absolute;width: 86%;'><h2>Ethernet</h2><div class='Group_Item'><label class='container'>Enabled <input type='checkbox' id='Enabled_Ethernet'><span class='checkmark'></span></label></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='IPAddress'>IP Address</label></div><div class='Group_Item_input'><input type='text' class='input' id='IPAddress' name='IPAddress' onchange=\'GetIPAddress();\' value='192.168.1.100'></div></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='NetMask'>Net Mask</label></div><div class='Group_Item_input'><input type='text' class='input' id='NetMask' name='NetMask' value='255.255.255.0'></div></div>");
    client.println("<div class='Group_Item'><div class='Group_Item_lbl'><label for='PreferredDNS'>Preferred DNS</label></div><input type='text' class='input' id='PreferredDNS' name='PreferredDNS' value='8.8.8.8'></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='AlternateDNS'>Alternate DNS</label></div><div class='Group_Item_input'><input type='text' class='input' id='AlternateDNS' name='AlternateDNS' value='8.8.4.4'></div></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='Gateway'>Gateway</label></div><div class='Group_Item_input'><input type='text' class='input' id='Gateway' name='Gateway' value='192.168.1.1'></div></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='MacAddress'>Mac Address</label></div><div class='Group_Item_input'><label for='MacAddress' style='left: 24%;top: 40%;'>48:7E</label><input type='text' class='input' id='MacAddress' name='MacAddress' value='72:64:64:32' style='width: 122px;margin-left: 22px;'></div></div>");
    client.println("<div class='Group_Item'><div class='Group_Item_lbl'><label for='UDPPort' style='margin-right: 142px;'>UDP Port</label></div><div class='Group_Item_input'><input type='number' class='input udpport' id='UDPPort' name='UDPPort' value='502'><input type='number' class='input udpport' id='UDPPort2' name='UDPPort2' value='503'><input type='number' class='input udpport' id='UDPPort3' name='UDPPort3' value='504'><input type='number' class='input udpport' id='UDPPort4' name='UDPPort4' value='505'></div></div></div>");
    client.println("<div class='content-div' id='div2' style='display: inline;position: absolute;width: 86%;'><h2>Serials</h2><div class='Group_Item'><label class='container'>Enabled <input type='checkbox' id='Enabled_Serial'><span class='checkmark'></span></label></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='SerialPort'>Serial Port</label></div><div class='Group_Item_input'><select id='SerialPort' class='ddl'><option value='Serial00'>Serial 00</option><option value='Serial01'>Serial 01</option><option value='Serial02'>Serial 02</option><option value='Serial03'>Serial 03</option></select></div></div>");
    client.println("<div class='Group_Item'><div class='Group_Item_lbl'><label for='BaudRate'>Baud Rate</label></div><div class='Group_Item_input'><select id='BaudRate' class='ddl'><option value='300'>300</option><option value='600'>600</option><option value='1200'>1200</option><option value='2400'>2400</option><option value='4800'>4800</option><option value='9600'>9600</option><option value='14400'>14400</option><option value='19200'>19200</option><option value='38400'>38400</option><option value='56000'>56000</option><option value='57600'>57600</option><option value='115200'>115200</option></select></div></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='DataBit'>Data Bit</label></div><div class='Group_Item_input'><select id='DataBit' class='ddl'><option value='5'>5</option><option value='6'>6</option><option value='7'>7</option><option value='8'>8</option></select></div></div>");
    client.println("<div class='Group_Item'><div class='Group_Item_lbl'><label for='StopBit'>Stop Bit</label></div><div class='Group_Item_input'><select id='StopBit' class='ddl'><option value='1'>1</option><option value='2'>2</option></select></div></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='Parity'>Parity</label></div><div class='Group_Item_input'><select id='Parity' class='ddl'><option value='None'>None</option><option value='Odd'>Odd</option><option value='Even'>Even</option></select></div></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='Interface'>Interface</label></div><div class='Group_Item_input'><select id='Interface' class='ddl'><option value='485'>RS485</option><option value='422'>RS422</option><option value='232'>RS232</option></select></div></div><div class='Group_Item'><div class='Group_Item_lbl'><label for='PortID'>Port ID</label></div><div class='Group_Item_input'><input type='number' class='input' id='PortID' name='PortID' style='width:100px;' value='1'><label style='margin-left: 40px;'>(1-30)</label></div></div></div>");
    client.println("<div class='content-div' id='div3' style='display: inline;position: absolute;width: 86%;'><h2>Operation Mode</h2><div class='Group_Item'><input type='radio' id='SerialServer' name='Operation_Mode' value='SerialServer' checked><label for='SerialServer'>Serial Server</label><br><p style='margin-left: 3%;'>multi ethernet client can communicate with serial port on this device</p></div><div class='Group_Item'><input type='radio' id='PeerToPeer' name='Operation_Mode' value='PeerToPeer'><label for='PeerToPeer'>Peer To Peer</label><br><label for='RemoteIP' style='top: 6px;position: relative;margin-left: 42px;'>Remote IP</label><label style='top: 6px;position: relative;margin-left: 7%;' id='IpAddress_txt'>192.168.1.</label><input type='number' min='1' class='input' id='RemoteIP' name='RemoteIP' style='width:50px;margin-left: 0px;' value='200'></div></div></div>");
    client.println("<script>var d=new Date,y=d.getFullYear();document.getElementById('thisYear').textContent=y;let CommandStr='',SerialConfig=1043;");
    client.println("function CheckLogin(){'true'==localStorage.getItem('Login')?(document.getElementById('Password_txt').style.display='none',document.getElementById('Login_btn').style.display='none',document.getElementById('Logout_btn').style.display='block',document.getElementById('LeftBarContent').style.display='block',GoToDiv('div1'),ReadFromController()):'false'==localStorage.getItem('Login')&&(document.getElementById('Password_txt').style.display='block',document.getElementById('div1').style.display = 'none',document.getElementById('div2').style.display = 'none',document.getElementById('div3').style.display = 'none',document.getElementById('Login_btn').style.display='block',document.getElementById('Logout_btn').style.display='none',document.getElementById('LeftBarContent').style.display='none')}");
    client.println("function Login(){var e=document.getElementById('Password_txt').value,t='';let l=new XMLHttpRequest,n='http://'+document.getElementById('IPAddress').value+'/api?message=961,000,000';l.open('GET',n,!0),l.onreadystatechange=function(){if(4===l.readyState&&200===l.status){let e=l.responseText;t=e}},l.send(),'1976'==e||e==t?(document.getElementById('Password_txt').style.display='none',document.getElementById('Login_btn').style.display='none',document.getElementById('Logout_btn').style.display='block',document.getElementById('LeftBarContent').style.display='block',GoToDiv('div1'),ReadFromController(),localStorage.setItem('Login','true')):(document.getElementById('Password_txt').style.display='block',document.getElementById('Login_btn').style.display='block',document.getElementById('Logout_btn').style.display='none',document.getElementById('LeftBarContent').style.display='none',localStorage.setItem('Login','false'))}");
    client.println("function Logout(){document.getElementById('Password_txt').style.display='block',document.getElementById('Login_btn').style.display='block',document.getElementById('Logout_btn').style.display='none',document.getElementById('LeftBarContent').style.display='none',document.getElementById('div1').style.display='none',document.getElementById('div2').style.display='none',document.getElementById('div3').style.display='none',localStorage.setItem('Login','false')}");
    client.println("function GetIPAddress(){var e=document.getElementById('IPAddress').value.split('.'),t=e[0]+'.'+e[1]+'.'+e[2]+'.';document.getElementById('IpAddress_txt').textContent=t}");
    client.println("function GoToDiv(e){'div1'==e?(document.getElementById('div1').style.display='block',document.getElementById('div2').style.display='none',document.getElementById('div3').style.display='none',document.getElementById('div1_btn').classList.add('active'),document.getElementById('div2_btn').classList.remove('active'),document.getElementById('div3_btn').classList.remove('active'),document.getElementById('div1_span').style.color='#339898',document.getElementById('div2_span').style.color='#ffffff',document.getElementById('div3_span').style.color='#ffffff'):'div2'==e?(document.getElementById('div1').style.display='none',document.getElementById('div2').style.display='block',document.getElementById('div3').style.display='none',document.getElementById('div1_btn').classList.remove('active'),document.getElementById('div2_btn').classList.add('active'),document.getElementById('div3_btn').classList.remove('active'),document.getElementById('div1_span').style.color='#ffffff',document.getElementById('div2_span').style.color='#339898',document.getElementById('div3_span').style.color='#ffffff'):'div3'==e&&(document.getElementById('div1').style.display='none',document.getElementById('div2').style.display='none',document.getElementById('div3').style.display='block',document.getElementById('div1_btn').classList.remove('active'),document.getElementById('div2_btn').classList.remove('active'),document.getElementById('div3_btn').classList.add('active'),document.getElementById('div1_span').style.color='#ffffff',document.getElementById('div2_span').style.color='#ffffff',document.getElementById('div3_span').style.color='#339898')}");
    client.println("function convertIPToDecimal(e,t,l,n){return parseInt(n.toString(2).padStart(8,'0')+l.toString(2).padStart(8,'0')+t.toString(2).padStart(8,'0')+e.toString(2).padStart(8,'0'),2).toString()}");
    client.println("function convertMacToDecimal(e,t,l,n){for(var o=[e,t,l,n],a='',r=3;r>=0;r--)a+=o[r].toString(2).padStart(8,'0');return parseInt(a,2).toString()}");
    client.println("function numberToByte(e){return e>=0&&e<=255?255&e:null}function convertBoolToString(e){var t='0';return null!=e&&('false'===(e=e.toString().toLowerCase())||'0'===e?t='0':('true'===e||'1'===e)&&(t='1')),t}");
    client.println("function WriteToController(){var e=document.getElementById('Parity').value;let t=1;'None'===e&&(t=3),'Odd'===e&&(t=2);var l=convertIPToDecimal(parseInt(document.getElementById('StopBit').value),parseInt(document.getElementById('DataBit').value),parseInt(t),1),n=convertMacToDecimal(numberToByte(document.getElementById('MacAddress').value.split(':')[0]),numberToByte(document.getElementById('MacAddress').value.split(':')[1]),numberToByte(document.getElementById('MacAddress').value.split(':')[2]),numberToByte(document.getElementById('MacAddress').value.split(':')[3])),o=convertIPToDecimal(parseInt(document.getElementById('IPAddress').value.split('.')[0]),parseInt(document.getElementById('IPAddress').value.split('.')[1]),parseInt(document.getElementById('IPAddress').value.split('.')[2]),parseInt(document.getElementById('IPAddress').value.split('.')[3])),a=convertIPToDecimal(parseInt(document.getElementById('NetMask').value.split('.')[0]),parseInt(document.getElementById('NetMask').value.split('.')[1]),parseInt(document.getElementById('NetMask').value.split('.')[2]),parseInt(document.getElementById('NetMask').value.split('.')[3])),r=convertIPToDecimal(parseInt(document.getElementById('Gateway').value.split('.')[0]),parseInt(document.getElementById('Gateway').value.split('.')[1]),parseInt(document.getElementById('Gateway').value.split('.')[2]),parseInt(document.getElementById('Gateway').value.split('.')[3])),i=convertIPToDecimal(parseInt(document.getElementById('PreferredDNS').value.split('.')[0]),parseInt(document.getElementById('PreferredDNS').value.split('.')[1]),parseInt(document.getElementById('PreferredDNS').value.split('.')[2]),parseInt(document.getElementById('PreferredDNS').value.split('.')[3])),s=convertIPToDecimal(parseInt(document.getElementById('AlternateDNS').value.split('.')[0]),parseInt(document.getElementById('AlternateDNS').value.split('.')[1]),parseInt(document.getElementById('AlternateDNS').value.split('.')[2]),parseInt(document.getElementById('AlternateDNS').value.split('.')[3]));SerialConfig=calcSerialConfig(parseInt(document.getElementById('StopBit').value),document.getElementById('Parity').value,parseInt(document.getElementById('StopBit').value));");
    client.println("var g='0',m='0',v=document.querySelector(\"input[name='Operation_Mode']:checked\").value;'PeerToPeer'==v&&(g='1'),'SerialServer'==v&&(m='1');var c,B='0',I='0',E=document.getElementById('Enabled_Ethernet').checked;E&&(I='1'),document.getElementById('Enabled_Serial').checked&&(B='1');var u=convertBoolToString('0');u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString('0'),u+=convertBoolToString(g),u+=convertBoolToString(m),u+=convertBoolToString(B),u+=convertBoolToString(I);var $='111',S=document.getElementById('SerialPort').value;switch(S){case'Serial00':$='110';break;case'Serial01':$='112';break;case'Serial02':$='114';break;case'Serial03':$='116'}");
    client.println("var _=$+',000,';_+=l+',',_+=SerialConfig+',',_+=document.getElementById('BaudRate').value+',',_+='35,',_+='36,',_+='1,',_+='1,',_+=document.getElementById('Interface').value+',',_+=n+',',_+=o+',',_+=a+',',_+=r+',',_+=i+',',_+=s+',',_+=document.getElementById('UDPPort').value+',',_+=document.getElementById('UDPPort2').value+',',_+=document.getElementById('UDPPort3').value+',',_+=document.getElementById('UDPPort4').value+',',_+=document.getElementById('PortID').value+',',_+='0,',_+=document.getElementById('RemoteIP').value+',',_+=parseInt(u,2),SendRequest(CommandStr=_)}");
    client.println("function ReadFromController(){var e='111',t=document.getElementById('SerialPort').value;switch(t){case'Serial00':e='111';break;case'Serial01':e='113';break;case'Serial02':e='115';break;case'Serial03':e='117'}CommandStr=e+',000,000';let l=new XMLHttpRequest,n='http://'+document.getElementById('IPAddress').value+'/api?message='+CommandStr;l.open('GET',n,!0),l.onreadystatechange=function(){if(4===l.readyState&&200===l.status){let e=l.responseText;SetDataToFild(e)}},l.send()}");
    client.println("function ResetMCU(){SendRequest(CommandStr='948,001,000')}function SendRequest(e){let t=new XMLHttpRequest,l='http://'+document.getElementById('IPAddress').value+'/api?message='+e;t.open('GET',l,!0),t.onreadystatechange=function(){if(4===t.readyState&&200===t.status){let e=t.responseText;console.log(e)}},t.send()}");
    client.println("function calcSerialConfig(e,t,l){let n=1,o=0,a=1024,r=1;'None'===t&&(r=3),'Odd'===t&&(r=2);let i=parseInt(e),s=parseInt(l);return s*=10,1===r?o=1:2===r?o=2:3===r?o=3:4===r?o=4:5===r?o=5:6===r&&(o=15),a=10===s?16:15===s?32:20===s?48:240,(n=5===i?256:6===i?512:7===i?768:8===i?1024:3840)|o|a}");
    client.println("function SetSerialConfig(){SerialConfig=calcSerialConfig(parseInt(document.getElementById('DataBit').value),document.getElementById('Parity').value,parseInt(document.getElementById('StopBit').value))}CheckLogin();const selectElement1=document.getElementById('DataBit');selectElement1.addEventListener('change',function(){SetSerialConfig()});const selectElement2=document.getElementById('Parity');selectElement2.addEventListener('change',function(){SetSerialConfig()});const selectElement3=document.getElementById('StopBit');");
    client.println("function ConvertDecimalToIP(e){let t=parseInt(e).toString(2).padStart(32,'0'),l=[,,,,];return l[3]=parseInt(t.substring(24,32),2),l[2]=parseInt(t.substring(16,24),2),l[1]=parseInt(t.substring(8,16),2),l[0]=parseInt(t.substring(0,8),2),l}");
    client.println("function ConvertDecimalToMac(e){let t=BigInt(e).toString(2).padStart(32,'0'),l=new Uint8Array(4);for(let n=0;n<4;n++)l[n]=parseInt(t.substring(8*n,(n+1)*8),2);return[l[3],l[2],l[1],l[0]]}");
    client.println("function ConvertDecimalToBool(e){return e.toString(2).padStart(16,'0')}function ConvertStringToBool(e){return'1'===e}");
    client.println("function SetDataToFild(e){var t=e.split(',');let l=ConvertDecimalToIP(t[0]);document.getElementById('StopBit').value=l[3],document.getElementById('DataBit').value=l[2];let n='Even';3===l[1]&&(n='None'),2===l[1]&&(n='Odd'),document.getElementById('Parity').value=n,SerialConfig=parseInt(t[1]),document.getElementById('BaudRate').value=parseInt(t[2]),document.getElementById('Interface').value=parseInt(t[7]);let o=ConvertDecimalToMac(t[8]);document.getElementById('MacAddress').value=o[0]+':'+o[1]+':'+o[2]+':'+o[3];let a=ConvertDecimalToIP(t[9]);document.getElementById('IPAddress').value=a[3]+'.'+a[2]+'.'+a[1]+'.'+a[0];let r=ConvertDecimalToIP(t[10]);document.getElementById('NetMask').value=r[3]+'.'+r[2]+'.'+r[1]+'.'+r[0];let i=ConvertDecimalToIP(t[11]);document.getElementById('Gateway').value=i[3]+'.'+i[2]+'.'+i[1]+'.'+i[0];let s=ConvertDecimalToIP(t[12]);document.getElementById('PreferredDNS').value=s[3]+'.'+s[2]+'.'+s[1]+'.'+s[0];let g=ConvertDecimalToIP(t[13]);document.getElementById('AlternateDNS').value=g[3]+'.'+g[2]+'.'+g[1]+'.'+g[0],document.getElementById('UDPPort').value=parseInt(t[14]),document.getElementById('UDPPort2').value=parseInt(t[15]),document.getElementById('UDPPort3').value=parseInt(t[16]),document.getElementById('UDPPort4').value=parseInt(t[17]),document.getElementById('PortID').value=parseInt(t[18]),document.getElementById('RemoteIP').value=parseInt(t[20]);let m=ConvertDecimalToBool(parseInt(t[21])),v=document.getElementById('Enabled_Ethernet');'1'==ConvertStringToBool(m.substring(0,1))?v.checked=!0:v.checked=!1;let c=document.getElementById('Enabled_Ethernet');'1'==ConvertStringToBool(m.substring(1,2))?c.checked=!0:c.checked=!1;var B='SerialServer';'1'==ConvertStringToBool(m.substring(2,3))&&(B='SerialServer'),'1'==ConvertStringToBool(m.substring(3,4))&&(B='PeerToPeer');document.querySelectorAll(`input[type='radio'][name='${B}']`).forEach(function(e){e.checked=!0})}selectElement3.addEventListener('change',function(){SetSerialConfig()});</script>");
}

#endif

#endif
