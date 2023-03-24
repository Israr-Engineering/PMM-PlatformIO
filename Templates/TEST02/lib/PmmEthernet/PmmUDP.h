//#include "ProjectDef.h"
#define UDP_TX_PACKET_MAX_SIZE 64
#ifdef PMMUDP

extern void PMMUDPInitialize(PmmEthernetUDP &UDP, int UDPPort);
extern void PMMUDPSendUint8_Message(PmmEthernetUDP &UDP, int UDPPort, char *CharRemoteIPAddress, uint8_t *msg, uint16_t msgLength);
extern void PMMUDPSendChar_Message(PmmEthernetUDP &UDP, int UDPPort, char *CharRemoteIPAddress, char *msg, uint16_t msgLength);
extern int PMMUDPRecivedMessage(PmmEthernetUDP &UDP, char *msg);

void PMMUDPInitialize(PmmEthernetUDP &UDP, int UDPPort)
{
    UDP.begin(UDPPort);
}

void PMMUDPSendUint8_Message(PmmEthernetUDP &UDP, int UDPPort, char *CharRemoteIPAddress, uint8_t *msg, uint16_t msgLength)
{
    UDP.beginPacket(CharRemoteIPAddress, UDPPort);
    UDP.write(msg, msgLength);
    UDP.endPacket();
    UDP.clearWriteError();
    UDP.flush();
}

void PMMUDPSendChar_Message(PmmEthernetUDP &UDP, int UDPPort, char *CharRemoteIPAddress, char *msg, uint16_t msgLength)
{
    UDP.beginPacket(CharRemoteIPAddress, UDPPort);
    UDP.write(msg, msgLength);
    UDP.endPacket();
    UDP.clearWriteError();
    UDP.flush();
}

int PMMUDPRecivedMessage(PmmEthernetUDP &UDP, char *msg)
{
    int UDPpacketSize = UDP.parsePacket();
    if (UDPpacketSize)
    {
        UDP.read(msg, UDP_TX_PACKET_MAX_SIZE);
        UDP.clearWriteError();
        UDP.flush();
    }
    return UDPpacketSize;
}

#endif