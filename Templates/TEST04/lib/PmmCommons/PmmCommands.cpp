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
    //SerialUSB.println(commandtype.c_str());
    // PMMSet,0,0,1,0,0,0,1,0,0,0,1,0,0,0,20,5,1,1,1,1,0,1,9600,8,1,2,422,3000,10,127,0,0,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,171,205,173,205,171,205,3000,10,502,503,504,505,1,0,0,0,1,0,1,1,1,0,0,0,0,0,0,1,2,0,2,3,1,2,3,0,2,3,1,2,3,10,0,0,0,0,6,0,1,0,0,3,0,0,0,7,0,5,3,0,6,10,0,0,0,0,0,0,0,0,0,

    //PMMSet,0,0,127,0,0,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,171,205,173,205,171,205,3000,10,502,503,504,711,
    //PMMGet,0,0

    //PMMSet,0,3,127,0,0,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,171,205,173,205,171,205,3000,10,502,503,504,505,
    // PMMGet,0,3

    if (commandtype == "PMMSet,0,0")
    {
        // if (SerialUSB.available() > 0)
        // {
        // string settings = PMMReturnDataFromSerialUSB();
        // PMMWriteIntoFlashGeneralSettings(settings);
        // result = "Done";

        string substring = "PMMSet,0,0,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        //SerialUSB.println(readData.c_str());

        //PMMWriteIntoFlashAllSettings(readData);
        SetProductSettings(readData);
        
        //result = "Done";
        //}

        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            PMMWriteIntoFlashGeneralSettings(settings);
            result = "Done";
        }
    }

    if (commandtype == "PMMSet,0,3")
    {
        string substring = "PMMSet,0,3,";
        std::size_t ind = readData.find(substring);
        readData.erase(ind, substring.length());
        //SerialUSB.println(readData.c_str());
        SetTCPSettings(readData);
        result = "Done";

        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            PMMWriteIntoFlashGeneralSettings(settings);
            result = "Done";
        }
    }

    if (commandtype == "PMMGet,0,3") // GET tcp TO STRING 
    {
        result2 = GetTCPSettings();
    }

    else if (commandtype == "PMMGet,0,0") // GET PRODUCT TO STRING 
    {
        result2 = GetProductSettings();
    }


    else if (readData == "PMMSetSettings,0,1")
    {
        if (SerialUSB.available() > 0)
        {
            string settings = PMMReturnDataFromSerialUSB();
            PMMWriteIntoFlashGeneralSettings(settings);
            result = "Done";
        }

        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            PMMWriteIntoFlashGeneralSettings(settings);
            result = "Done";
        }
    }

    else if (readData == "PMMSetSettings,0,2")
    {
        if (SerialUSB.available() > 0)
        {
            string settings = PMMReturnDataFromSerialUSB();
            PMMWriteIntoFlashSerialSettings(settings);
            result = "Done";
        }

        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            PMMWriteIntoFlashSerialSettings(settings);
            result = "Done";
        }
    }

    else if (readData == "PMMSetSettings,0,3")
    {
        if (SerialUSB.available() > 0)
        {
            string settings = PMMReturnDataFromSerialUSB();
            PMMWriteIntoFlashTCPSettings(settings);
            result = "Done";
        }

        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            PMMWriteIntoFlashTCPSettings(settings);
            result = "Done";
        }
    }

    else if (readData == "PMMSetSettings,0,4")
    {
        if (SerialUSB.available() > 0)
        {
            string settings = PMMReturnDataFromSerialUSB();
            PMMWriteIntoFlashOptionsAndPinsSettings(settings);
            result = "Done";
        }

        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            PMMWriteIntoFlashOptionsAndPinsSettings(settings);
            result = "Done";
        }
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