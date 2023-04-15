#include "PmmCommands.h"
#include "PmmConfigrature.h"
#include <PmmEthernet.h>

PmmEthernetServer server(80);
PmmEthernetClient client = server.available();

void PMMInitializeEthernet(IPAddress ip, byte mac[]);
string PMMReadCommands();
string PMMCommnads(string readData);
string PMMReturnDataFromSerialUSB();
string PMMReturnDataFromAPIHTTPHeader();
void PMMSetUSBConfigurationSettings(string Settings);
string PMMGetUSBConfigurationSettings();
void PMMIsAlive();

void PMMInitializeEthernet(IPAddress ip, byte mac[])
{
    Ethernet.init(10);
    Ethernet.begin(mac, ip);
    server.begin();
}

string PMMReadCommands()
{
    string result = "";
    if (SerialUSB.available())
    {
        string cmd = PMMReturnDataFromSerialUSB();

        if (!cmd.empty())
        {
            result = PMMCommnads(cmd);
        }
    }

    if (client)
    {
        result = PMMCommnads(PMMReturnDataFromAPIHTTPHeader());
    }

    return result;
}

string PMMCommnads(string readData)
{
    string result = "";
    std::string commandtype = readData.substr(0, 10);

    // PMMSet,0,0,0620,5000,0,1000,55,200,0,0,1,0,3891,1,2,3,4,5,6,100,200,300,400,500,600
    // PMMSet,0,1,1,9600,1,8,0,5000,3,485,2,9600,2,7,1,6000,4,322,3,9600,1,8,2,7000,5,485,4,9600,2,7,1,7000,5,322
    // PMMSet,0,2,171,205,173,205,171,192,186,1,100,192,168,1,200,255,255,255,255,8,8,8,8,8,8,0,0,5000,3,90,91,92,93
    // PMMSet,0,3,4384,1,1001,2001,3001,1,1001,2001,3001,32,64,128,256,100,100,03,1,1,1000,1,1,100
    // PMMSet,0,4,195,
    // PMMGet,0,0
    // PMMGet,0,1
    // PMMGet,0,2
    // PMMGet,0,3
    // PMMGet,0,4

    if (commandtype == "PMMSet,0,0")
    {
        string substring = "PMMSet,0,0,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteGeneralSettingsInternalFlash(readData);
    }

    if (commandtype == "PMMSet,0,1")
    {
        string substring = "PMMSet,0,1,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteRTUSettingsInternalFlash(readData);
    }

    if (commandtype == "PMMSet,0,2")
    {
        string substring = "PMMSet,0,2,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteTCPUDPSettingsInternalFlash(readData);
    }

    if (commandtype == "PMMSet,0,3")
    {
        string substring = "PMMSet,0,3,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteModbusSettingsInternalFlash(readData);
    }

    if (commandtype == "PMMSet,0,4")
    {
        string substring = "PMMSet,0,4,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        PmmWriteTimerSettingsInternalFlash(readData);
    }

    if (commandtype == "PMMGet,0,0") // GET General TO STRING
    {
        PmmReadGeneralSettingsInternalFlash();
    }

    if (commandtype == "PMMGet,0,1") // GET RTU TO STRING
    {
        PmmReadRTUSettingsInternalFlash();
    }

    if (commandtype == "PMMGet,0,2") // GET TCP TO STRING
    {
        PmmReadTCPUDPSettingsInternalFlash();
    }

    if (commandtype == "PMMGet,0,3") // GET Modbus TO STRING
    {
        PmmReadModbusSettingsInternalFlash();
    }

    if (commandtype == "PMMGet,0,4") // GET Timers TO STRING
    {
        PmmReadTimersSettingsInternalFlash();
    }

    if (readData == "PMMTestConfiguration")
    {
        PMMIsAlive();
    }

    return result;
}

string PMMReturnDataFromSerialUSB()
{
    String Command = "";
    Command = SerialUSB.readStringUntil('\n');
    return Command.c_str();
}

string PMMReturnDataFromAPIHTTPHeader()
{
    String APIData = "";
    String readString = " ";

    while (client.connected())
    {
        if (client.available())
        {
            char c = client.read();

            if (readString.length() < 120)
            {
                readString += c;
            }
            if (c == '\n')
            {
                APIData = readString.substring(readString.indexOf("api?message=") + 12, readString.indexOf("HTTP/1.1"));
                client.stop();
            }
        }
    }

    return APIData.c_str();
}

void PMMSendDataHTTPClient(String Data)
{
    PmmEthernetClient client = server.available();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");

    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.print(Data);
    client.println("</head>");
    client.println("</html>");
    client.stop();
}