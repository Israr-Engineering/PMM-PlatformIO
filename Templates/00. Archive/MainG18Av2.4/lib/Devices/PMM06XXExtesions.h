
#ifndef PMM06XXEXTESIONS_H
#define PMM06XXEXTESIONS_H


#include "Arduino.h"
#include <PmmTypes.h>
#include <PmmCommands.h>
#include <PmmPCA95x5.h>

PCA9555 IOext[6] =
{
    PCA9555(),
    PCA9555(),
    PCA9555(),
    PCA9555(),
    PCA9555(),
    PCA9555()
};






void ExtensionBoardSetup(int SlotNumber ,int BoardName,int BoardAddress , bool PCASupport)
{
    if (SlotNumber < 1) SlotNumber = 1 ;

    if ((BoardName != 0) & PCASupport )
    {
        //1. 
        switch (BoardName)
        {
        case 620 :
            IOext[SlotNumber -1].attach(Wire,BoardAddress);
            IOext[SlotNumber -1].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[SlotNumber -1].direction(PCA95x5::Direction::IN_ALL);

            IOext[SlotNumber].attach(Wire,BoardAddress + 1);
            IOext[SlotNumber].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[SlotNumber].direction(PCA95x5::Direction::IN_ALL);
            break;

        case (625 | 626) :
            IOext[SlotNumber -1].attach(Wire,BoardAddress);
            IOext[SlotNumber -1].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[SlotNumber -1].direction(PCA95x5::Direction::OUT_ALL);

            IOext[SlotNumber].attach(Wire,BoardAddress + 1);
            IOext[SlotNumber].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[SlotNumber].direction(PCA95x5::Direction::OUT_ALL);
            break;

        case 628 :
            IOext[SlotNumber -1].attach(Wire,BoardAddress);
            IOext[SlotNumber -1].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[SlotNumber -1].direction(PCA95x5::Direction::IN_ALL);

            IOext[SlotNumber].attach(Wire,BoardAddress + 1);
            IOext[SlotNumber].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[SlotNumber].direction(PCA95x5::Direction::OUT_ALL);
            break;
        // default:
        // break;
        }
       
        

    }

}

void AllExtensionBoardsSetup()
{
    //ExtensionBoardSetup()
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