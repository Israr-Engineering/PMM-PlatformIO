#include "ProjectDef.h"

#ifdef PMMCommands

extern void PMMInitializeEthernet(IPAddress ip, byte mac[]);
extern void PMMReadCommands();
extern string PMMCommnads(string readData);
extern string PMMReturnDataFromSerialUSB();
extern string PMMReturnDataFromAPIHTTPHeader();

EthernetServer server(80);
EthernetClient client = server.available();

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
    if (readData == "PMMSetUSBConfigurationSettings")
    {
        if (SerialUSB.available() > 0)
        {
            string settings = PMMReturnDataFromSerialUSB();
            PMMSetUSBConfigurationSettings(settings);
            result = "PMMSetUSBConfigurationSettings";
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

    // else if (readData == "RTC")
    // {
    //     DateTime RTCNow = PMMGetRTCNOW();
    //     result = DateTimeToString(RTCNow.year(), RTCNow.month(), RTCNow.day(), RTCNow.hour(), RTCNow.minute(), RTCNow.second());
    // }
    // else if (readData == "RTCUNIX")
    // {
    //     result = ConvertUint32_tTostring(PMMGetRTCUNIXTime());
    // }
    // else if (readData == "SetRTCUNIX")
    // {
    //     PMMSetRTCUNIXTime(GetUnixDateTimeNow());
    // }
    // else if (readData == "CheckHardware")
    // {
    //     CheckAvailabeHardware();
    // }

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
    EthernetClient client = server.available();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    // client.println("Refresh: 5");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.print(Data);
    client.println("</head>");
    client.println("</html>");
    client.stop();
}

#endif