
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

int val = 0;

void ExtensionBoardSetup(int SlotNumber, int BoardName, uint8_t BoardAddress01, uint8_t BoardAddress02, bool PCASupport)
{
    if (SlotNumber < 0)
        SlotNumber = 0;

    int index01 = (SlotNumber - 1) * 2;
    int index02 = index01 + 1;

    if ((BoardName != 0) & PCASupport)
    {
        // 1.
        switch (BoardName)
        {
        case 620:

            Wire.begin();
            IOext[index01].attach(Wire, BoardAddress01);
            IOext[index01].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index01].direction(PCA95x5::Direction::IN_ALL);

            IOext[index02].attach(Wire, BoardAddress02);
            IOext[index02].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index02].direction(PCA95x5::Direction::IN_ALL);
            break;

        case 625: // transistor output

            Wire.begin();
            IOext[index01].attach(Wire, BoardAddress01);
            IOext[index01].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index01].direction(PCA95x5::Direction::OUT_ALL);
            IOext[index01].write(PCA95x5::Level::L_ALL);

            IOext[index02].attach(Wire, BoardAddress02);
            IOext[index02].polarity(PCA95x5::Polarity::ORIGINAL_ALL);
            IOext[index02].direction(PCA95x5::Direction::OUT_ALL);
            IOext[index02].write(PCA95x5::Level::L_ALL);

            break;
        case 626: // Relay output

            Wire.begin();
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
            Wire.begin();
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

    int MBIntIndex01 = (SlotNumber * 30);
    int MBIntIndex02 = MBIntIndex01 + 1;

    if (PAC9535Support)
    {
        switch (BoardName)
        {
        case 620:
            tmpint = IOext[index01].read();
            PmmIO.Inputs[MBIntIndex01] = tmpint;

            tmpint = IOext[index02].read();
            PmmIO.Inputs[MBIntIndex02] = tmpint;
            break;
        case 625:
            IOext[index01].write(PmmIO.Outputs[MBIntIndex01]);
            IOext[index02].write(PmmIO.Outputs[MBIntIndex02]);
            break;
        case 626:
            IOext[index01].write(PmmIO.Outputs[MBIntIndex01]);
            IOext[index02].write(PmmIO.Outputs[MBIntIndex02]);
            break;
        case 628:
            tmpint = IOext[index01].read();
            PmmIO.Inputs[MBIntIndex01] = tmpint;

            IOext[index02].write(PmmIO.Outputs[MBIntIndex02]);
            break;
        }
    }
    else  // read in normal I2C
    {
        
        byte I2CArray[64];

        // prepare board name and settings for ext board
        PmmIO.Outputs[MBIntIndex01 + 24] = BoardName ; //select board
        PmmIO.Outputs[MBIntIndex01 + 25] = 1967 ; // Ext board settings if any .


        // Send data to i2c board
        Wire.begin();
        Wire.beginTransmission(BoardAddress01); // transmit to device #BoardAddress01
        
        for (int i = 0; i < 32; i++)
        {
            int itemp = i * 2;
            I2CArray[itemp] = lowByte(PmmIO.Outputs[i + MBIntIndex01]);
            I2CArray[itemp + 1] = highByte(PmmIO.Outputs[i + MBIntIndex01]);
        }
        Wire.write(I2CArray, 64);
        Wire.endTransmission(); // stop transmitting

        // Request data from i2c Ext board
        Wire.requestFrom(BoardAddress01, 64); // Request 64 bytes from slave device BoardAddress01
        // Slave may send less than requested
        int ArryCounter = 0;
        while (Wire.available())
        {
            I2CArray[ArryCounter] = Wire.read();    // Receive a byte 
            //SerialUSB.print(I2CArray[ArryCounter]); // Test Print 
            ArryCounter++ ;
        }

        // convert byte array back to int array
        for (int i = 0; i < 32; i++)
        {
            int itemp = i * 2;
            PmmIO.Inputs[i + MBIntIndex01] =  I2CArray[itemp +1 ] ;
            PmmIO.Inputs[i + MBIntIndex01] =  (PmmIO.Inputs[i + MBIntIndex01] << 8) | I2CArray[itemp ] ;

            // I2CArray[itemp] = lowByte(PmmIO.Outputs[i]);
            // I2CArray[itemp + 1] = highByte(PmmIO.Outputs[i]);
            // SerialUSB.print(PmmIO.Inputs[i + MBIntIndex01]); // Print the character
        }


    }
}

void AllExtensionBoarsdUpdate()
{

    if (ThisProduct.PmmGeneral.Ext01Name != 0)
        ExtensionBoardUpdate(1, ThisProduct.PmmGeneral.Ext01Name,
                             ThisProduct.PmmGeneral.Ext01Address01,
                             ThisProduct.PmmGeneral.Ext01Pac9535);

    if (ThisProduct.PmmGeneral.Ext02Name != 0)
        ExtensionBoardUpdate(2, ThisProduct.PmmGeneral.Ext02Name,
                             ThisProduct.PmmGeneral.Ext02Address01,
                             ThisProduct.PmmGeneral.Ext02Pac9535);

    if (ThisProduct.PmmGeneral.Ext03Name != 0)
        ExtensionBoardUpdate(3, ThisProduct.PmmGeneral.Ext03Name,
                             ThisProduct.PmmGeneral.Ext03Address01,
                             ThisProduct.PmmGeneral.Ext03Pac9535);
}

#endif