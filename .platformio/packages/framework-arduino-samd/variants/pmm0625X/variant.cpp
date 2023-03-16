
#include "variant.h"

/*
 * Pins descriptions
 */
const PinDescription g_APinDescription[] =
    {

        // 0..13 - Digital pins
        // ----------------------
        // 0/1 - SERCOM/UART (Serial1)
         //(0)
         { PORTA, 11, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11 }, // RX: SERCOM0/PAD[3]
         //(1)
         { PORTA, 10, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_10 }, // TX: SERCOM0/PAD[2]
       


        //(2)
        {PORTB, 8, PIO_ANALOG, (PIN_ATTR_PWM | PIN_ATTR_TIMER), ADC_Channel2, PWM4_CH0, TC4_CH0, EXTERNAL_INT_8}, // ADC/AIN[2]
        //(3)
        {PORTB, 9, PIO_ANALOG, (PIN_ATTR_PWM | PIN_ATTR_TIMER), ADC_Channel3, PWM4_CH1, TC4_CH1, EXTERNAL_INT_9}, // ADC/AIN[3] 
        //(4)
        {PORTA, 4, PIO_ANALOG, 0, ADC_Channel4, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_4},                        // ADC/AIN[4]
        //(5)
        {PORTA, 5, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM1_CH0, TCC1_CH0, EXTERNAL_INT_6}, // TCC1/WO[0]
        //(6)
        {PORTA, 8, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER | PIN_ATTR_ANALOG), ADC_Channel16, PWM0_CH0, TCC0_CH0, EXTERNAL_INT_NMI}, // TCC0/WO[0]
        //(7)
        {PORTA, 9, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER | PIN_ATTR_ANALOG), ADC_Channel17, PWM0_CH1, TCC0_CH1, EXTERNAL_INT_9},   // TCC0/WO[1]
        //(8)
        {PORTA, 10, PIO_SERCOM, (PIN_ATTR_DIGITAL | PIN_ATTR_ANALOG), ADC_Channel18, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_10}, // TX: SERCOM0/PAD[2]
        //(9)
        {PORTA, 11, PIO_SERCOM, (PIN_ATTR_DIGITAL | PIN_ATTR_ANALOG), ADC_Channel19, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11}, // RX: SERCOM0/PAD[3]
       



       
       
       
        //(10)
        {PORTA, 6, PIO_ANALOG, 0, ADC_Channel5, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_5},                                                      // ADC/AIN[5]
        //(11)
        {PORTA, 7, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM1_CH1, TCC1_CH1, EXTERNAL_INT_7}, // TCC1/WO[1]
        
        //(12)
        {PORTA, 13, PIO_PWM, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM), No_ADC_Channel, PWM0_CH5, NOT_ON_TIMER, EXTERNAL_INT_13},       // EIC/EXTINT[13] *TCC2/WO[1] TCC0/WO[7]
        //(13)
        {PORTA, 20, PIO_TIMER_ALT, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER_ALT), No_ADC_Channel, PWM0_CH6, TCC0_CH6, EXTERNAL_INT_4}, // TCC0/WO[6]
        //(14)
        {PORTA, 21, PIO_TIMER_ALT, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER_ALT), No_ADC_Channel, PWM0_CH7, TCC0_CH7, EXTERNAL_INT_5}, // TCC0/WO[7]
        //(15)
        {PORTB, 2, PIO_ANALOG, 0, ADC_Channel10, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2},                       // ADC/AIN[10]
        //(16)
        {PORTB, 3, PIO_OUTPUT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE},  // used as output only
        //(17)
        {PORTA, 6, PIO_ANALOG, 0, ADC_Channel5, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_5},  // ADC/AIN[5]
       

 
                                                                                                                                 // 2..12
         
         
                                                                                                                        // Digital Low
        //(18)
        {PORTA, 14, PIO_DIGITAL, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_14},

        // {PORTA, 9, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH1, TCC0_CH1, EXTERNAL_INT_9},          // TCC0/WO[1]
        // {PORTA, 8, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH0, TCC0_CH0, EXTERNAL_INT_NMI},        // TCC0/WO[0]
        //(19)
        {PORTA, 15, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH1, TC3_CH1, EXTERNAL_INT_15},         // TC3/WO[1]
        // Digital High
        //(20)
        {PORTA, 5, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM1_CH0, TCC1_CH0, EXTERNAL_INT_6}, // TCC1/WO[0]

        // {PORTA, 18, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH0, TC3_CH0, EXTERNAL_INT_2},  // TC3/WO[0]
        // {PORTA, 16, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM2_CH0, TCC2_CH0, EXTERNAL_INT_0}, // TCC2/WO[0]
        //(21)
        {PORTA, 18, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH0, TC3_CH0, EXTERNAL_INT_2},  // TC3/WO[0]
        //(22)
        {PORTA, 16, PIO_TIMER, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM2_CH0, TCC2_CH0, EXTERNAL_INT_0}, // TCC2/WO[0]

        //(23)
        {PORTA, 19, PIO_TIMER_ALT, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER_ALT), No_ADC_Channel, PWM0_CH3, TCC0_CH3, EXTERNAL_INT_3}, // TCC0/WO[3]

        // 13 (LED)
        //(24)
        {PORTA, 17, PIO_PWM, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), No_ADC_Channel, PWM2_CH1, TCC2_CH1, EXTERNAL_INT_1}, // TCC2/WO[1]

        // 14..19 - Analog pins
        // --------------------
        //(25)
        {PORTA, 2, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2},          // ADC/AIN[0]
        
        


        // 20..21 I2C pins (SDA/SCL and also EDBG:SDA/SCL)
        // ----------------------
        //(26)
        {PORTA, 22, PIO_SERCOM, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_6}, // SDA: SERCOM3/PAD[0]
        //(0){PORTA, 23, PIO_SERCOM, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_7}, // SCL: SERCOM3/PAD[1]


        // 22..24 - SPI pins (ICSP:MISO,SCK,MOSI)
        // ----------------------
        //(27)
        {PORTA, 12, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_12}, // MISO: SERCOM4/PAD[0]
        //(28)
        {PORTB, 10, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_10}, // MOSI: SERCOM4/PAD[2]
        //(29)
        {PORTB, 11, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11}, // SCK: SERCOM4/PAD[3]

        // 25..26 - RX/TX LEDS (PB03/PA27)
        // --------------------
        
        //(30)
        {PORTA, 27, PIO_OUTPUT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // used as output only

        // 27..29 - USB
        // --------------------
        //(31)
        {PORTA, 28, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // USB Host enable
        //(32)
        {PORTA, 24, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // USB/DM
        //(33)
        {PORTA, 25, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // USB/DP

        // 30..41 - EDBG
        // ----------------------
        // 30/31 - EDBG/UART
        //(0){PORTB, 22, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // TX: SERCOM5/PAD[2]

        //(34)
        {PORTB, 23, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // RX: SERCOM5/PAD[3]

        // 32/33 I2C (SDA/SCL and also EDBG:SDA/SCL)
        //(0){PORTA, 22, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // SDA: SERCOM3/PAD[0]

        //(35)
        {PORTA, 23, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // SCL: SERCOM3/PAD[1]

        // 34..37 - EDBG/SPI
        //(36)
        {PORTA, 19, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // MISO: SERCOM1/PAD[3]
        //(37)
        {PORTA, 16, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // MOSI: SERCOM1/PAD[0]
        //(38)
        {PORTA, 18, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // SS: SERCOM1/PAD[2]
        //(39)
        {PORTA, 17, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // SCK: SERCOM1/PAD[1]
                                                                                                             // { PORTA, 11, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11 }, // RX: SERCOM0/PAD[3]
                                                                                                             // { PORTA, 10, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_10 }, // TX: SERCOM0/PAD[2]
                                                                                                             // 38..41 - EDBG/Digital
        
        //(40)
        {PORTA, 21, PIO_PWM_ALT, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM), No_ADC_Channel, PWM0_CH7, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // Pin 7
        //(41)
        {PORTA, 6, PIO_PWM, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM), No_ADC_Channel, PWM1_CH0, NOT_ON_TIMER, EXTERNAL_INT_NONE},      // Pin 8
        //(42)
        {PORTA, 7, PIO_PWM, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM), No_ADC_Channel, PWM1_CH1, NOT_ON_TIMER, EXTERNAL_INT_NONE},      // Pin 9

        // 42 (AREF)
        //(43)
        {PORTA, 3, PIO_ANALOG, PIN_ATTR_ANALOG, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE}, // DAC/VREFP

        // ----------------------
        // 43..45 - Alternate use of A0 (DAC output), 44 SWCLK, 45, SWDIO
        //(44)
        {PORTA, 2, PIO_ANALOG, PIN_ATTR_ANALOG, DAC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2},            // DAC/VOUT
        //(45)
        {PORTA, 30, PIO_PWM, PIN_ATTR_DIGITAL | PIO_SERCOM, No_ADC_Channel, NOT_ON_PWM, TCC1_CH0, EXTERNAL_INT_10}, // SWCLK
        //(46)
        {PORTA, 31, PIO_PWM, PIN_ATTR_DIGITAL | PIO_SERCOM, No_ADC_Channel, NOT_ON_PWM, TCC1_CH1, EXTERNAL_INT_11}, // SWDIO

	// Samd21j15a pins

	// //(47)
  // {PORTB, 4, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel12, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_4},
	// //(48)
  // {PORTB, 5, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel13, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_5},
	// //(49)
  // {PORTB, 6, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel14, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_6},
	// //(50)
  // {PORTB, 7, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel15, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_7},
	// //(51)
  // {PORTB, 0, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel8, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_0},
	// //(52)
  // {PORTB, 1, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel9, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_1},
	// //(53)
  // {PORTB, 2, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel10, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2},
	// //(54)
  // {PORTB, 3, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel11, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_3},
	// //(55)
  // {PORTB, 12, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER},
	// //(56)
  // {PORTB, 13, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER},
	// //(57)
  // {PORTB, 14, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER},
	// //(58)
  // {PORTB, 15, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER},
	// //(59)
  // {PORTB, 16, PIO_TIMER_ALT, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER_ALT), No_ADC_Channel, PWM0_CH0, TCC0_CH0, EXTERNAL_INT_0},
	// //(60)
  // {PORTB, 17, PIO_TIMER_ALT, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER_ALT), No_ADC_Channel, PWM0_CH1, TCC0_CH1, EXTERNAL_INT_1},
	// //(61)
  // {PORTB, 22, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER},
	// //(62)
  // {PORTB, 23, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER},
	// //(63)
  // {PORTB, 30, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER},
	// //(64)
  // {PORTB, 31, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER},     

};

extern "C"
{
  unsigned int PINCOUNT_fn()
  {
    return (sizeof(g_APinDescription) / sizeof(g_APinDescription[0]));
  }
}

const void *g_apTCInstances[TCC_INST_NUM + TC_INST_NUM] = {TCC0, TCC1, TCC2, TC3, TC4, TC5};

// Multi-serial objects instantiation
SERCOM sercom0(SERCOM0);
SERCOM sercom1(SERCOM1);
SERCOM sercom2(SERCOM2);
SERCOM sercom3(SERCOM3);
SERCOM sercom4(SERCOM4);
SERCOM sercom5(SERCOM5);

Uart Serial1(&sercom0, PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX);
Uart Serial(&sercom5, PIN_SERIAL_RX, PIN_SERIAL_TX, PAD_SERIAL_RX, PAD_SERIAL_TX);
Uart Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
Uart Serial3(&sercom2, 3, 4, SERCOM_RX_PAD_1, UART_TX_PAD_0);

/// ============== Test Area
// #include <webPagesEEPROMSetting.h>
/**
 * SERCOM 0 => Serial 1
 * SERCOM 5 => Serial 0
 * SERCOM 1 => Serial 2 : (Pins 10 & 11 )
 * SERCOM 2 => Serial 3 : (Pins 3 & 4 )
 * SERCOM 4 => SPI
 * SERCOM 3 => I2C
 */

// ==============

void SERCOM0_Handler()
{
  Serial1.IrqHandler();
}
void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}
void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}
void SERCOM5_Handler()
{
  Serial.IrqHandler();
}


