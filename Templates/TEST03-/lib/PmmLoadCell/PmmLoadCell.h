#include "PmmHX711.h"
PmmHX711 PmmLoadCell;

extern void PmmLoadCellSetup(uint8_t DoutPin = 15, uint8_t SCKPin = 16);
extern bool PmmLoadCellReady();
extern void PmmLoadCellSetOffset(float Offset = 0);
extern float PmmLoadCellGetOffset();
extern void PmmLoadCellSetScale(float Scale = 1);
extern float PmmLoadCellGetScale();
extern float PmmLoadCellGetRealRead(uint8_t NumberOfReads = 10);
extern float PmmLoadCellGetScaleReading(uint8_t NumberOfReads = 10);

void PmmLoadCellSetup(uint8_t DoutPin, uint8_t SCKPin)
{
    PmmLoadCell.begin(DoutPin, SCKPin);
}

bool PmmLoadCellReady()
{
    return PmmLoadCell.is_ready();
}

void PmmLoadCellSetOffset(float Offset)
{
    PmmLoadCell.set_offset(Offset);
}

float PmmLoadCellGetOffset()
{
    PmmLoadCell.get_offset();
}

void PmmLoadCellSetScale(float Scale)
{
    PmmLoadCell.set_scale(Scale);
}

float PmmLoadCellGetScale()
{
    PmmLoadCell.get_scale();
}

float PmmLoadCellGetRealRead(uint8_t NumberOfReads)
{
    PmmLoadCell.read_average(NumberOfReads);
}

float PmmLoadCellGetScaleReading(uint8_t NumberOfReads)
{
    PmmLoadCell.get_units(NumberOfReads);
}
