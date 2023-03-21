#include "../Commons/ProjectDef.h"

#ifdef PMMI2C

extern void PMMI2CMasterRead(int id);
extern void PMMI2CSlaveSendRequest();
extern void PMMI2CSlaveWrite(int Value);
extern int *PMMI2CMaster(int id, int NumbOfBytes);

long SlaveTimer = 0;
long MasterTimer = 0;

void PMMInitializeI2CSlave(int id)
{
    Wire.begin(id);
}

void PMMI2CSlaveSendRequest()
{
    
    if (millis() > (SlaveTimer + 100))
    {
        Wire.onRequest(PMMI2CSlaveWrite);
        SlaveTimer = millis();
    }
}

void PMMI2CSlaveWrite(int Value)
{
    Wire.write(Value);
}

int *PMMI2CMasterRead(int id, int NumbOfBytes)
{
    int ReturnArr[NumbOfBytes];
    Wire.requestFrom(id, NumbOfBytes);

    for (int itm = 0; itm < NumbOfBytes; itm++)
    {
        if (Wire.available())
        {
            if (millis() > (MasterTimer + 10))
            {
                ReturnArr[itm] = Wire.read();
                MasterTimer = millis();
            }
        }
    }

    return ReturnArr;
}

#endif