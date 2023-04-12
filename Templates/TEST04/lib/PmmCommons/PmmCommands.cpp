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
    if (SerialUSB.available() > 0)
    {
        result = PMMCommnads(PMMReturnDataFromSerialUSB());
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

    if (readData == "PMMSetSettings,0,0")
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

    else if (readData == "PMMGetSettings,0")
    {
        result = PMMReadFromFlashAllSettings();
    }

    else if (readData == "PMMTestConfiguration")
    {
        result = PMMIsAlive();
    }

    return result;
}

string PMMReturnDataFromSerialUSB()
{
    String Command = "";
    if (SerialUSB.available() > 0)
    {
        SerialUSB.setTimeout(200);
        Command = SerialUSB.readStringUntil('\n');
    }

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