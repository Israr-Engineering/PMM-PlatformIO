
#ifndef PmmPWM_h
#define PmmPWM_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


class PmmPWMClass
{

public:

        PmmPWMClass();

		virtual ~PmmPWMClass();

        void PmmPWMSetup(int PWMOutputArray[], int Resolution);
        void PmmPWMWrite(uint16_t Value , int pinNumber);
        void PmmPWMWritePercentage(int Resolution, int Percentage, int pinNumber);

};


#endif