#include <PmmPCA9535.h>
PmmPCA9555 P1;
PmmPCA9555 P2;

void PMMPCA9535Setup(bool ChipSelection, uint8_t Addrss, bool INVERTEDPolarity, bool INOROUT)
{
    if (ChipSelection == 0)
    {
        Wire.begin();
        P1.attach(Wire, Addrss);

        if (INVERTEDPolarity == 0)
            P1.polarity(PmmPCA95x5::Polarity::ORIGINAL_ALL);
        if (INVERTEDPolarity == 1)
            P1.polarity(PmmPCA95x5::Polarity::INVERTED_ALL);

        if (INOROUT == 0)
            P1.direction(PmmPCA95x5::Direction::IN_ALL);
        if (INOROUT == 1)
            P1.direction(PmmPCA95x5::Direction::OUT_ALL);
    }
    if (ChipSelection == 1)
    {
        Wire.begin();
        P2.attach(Wire, Addrss);

        if (INVERTEDPolarity == 0)
            P2.polarity(PmmPCA95x5::Polarity::ORIGINAL_ALL);
        if (INVERTEDPolarity == 1)
            P2.polarity(PmmPCA95x5::Polarity::INVERTED_ALL);

        if (INOROUT == 0)
            P2.direction(PmmPCA95x5::Direction::IN_ALL);
        if (INOROUT == 1)
            P2.direction(PmmPCA95x5::Direction::OUT_ALL);
    }
}

void PMMPCA9535Write(bool ChipSelection, uint8_t addres, bool Status) // this function use for digatal output :: the  P1 must be like the (PMMPCA9535Setup) and the adress to selcet which pin you need to do chang on it
{
    // the status is for give the a value if it 0 the digital out well be zero

    if (ChipSelection == 0)
    {

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P00, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P00, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P01, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P01, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P02, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P02, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P03, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P03, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P04, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P04, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P05, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P05, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P06, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P06, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P07, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P07, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P10, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P10, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P11, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P11, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P12, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P12, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P13, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P13, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P14, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P15, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P16, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P16, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PmmPCA95x5::Port::P17, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PmmPCA95x5::Port::P17, PmmPCA95x5::Level::H);
        }
    }

    if (ChipSelection == 1)
    {

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P00, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P00, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P01, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P01, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P02, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P02, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P03, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P03, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P04, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P04, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P05, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P05, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P06, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P06, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P07, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P07, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P10, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P10, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P11, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P11, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P12, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P12, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P13, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P13, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P14, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P15, PmmPCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P16, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P16, PmmPCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PmmPCA95x5::Port::P17, PmmPCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PmmPCA95x5::Port::P17, PmmPCA95x5::Level::H);
        }
    }
}

uint16_t PMMPCA9535Read(bool ChipSelection, uint8_t Addres)
{

    uint16_t InputArray[16];

    if (ChipSelection == 0)

    {
        InputArray[0] = P1.read(PmmPCA95x5::Port::P00);
        InputArray[1] = P1.read(PmmPCA95x5::Port::P01);
        InputArray[2] = P1.read(PmmPCA95x5::Port::P02);
        InputArray[3] = P1.read(PmmPCA95x5::Port::P03);
        InputArray[4] = P1.read(PmmPCA95x5::Port::P04);
        InputArray[5] = P1.read(PmmPCA95x5::Port::P05);
        InputArray[6] = P1.read(PmmPCA95x5::Port::P06);
        InputArray[7] = P1.read(PmmPCA95x5::Port::P07);
        InputArray[8] = P1.read(PmmPCA95x5::Port::P10);
        InputArray[9] = P1.read(PmmPCA95x5::Port::P11);
        InputArray[10] = P1.read(PmmPCA95x5::Port::P12);
        InputArray[11] = P1.read(PmmPCA95x5::Port::P13);
        InputArray[12] = P1.read(PmmPCA95x5::Port::P13);
        InputArray[13] = P1.read(PmmPCA95x5::Port::P14);
        InputArray[14] = P1.read(PmmPCA95x5::Port::P15);
        InputArray[15] = P1.read(PmmPCA95x5::Port::P16);
    }

    if (ChipSelection == 1)
    {
        InputArray[0] = P1.read(PmmPCA95x5::Port::P00);
        InputArray[1] = P1.read(PmmPCA95x5::Port::P01);
        InputArray[2] = P1.read(PmmPCA95x5::Port::P02);
        InputArray[3] = P1.read(PmmPCA95x5::Port::P03);
        InputArray[4] = P1.read(PmmPCA95x5::Port::P04);
        InputArray[5] = P1.read(PmmPCA95x5::Port::P05);
        InputArray[6] = P1.read(PmmPCA95x5::Port::P06);
        InputArray[7] = P1.read(PmmPCA95x5::Port::P07);
        InputArray[8] = P1.read(PmmPCA95x5::Port::P10);
        InputArray[9] = P1.read(PmmPCA95x5::Port::P11);
        InputArray[10] = P1.read(PmmPCA95x5::Port::P12);
        InputArray[11] = P1.read(PmmPCA95x5::Port::P13);
        InputArray[12] = P1.read(PmmPCA95x5::Port::P13);
        InputArray[13] = P1.read(PmmPCA95x5::Port::P14);
        InputArray[14] = P1.read(PmmPCA95x5::Port::P15);
        InputArray[15] = P1.read(PmmPCA95x5::Port::P16);
    }

    return InputArray[Addres];
}