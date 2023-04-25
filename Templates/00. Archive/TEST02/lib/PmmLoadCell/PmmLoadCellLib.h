#include "PmmHX711.h"

PmmHX711 PMMLoadCell;

class PmmLoadCell
{
public:   
#ifndef PmmHX711 
  void PmmLoadCellSetup(uint8_t DoutPin = 15, uint8_t SCKPin = 16);
  bool PmmLoadCellReady();
  void PmmLoadCellSetOffset(float Offset = 0);
 float PmmLoadCellGetOffset();
  void PmmLoadCellSetScale(float Scale = 1);
 float PmmLoadCellGetScale();
 float PmmLoadCellGetRealRead(uint8_t NumberOfReads = 10);
 float PmmLoadCellGetScaleReading(uint8_t NumberOfReads = 10);
 #endif 
 void PmmADS1231Setup();
 uint32_t PmmADS1231ReadRowData();
 float PmmADS1231Caliberation(float RealScale);
 float PmmADS1231ReadScaleValue();
}

