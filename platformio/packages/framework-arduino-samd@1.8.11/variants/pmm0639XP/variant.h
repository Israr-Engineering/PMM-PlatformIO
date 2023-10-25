/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_ARDUINO_ZERO_
#define _VARIANT_ARDUINO_ZERO_

// The definitions here needs a SAMD core >=1.6.10
#define ARDUINO_SAMD_VARIANT_COMPLIANCE 10610

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		(32768ul)

/** Master clock frequency */
#define VARIANT_MCK			  (48000000ul)

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
#include "SERCOM.h"
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
#endif
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (20u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (1u)
#define analogInputToDigitalPin(p)  ((p < 6u) ? (p) + 14u : -1)

#define digitalPinToPort(P)        ( &(PORT->Group[g_APinDescription[P].ulPort]) )
#define digitalPinToBitMask(P)     ( 1 << g_APinDescription[P].ulPin )
//#define analogInPinToBit(P)        ( )
#define portOutputRegister(port)   ( &(port->OUT.reg) )
#define portInputRegister(port)    ( &(port->IN.reg) )
#define portModeRegister(port)     ( &(port->DIR.reg) )
#define digitalPinHasPWM(P)        ( g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER )

/*
 * digitalPinToTimer(..) is AVR-specific and is not defined for SAMD
 * architecture. If you need to check if a pin supports PWM you must
 * use digitalPinHasPWM(..).
 *
 * https://github.com/arduino/Arduino/issues/1833
 */
// #define digitalPinToTimer(P)

// LEDs
#define PIN_LED_13           (13u)
#define PIN_LED_RXL          (25u)
#define PIN_LED_TXL          (26u)
#define PIN_LED              PIN_LED_13
#define PIN_LED2             PIN_LED_RXL
#define PIN_LED3             PIN_LED_TXL
#define LED_BUILTIN          PIN_LED_13


 /*
 * Digital pins
 */
#define PMM_DI0          (52ul) //PB05 
#define PMM_DI1          (33ul) //PA23
#define PMM_DI2          (51ul) //PB04 
#define PMM_DI3          (19Ul) //PB02 
#define PMM_DI4          (14ul) //PA02 
#define PMM_DI5          (53ul) //PB06 
#define PMM_DI6          (50ul) //PB03 
#define PMM_DI7          (26ul) //PA27 
#define PMM_DI8          (54ul) //PB07 
#define PMM_DI9          (42Ul) //PA03 
#define PMM_DI10         (62ul) //PB30 
#define PMM_DI11         (49ul) //PB01 

#define PMM_DO0          (63ul)  //PB31 
#define PMM_DO1          (48ul) //PB00
#define PMM_DO2          (54ul) //PA08 
#define PMM_DO3          (4ul) //PA09
#define PMM_DO4          (1ul)  //PA10 
#define PMM_DO5          (0ul)  //PA11 

 /*
 * Analog pins
 */
#define PMM_A0           (16ul)  //PB09 
#define PMM_A1           (17ul) //PA04 
#define PMM_A2           (15ul)  //PB08 
#define PMM_A3           (18ul) //PA05

#define PMM_PWM0         (3ul) //PA07
#define PMM_PWM1         (9ul) //PA06 

#define PMM_RUN          (30ul) //PB22
#define PMM_FAULT        (7ul) //PA21
//02. Dip switches 
#define PMM_DI_PROG          (59ul)//PB15 : Digital Input for programming mode - near termination dip switch
#define PMM_DI_PROG01        (22ul) //PA12 : Digital Input - general (front panel dip switch )
#define PMM_DI_PROG02        (38ul) //PA13 : Digital Input - general (front panel dip switch)
//03. Internal signals
//03a. Ethernet controller 
//#define PMM_DO_ETH_RST     (16u) //PB09 : Digital output : Ethernet controller reset command
//#define PMM_D1_ETH_INT     (17u) //PA04 : Digital Input : Ethernet controller interrupt 
//#define PMM_DO_SCSn        (10u) //PA18 : Digital output : Ethernet controller chip select command  
//03b. Flash memory
#define PMM_DO_FLASH_EN      (58ul) //PB14 : Digital output : Flash Enable command
#define PMM_DO_FLASH_CS      (57ul) //PB13 : Digital output : Flash chip select command 
//03c. Serial RS485
//#define PMM_DO_RDENA       (59ul)//PB15 : Digital output : RS485 Direction control  
//03d. options 
#define PMM_DI_INT           (2ul)//PA14 : Digital Input : General interrupt from RTC or Extension boards  
//#define PMM_DI_LossOfPower        (11u)//PA02 : Digital Input :loos of power 

static const uint8_t DI0  = PMM_DI0;
static const uint8_t DI1  = PMM_DI1;
static const uint8_t DI2  = PMM_DI2;
static const uint8_t DI3  = PMM_DI3;
static const uint8_t DI4  = PMM_DI4;
static const uint8_t DI5  = PMM_DI5;
static const uint8_t DI6  = PMM_DI6;
static const uint8_t DI7  = PMM_DI7;
static const uint8_t DI8  = PMM_DI8;
static const uint8_t DI9  = PMM_DI9;
static const uint8_t DI10 = PMM_DI10;
static const uint8_t DI11 = PMM_DI11;

static const uint8_t DO0  = PMM_DO0;
static const uint8_t DO1  = PMM_DO1;
static const uint8_t DO2  = PMM_DO2;
static const uint8_t DO3  = PMM_DO3;
static const uint8_t DO4  = PMM_DO4;
static const uint8_t DO5  = PMM_DO5;


static const uint8_t A0   = PMM_A0;
static const uint8_t A1   = PMM_A1;
static const uint8_t A2   = PMM_A2;
static const uint8_t A3   = PMM_A3;

static const uint8_t PWM0 = PMM_PWM0;
static const uint8_t PWM1 = PMM_PWM1;


//static const uint8_t DAC0 = PMM_DAC0;
//#define ADC_RESOLUTION		12

// Other pins
#define PIN_ATN              (38ul)
static const uint8_t ATN = PIN_ATN;

/*
 * Serial interfaces
 */
// Serial (EDBG)
#define PIN_SERIAL_RX       (31ul)
#define PIN_SERIAL_TX       (30ul)
#define PAD_SERIAL_TX       (UART_TX_PAD_2)
#define PAD_SERIAL_RX       (SERCOM_RX_PAD_3)

// Serial1
#define PIN_SERIAL1_RX       (0ul)
#define PIN_SERIAL1_TX       (1ul)
#define PAD_SERIAL1_TX       (UART_TX_PAD_2)
#define PAD_SERIAL1_RX       (SERCOM_RX_PAD_3)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO         (22u)
#define PIN_SPI_MOSI         (23u)
#define PIN_SPI_SCK          (24u)
#define PERIPH_SPI           sercom4
#define PAD_SPI_TX           SPI_PAD_2_SCK_3
#define PAD_SPI_RX           SERCOM_RX_PAD_0

static const uint8_t SS	  = PMM_A2 ;	// SERCOM4 last PAD is present on A2 but HW SS isn't used. Set here only for reference.
static const uint8_t MOSI = PIN_SPI_MOSI ;
static const uint8_t MISO = PIN_SPI_MISO ;
static const uint8_t SCK  = PIN_SPI_SCK ;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (20u)
#define PIN_WIRE_SCL         (21u)
#define PERIPH_WIRE          sercom3
#define WIRE_IT_HANDLER      SERCOM3_Handler

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

/*
 * USB
 */
#define PIN_USB_HOST_ENABLE (27ul)
#define PIN_USB_DM          (28ul)
#define PIN_USB_DP          (29ul)

/*
 * I2S Interfaces
 */
#define I2S_INTERFACES_COUNT 1

#define I2S_DEVICE          0
#define I2S_CLOCK_GENERATOR 3
#define PIN_I2S_SD          (9u)
#define PIN_I2S_SCK         (1u)
#define PIN_I2S_FS          (0u)

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

/*	=========================
 *	===== SERCOM DEFINITION
 *	=========================
*/
extern SERCOM sercom0;
extern SERCOM sercom1;
extern SERCOM sercom2;
extern SERCOM sercom3;
extern SERCOM sercom4;
extern SERCOM sercom5;

extern Uart Serial;
extern Uart Serial1;

#endif

#ifdef __cplusplus
extern "C" {
#endif
unsigned int PINCOUNT_fn();
#ifdef __cplusplus
}
#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_USBVIRTUAL      SerialUSB
#define SERIAL_PORT_MONITOR         Serial
// Serial has no physical pins broken out, so it's not listed as HARDWARE port
#define SERIAL_PORT_HARDWARE        Serial1
#define SERIAL_PORT_HARDWARE_OPEN   Serial1

#endif /* _VARIANT_ARDUINO_ZERO_ */

