#include "PmmCommands.h"
#include <PmmEthernet.h>
#include <string>
using namespace std;

PmmEthernetServer server(80);
PmmEthernetClient client = server.available();

void PMMInitializeEthernet(IPAddress ip, byte mac[]);
void PMMReadCommands();
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

void PMMReadCommands()
{
    if (SerialUSB.available() > 0)
    {
        PMMCommnads(PMMReturnDataFromSerialUSB());
    }

    if (client)
    {
        PMMCommnads(PMMReturnDataFromAPIHTTPHeader());
    }
}

string PMMCommnads(string readData)
{
    string result = "";

    if (readData == "PMMSetSettings,0")
    {
        if (SerialUSB.available() > 0)
        {
            string settings = PMMReturnDataFromSerialUSB();
            PMMSetUSBConfigurationSettings(settings);
            //PMMSetDeviceSettingsEProm();
            result = "Done";
        }

        if (client)
        {
            string settings = PMMReturnDataFromAPIHTTPHeader();
            PMMSetUSBConfigurationSettings(settings);
            result = "PMMSetUSBConfigurationSettings";
        }
    }
    else if (readData == "PMMGetUSBConfigurationSettings")
    {
        result = PMMGetUSBConfigurationSettings();
    }
    else if (readData == "PMMUSBConfiguration")
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