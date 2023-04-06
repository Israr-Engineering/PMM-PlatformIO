#include "PmmInternalRTCLib.h"
#include "PmmInternalRTC.h"

PmmInternalRTC PMMRTC;

void PMMInternalRTCSetup()
{
    PMMRTC.begin();
}

void PMMInternalRTCSetUnixTime(int32_t UnixTime, float TimeZone)
{
    PMMRTC.setEpoch(UnixTime + (3600 * TimeZone));
}

uint32_t PMMInternalRTCGetUnixTime()
{
    return PMMRTC.getEpoch();
}

int16_t PMMInternalRTCGetDay()
{
    return PMMRTC.getDay();
}

int16_t PMMInternalRTCGetMonth()
{
    return PMMRTC.getMonth();
}

int16_t PMMInternalRTCGetYear()
{
    return PMMRTC.getYear() + 2000;
}

int16_t PMMInternalRTCGetHours()
{
    return PMMRTC.getHours();
}

int16_t PMMInternalRTCGetMinutes()
{
    return PMMRTC.getMinutes();
}

int16_t PMMInternalRTCGetSeconds()
{
    return PMMRTC.getSeconds();
}