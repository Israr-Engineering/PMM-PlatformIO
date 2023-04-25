//#include "ProjectDef.h"

#ifdef PMMEEPROM

PmmExternalEEPROM PMMEprom;

extern void EEpromSetup();
extern int GetIntDataFromEEprom(int Address);
extern void PutIntDataToEEprom(int Address, int Value);

extern long GetLongDataFromEEprom(int Address);
extern void PutLongDataToEEprom(int Address, long Value);

extern String GetStringDataFromEEprom(int Address);
extern void PutStringDataToEEprom(int Address, String Value);

extern float GetFloatDataFromEEprom(int Address);
extern void PutFloatDataToEEprom(int Address, float Value);

extern bool GetBoolDataFromEEprom(int Address);
extern void PutBoolDataToEEprom(int Address, bool Value);

void EEpromSetup()
{
    PMMEprom.begin();
}

bool GetBoolDataFromEEprom(int Address)
{
    bool Value = false;
    PMMEprom.get(Address, Value);
    return Value;
}

void PutBoolDataToEEprom(int Address, bool Value)
{
    PMMEprom.put(Address, Value);
}

int GetIntDataFromEEprom(int Address)
{
    int Value;
    PMMEprom.get(Address, Value);
    return Value;
}

void PutIntDataToEEprom(int Address, int Value)
{
    PMMEprom.put(Address, Value);
}

long GetLongDataFromEEprom(int Address)
{
    long Value;
    PMMEprom.get(Address, Value);
    return Value;
}

void PutLongDataToEEprom(int Address, long Value)
{
    PMMEprom.put(Address, Value);
}

String GetStringDataFromEEprom(int Address)
{
    String Value;
    PMMEprom.get(Address, Value);
    return Value;
}

void PutStringDataToEEprom(int Address, String Value)
{
    PMMEprom.put(Address, Value);
}

float GetFloatDataFromEEprom(int Address)
{
    float Value;
    PMMEprom.get(Address, Value);
    return Value;
}

void PutFloatDataToEEprom(int Address, float Value)
{
    PMMEprom.put(Address, Value);
}

#endif