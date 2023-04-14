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
string PMMIsAlive();

void PMMInitializeEthernet(IPAddress ip, byte mac[])
{
    Ethernet.init(10);
    Ethernet.begin(mac, ip);
    server.begin();
}

string PMMReadCommands()
{
    string result = "";
    // if (SerialUSB.available() > 0)
    // {

    string cmd = PMMReturnDataFromSerialUSB();

    if (!cmd.empty())
    {
        // SerialUSB.println(cmd.c_str());
        result = PMMCommnads(cmd);
    }

    //}

    if (client)
    {
        result = PMMCommnads(PMMReturnDataFromAPIHTTPHeader());
    }

    return result;
}

string PMMCommnads(string readData)
{
    string result = "";

    String result2 = "";

    std::string commandtype = readData.substr(0, 10);
    SerialUSB.println(commandtype.c_str());
    //  PMMSet,0,0,1,0,0,0,1,0,0,0,1,0,0,0,20,5,1,1,1,1,0,1,9600,8,1,2,422,3000,10,127,0,0,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,171,205,173,205,171,205,3000,10,502,503,504,505,1,0,0,0,1,0,1,1,1,0,0,0,0,0,0,1,2,0,2,3,1,2,3,0,2,3,1,2,3,10,0,0,0,0,6,0,1,0,0,3,0,0,0,7,0,5,3,0,6,10,0,0,0,0,0,0,0,0,0,

    // PMMSet,0,0,05711503,503,0,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,171,205,173,205,171,205,3000,10,502,503,504,711,
    // PMMGet,0,0

    // PMMSet,0,3,127,0,0,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,171,205,173,205,171,205,3000,10,502,503,504,505,
    //  PMMGet,0,3

    // PMMSet,0,0,0620,5000,0,1000,55,200,0,0,1,0,3891,1,2,3,4,5,6,100,200,300,400,500,600,
    //PMMSet,0,1,1,9600,1,8,0,5000,3,485,2,9600,2,7,1,6000,4,322,3,9600,1,8,2,7000,5,485,4,9600,2,7,1,7000,5,322


    if (commandtype == "PMMSet,0,0")
    {
        // if (SerialUSB.available() > 0)
        // {

        string substring = "PMMSet,0,0,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        // SerialUSB.println(readData.c_str());

        PmmWriteGeneralSettings(readData);

        //}

        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            // PMMWriteIntoFlashGeneralSettings(settings);
            result = "Done";
        }
    }

    if (commandtype == "PMMSet,0,1")
    {
        string substring = "PMMSet,0,1,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        // SerialUSB.println(readData.c_str());
        // SetTCPSettings(readData);

        PmmWriteRTUSettings(readData);
        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            // PMMWriteIntoFlashGeneralSettings(settings);
            result = "Done";
        }
    }

    if (commandtype == "PMMGet,0,3") // GET tcp TO STRING
    {
        // result2 = GetTCPSettings();
    }

    else if (commandtype == "PMMGet,0,0") // GET General TO STRING
    {
        PmmReadGeneralSettings();
    }

    else if (commandtype == "PMMGet,0,1") // GET RTU TO STRING
    {
        PmmReadRTUSettings();
    }

    // else if (commandtype == "PMMGet,0,0")
    // {

    //     PMMReadFromFlashAllSettings();
    // }

    else if (readData == "PMMTestConfiguration")
    {
        result = PMMIsAlive();
    }

    return result;
}

string PMMReturnDataFromSerialUSB()
{
    String Command = "";
    // if (SerialUSB.available() > 0)
    // {
    // SerialUSB.setTimeout(200);
    Command = SerialUSB.readStringUntil('\n');
    //}

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