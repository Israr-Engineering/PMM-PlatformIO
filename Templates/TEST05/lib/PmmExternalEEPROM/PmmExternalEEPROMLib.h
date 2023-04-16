//#include "ProjectDef.h"

#ifdef PMMEEPROM

PmmExternalEEPROM PMMEprom;

extern void EEpromSetup();
extern int16_t GetIntDataFromEEprom(int16_t Address);
extern void PutIntDataToEEprom(int16_t Address, int16_t Value);

extern int32_t GetLongDataFromEEprom(int16_t Address);
extern void PutLongDataToEEprom(int16_t Address, int32_t Value);

extern String GetStringDataFromEEprom(int16_t Address);
extern void PutStringDataToEEprom(int16_t Address, String Value);

extern float GetFloatDataFromEEprom(int16_t Address);
extern void PutFloatDataToEEprom(int16_t Address, float Value);

void EEpromSetup()
{
    PMMEprom.begin();
}

int16_t GetIntDataFromEEprom(int16_t Address)
{
    int16_t  Value;
    PMMEprom.get(Address, Value);
    return Value;
}

void PutIntDataToEEprom(int16_t Address, int16_t Value)
{
    PMMEprom.put(Address, Value);
}

int32_t GetLongDataFromEEprom(int16_t Address)
{
    int32_t Value;
   PMMEprom.get(Address, Value);
    return Value;
}

void PutLongDataToEEprom(int16_t Address, int32_t Value)
{
    PMMEprom.put(Address, Value);
}

String GetStringDataFromEEprom(int16_t Address)
{
    String  Value;
    PMMEprom.get(Address, Value);
    return Value;
}

void PutStringDataToEEprom(int16_t Address, String Value)
{
    PMMEprom.put(Address, Value);
}

float GetFloatDataFromEEprom(int16_t Address)
{
    float  Value;
     PMMEprom.get(Address, Value);
    return Value;
}

void PutFloatDataToEEprom(int16_t Address, float Value)
{
    PMMEprom.put(Address, Value);
}


#endif