#include "PMMFlashMemoryLib.h"
#include <PmmFlashAsEEPROM.h>

void PMMFlashWriteInt (int Address, int Value)
{
    EEPROM.update(Address, Value);
}

void PMMFlashWriteFloat (int Address, float Value)
{
    EEPROM.update(Address, Value);
}

void PMMFlashWriteLong (int Address, long Value)
{
    EEPROM.update(Address, Value);
}

void PMMFlashWriteString (int Address, String Value)
{
    //EEPROM.update(Address, Value);
}

int PMMFlashReadInt(int Address, int Value)
{
    return EEPROM.read(Address);
}

float PMMFlashReadFloat(int Address, float Value)
{
    return EEPROM.read(Address);
}

long PMMFlashReadLong(int Address, long Value)
{
    return EEPROM.read(Address);
}

String PMMFlashReadString(int Address, String Value)
{
    return "Under Construction";
    //return EEPROM.read(Address).c_str();
}

