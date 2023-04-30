
#ifndef PMM06XXEXTESIONS_H
#define PMM06XXEXTESIONS_H


#include "Arduino.h"
#include <PmmTypes.h>
#include <PmmCommands.h>


void ExtensionBoardSetup(int BoardName,int BoardAddress)
{

}

void ExtensionBoardUpdate(int BoardName,int BoardOrder ,int BoardAddress01 ,int BoardAddress02 , bool PAC9535Support = false )
{

    // Check if device support PAC9535 (06xxXP) or normal MCU
    // 0620xp = 2620 + 0625xp = 2625 +0626xp =2626
    if (PAC9535Support)
    {

    }
    else
    {
        // read in normal I2C
        

    }


}

void AllExtensionBoarsdUpdate()
{


}





#endif