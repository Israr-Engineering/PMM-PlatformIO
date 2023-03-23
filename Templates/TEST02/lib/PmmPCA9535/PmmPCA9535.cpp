#include <PmmPCA9535.h>
PCA9555 P1;
PCA9555 P2;

void PMMPCA9535Setup(bool ChipSelection, uint8_t Addrss, bool INVERTEDPolarity, bool INOROUT)
{
    if (ChipSelection == 0)
    {
        Wire.begin();
        P1.attach(Wire, Addrss);

        if (INVERTEDPolarity == 0)
            P1.polarity(PCA95x5::Polarity::ORIGINAL_ALL);
        if (INVERTEDPolarity == 1)
            P1.polarity(PCA95x5::Polarity::INVERTED_ALL);

        if (INOROUT == 0)
            P1.direction(PCA95x5::Direction::IN_ALL);
        if (INOROUT == 1)
            P1.direction(PCA95x5::Direction::OUT_ALL);
    }
    if (ChipSelection == 1)
    {
        Wire.begin();
        P2.attach(Wire, Addrss);

        if (INVERTEDPolarity == 0)
            P2.polarity(PCA95x5::Polarity::ORIGINAL_ALL);
        if (INVERTEDPolarity == 1)
            P2.polarity(PCA95x5::Polarity::INVERTED_ALL);

        if (INOROUT == 0)
            P2.direction(PCA95x5::Direction::IN_ALL);
        if (INOROUT == 1)
            P2.direction(PCA95x5::Direction::OUT_ALL);
    }
}

void PMMPCA9535Write(bool ChipSelection, uint8_t addres, bool Status) // this function use for digatal output :: the  P1 must be like the (PMMPCA9535Setup) and the adress to selcet which pin you need to do chang on it
{
    // the status is for give the a value if it 0 the digital out well be zero

    if (ChipSelection == 0)
    {

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P00, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P00, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P01, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P01, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P02, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P02, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P03, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P03, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P04, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P04, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P05, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P05, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P06, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P06, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P07, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P07, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P10, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P10, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P11, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P11, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P12, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P12, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P13, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P13, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P14, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P15, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P16, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P16, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P1.write(PCA95x5::Port::P17, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P1.write(PCA95x5::Port::P17, PCA95x5::Level::H);
        }
    }

    if (ChipSelection == 1)
    {

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P00, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P00, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P01, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P01, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P02, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P02, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P03, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P03, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P04, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P04, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P05, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P05, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P06, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P06, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P07, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P07, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P10, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P10, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P11, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P11, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P12, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P12, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P13, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P13, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P14, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P15, PCA95x5::Level::H);
        }

        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P16, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P16, PCA95x5::Level::H);
        }
        if ((addres == 0) && (Status == 0))
        {
            P2.write(PCA95x5::Port::P17, PCA95x5::Level::L);
        }
        else if ((addres == 0) && (Status == 1))
        {
            P2.write(PCA95x5::Port::P17, PCA95x5::Level::H);
        }
    }
}

uint16_t PMMPCA9535Read(bool ChipSelection, uint8_t Addres)
{

    uint16_t InputArray[16];

    if (ChipSelection == 0)

    {
        InputArray[0] = P1.read(PCA95x5::Port::P00);
        InputArray[1] = P1.read(PCA95x5::Port::P01);
        InputArray[2] = P1.read(PCA95x5::Port::P02);
        InputArray[3] = P1.read(PCA95x5::Port::P03);
        InputArray[4] = P1.read(PCA95x5::Port::P04);
        InputArray[5] = P1.read(PCA95x5::Port::P05);
        InputArray[6] = P1.read(PCA95x5::Port::P06);
        InputArray[7] = P1.read(PCA95x5::Port::P07);
        InputArray[8] = P1.read(PCA95x5::Port::P10);
        InputArray[9] = P1.read(PCA95x5::Port::P11);
        InputArray[10] = P1.read(PCA95x5::Port::P12);
        InputArray[11] = P1.read(PCA95x5::Port::P13);
        InputArray[12] = P1.read(PCA95x5::Port::P13);
        InputArray[13] = P1.read(PCA95x5::Port::P14);
        InputArray[14] = P1.read(PCA95x5::Port::P15);
        InputArray[15] = P1.read(PCA95x5::Port::P16);
    }

    if (ChipSelection == 1)
    {
        InputArray[0] = P1.read(PCA95x5::Port::P00);
        InputArray[1] = P1.read(PCA95x5::Port::P01);
        InputArray[2] = P1.read(PCA95x5::Port::P02);
        InputArray[3] = P1.read(PCA95x5::Port::P03);
        InputArray[4] = P1.read(PCA95x5::Port::P04);
        InputArray[5] = P1.read(PCA95x5::Port::P05);
        InputArray[6] = P1.read(PCA95x5::Port::P06);
        InputArray[7] = P1.read(PCA95x5::Port::P07);
        InputArray[8] = P1.read(PCA95x5::Port::P10);
        InputArray[9] = P1.read(PCA95x5::Port::P11);
        InputArray[10] = P1.read(PCA95x5::Port::P12);
        InputArray[11] = P1.read(PCA95x5::Port::P13);
        InputArray[12] = P1.read(PCA95x5::Port::P13);
        InputArray[13] = P1.read(PCA95x5::Port::P14);
        InputArray[14] = P1.read(PCA95x5::Port::P15);
        InputArray[15] = P1.read(PCA95x5::Port::P16);
    }

    return InputArray[Addres];
}