
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
        PCA9555()};

        int val = 0 ;

void ExtensionBoardSetup(int SlotNumber, int BoardName, u_int8_t BoardAddress01,u_int8_t BoardAddress02, bool PCASupport)
{
    if (SlotNumber < 0)
        SlotNumber = 0;

    int index01 = (SlotNumber - 1) * 2;
    int index02 = index01 + 1;

    Wire.begin();

    if ((BoardName != 0) & PCASupport)
    {
        // 1.
        switch (BoardName)
        {
        case 620:
            IOext[index01].attach(Wire, BoardAddress01);
            IOext[index01].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index01].direction(PCA95x5::Direction::IN_ALL);

            IOext[index02].attach(Wire, BoardAddress02);
            IOext[index02].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index02].direction(PCA95x5::Direction::IN_ALL);
            break;

        case 625 : // || 626
            IOext[index01].attach(Wire, BoardAddress01);
            IOext[index01].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index01].direction(PCA95x5::Direction::OUT_ALL);
            IOext[index01].write(PCA95x5::Level::L_ALL);

            IOext[index02].attach(Wire, BoardAddress02);
            IOext[index02].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index02].direction(PCA95x5::Direction::OUT_ALL);
            IOext[index02].write(PCA95x5::Level::L_ALL);

            break;

        case 628:
            IOext[index01].attach(Wire, BoardAddress01);
            IOext[index01].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index01].direction(PCA95x5::Direction::IN_ALL);

            IOext[index02].attach(Wire, BoardAddress02);
            IOext[index02].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index02].direction(PCA95x5::Direction::OUT_ALL);
            IOext[index02].write(PCA95x5::Level::L_ALL);

            break;
            // default:
            // break;
        }
    }
}

void AllExtensionBoardsSetup()
{

    // Wire.begin();
    // IOext[0].attach(Wire, 0x20);
    // IOext[0].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
    // IOext[0].direction(PCA95x5::Direction::OUT_ALL);
    // IOext[0].write(PCA95x5::Level::L_ALL);
    // IOext[1].attach(Wire, 0x21); // to select one of the io expander to comunecate with 
    // IOext[1].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
    // IOext[1].direction(PCA95x5::Direction::OUT_ALL);
    // IOext[1].write(PCA95x5::Level::L_ALL);


    // ExtensionBoard #1
    if ((ThisProduct.PmmGeneral.Ext01Name != 0) & ThisProduct.PmmGeneral.Ext01Pac9535)
        ExtensionBoardSetup(1, ThisProduct.PmmGeneral.Ext01Name,
                            ThisProduct.PmmGeneral.Ext01Address01,
                            ThisProduct.PmmGeneral.Ext01Address02, 1);
    // ExtensionBoard #2
    if ((ThisProduct.PmmGeneral.Ext02Name != 0) & ThisProduct.PmmGeneral.Ext02Pac9535)
        ExtensionBoardSetup(2, ThisProduct.PmmGeneral.Ext02Name,
                            ThisProduct.PmmGeneral.Ext02Address01,
                            ThisProduct.PmmGeneral.Ext01Address02, 1);
    // ExtensionBoard #3
    if ((ThisProduct.PmmGeneral.Ext03Name != 0) & ThisProduct.PmmGeneral.Ext03Pac9535)
        ExtensionBoardSetup(3, ThisProduct.PmmGeneral.Ext03Name,
                            ThisProduct.PmmGeneral.Ext01Address01,
                            ThisProduct.PmmGeneral.Ext01Address02, 1);
}

void ExtensionBoardUpdate(int SlotNumber, int BoardName, int BoardAddress01, bool PAC9535Support = false)
{

    u_int16_t tmpint = 0;
    int index01 = (SlotNumber - 1) * 2;
    int index02 = index01 + 1;

    //int MBbyteIndex01 = (SlotNumber * 30);
    //int MBbyteIndex02 = MBbyteIndex01 +1;

    int MBIntIndex01 = (SlotNumber * 30);
    int MBIntIndex02 = MBIntIndex01 + 1 ;

     //SerialUSB.println("update ext01");

    // Check if device support PAC9535 (06xxXP) or normal MCU
    // 0620xp = 2620 + 0625xp = 2625 +0626xp =2626
    if (PAC9535Support)
    {
        switch (BoardName)
        {
        case 620:
            tmpint = IOext[index01].read();
            // PmmIO.InputsByte[MBbyteIndex01] = lowByte(tmpint);
            // PmmIO.InputsByte[MBbyteIndex01 + 1] = highByte(tmpint);
            PmmIO.Inputs[MBIntIndex01] = tmpint;

            tmpint = IOext[index02].read();
            // PmmIO.InputsByte[MBbyteIndex01 + 2] = lowByte(tmpint);
            // PmmIO.InputsByte[MBbyteIndex01 + 3] = highByte(tmpint);
            PmmIO.Inputs[MBIntIndex02] = tmpint;
            break;
        case 625 :
            
            // tmpint = BytesToInt(PmmIO.OutputsByte[MBbyteIndex01],PmmIO.OutputsByte[MBbyteIndex01+1]);
            // tmpint = 0xffff;
            //IOext[index01].write(tmpint);
            // SerialUSB.print("index : ");SerialUSB.println(MBbyteIndex01);
            // SerialUSB.print("Data : ");SerialUSB.println(tmpint);
            //tmpint = BytesToInt(PmmIO.OutputsByte[MBbyteIndex01+2],PmmIO.OutputsByte[MBbyteIndex01+3]);

            IOext[index01].write(PmmIO.Outputs[MBIntIndex01]);
            IOext[index02].write(PmmIO.Outputs[MBIntIndex02]);
            break;
        case 628:
            tmpint = IOext[index01].read();
            // PmmIO.InputsByte[MBbyteIndex01] = lowByte(tmpint);
            // PmmIO.InputsByte[MBbyteIndex01 + 1] = highByte(tmpint);
            PmmIO.Inputs[MBIntIndex01] = tmpint;

            //tmpint = BytesToInt(PmmIO.OutputsByte[MBbyteIndex01],PmmIO.OutputsByte[MBbyteIndex01+1]);
            IOext[index02].write(PmmIO.Outputs[MBIntIndex02]);   
            break;            
        }
    }
    else
    {
        // read in normal I2C
    }
}

void AllExtensionBoarsdUpdate()
{

    //IOext[0].write(0xffff);
    //  IOext[0].write(val);
    // IOext[1].write(val + 1000);
    // val++;


    if (ThisProduct.PmmGeneral.Ext01Name != 0)
    ExtensionBoardUpdate(1,ThisProduct.PmmGeneral.Ext01Name,
                           ThisProduct.PmmGeneral.Ext01Address01,
                           ThisProduct.PmmGeneral.Ext01Pac9535);

    if (ThisProduct.PmmGeneral.Ext02Name != 0)
    ExtensionBoardUpdate(2,ThisProduct.PmmGeneral.Ext02Name,
                           ThisProduct.PmmGeneral.Ext02Address01,
                           ThisProduct.PmmGeneral.Ext02Pac9535);

    if (ThisProduct.PmmGeneral.Ext03Name != 0)
    ExtensionBoardUpdate(3,ThisProduct.PmmGeneral.Ext03Name,
                           ThisProduct.PmmGeneral.Ext03Address01,
                           ThisProduct.PmmGeneral.Ext03Pac9535);
}

#endif