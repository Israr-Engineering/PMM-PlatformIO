#include <PmmPWM.h>


PmmPWMClass::PmmPWMClass() {
}

PmmPWMClass::~PmmPWMClass() {
}

void PmmPWMClass::PmmPWMSetup(int PWMOutputArray[], int Resolution)
{
    int Length= sizeof(PWMOutputArray) / sizeof(PWMOutputArray[0]);
    for(int i=0;i<Length;i++)
    {
        pinMode(PWMOutputArray[i],OUTPUT);
    }

    analogWriteResolution(Resolution);
}

void PmmPWMClass::PmmPWMWrite(uint16_t Value , int pinNumber)
{
    analogWrite(pinNumber, Value);
}

void PmmPWMClass::PmmPWMWritePercentage(int Resolution, int Percentage, int pinNumber)
{
    uint16_t FullValue= pow(2,Resolution);
    uint16_t Value = FullValue * (Percentage/100);
    analogWrite(pinNumber, Value);
}