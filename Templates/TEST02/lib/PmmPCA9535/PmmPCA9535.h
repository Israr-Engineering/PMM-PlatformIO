#ifndef PCA9535
#define PCA5935
#include "PmmPCA95X5.h"
#define PCA9535INVERTED 1
#define PCA9535NONINVERTED 0
#define PCA9535OUTPUT 1
#define PCA9535INPUT 0
void PMMPCA9535Setup(bool ChipSelection, uint8_t Addrss, bool INVERTEDPolarity, bool INOROUT);
void PMMPCA9535Write(bool ChipSelection, uint8_t addres, bool Status);
uint16_t PMMPCA9535Read(bool ChipSelection, uint8_t Addres);
#endif