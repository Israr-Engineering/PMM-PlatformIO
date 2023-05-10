#include <Arduino.h>
#include <ProjectDef.h>

#include <PmmCommands.h>
#include <PmmGlobalFunctions.h>

// Include your board here
//#include <PMM0620.h>

void setup()
{
  // STEP00: Start usb serial for debug
  SerialUSB.begin(9600);
  // STEP01: Read Flash ROM and update Settings
   PmmInitializeProjectSettings();
  //  STEP02: Initialize Extra needed Modules
  //ThisProductSetup();

    pinMode(DIPPROG01,INPUT);
    pinMode(DIPPROG02,INPUT);
    pinMode(DILOSSPOWER,INPUT);
     
  // STEP03: Warmup 1 sec
  //SCB_SCR_SLEEPDEEP_Msk
  delay(1000);
  SerialUSB.println("New Starting ...");
  
}

/* ///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                  //
/////////////////////////////////////////////////// */

int x5 = 0;
int val;
 
void loop()
{
  PmmPowerManagerUpdate();

  if ((millis() - MainLoopTimer) > 1000)
  {

    if (x5 >= 5)
    {
      
      
      val =digitalRead(DIPPROG01);
      SerialUSB.println(val);
      val =digitalRead(DIPPROG02);
      SerialUSB.println(val);
      val =digitalRead(DILOSSPOWER);
      SerialUSB.println(val);
      
      if (digitalRead(DIPPROG02) == 1)
      {
        SerialUSB.println("Enter Sleep mode");
        PmmWatchDoggy.setup(0x00);
        PmmRTCInternal.standbyMode();

      } 
    }
    
   
  x5++;
    MainLoopTimer = millis();
  }
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 02 : Configuration and commands updating    //
/////////////////////////////////////////////////// */

void PMMConfiguration()
{
  if ((millis() - ConfigurationTimer) > 500)
  {

    StartCommandHttpServer(); // this Should be here always
    
    PMMReadCommands();
    ConfigurationTimer = millis();
  }
  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 03 : Communication updating loop            //
/////////////////////////////////////////////////// */

void PMMCommunication()
{
  if ((millis() - CommunicationTimer) > ThisProduct.PmmTimers.CommUpdateTimer)
  {
    // Manage Extension Boards
    AllExtensionBoarsdUpdate();
    // Manage Modbus Server
    //ModbusPort[1].ModbusServersUpdate(1);

    CommunicationTimer = millis();
  }
  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

/* ///////////////////////////////////////////////////
// Loop 04 : Timers updating loop                   //
/////////////////////////////////////////////////// */

void PMMTimers()
{
  if ((micros() - CommunicationTimer) > 10000)
  {
    CommunicationTimer = micros();
  }
  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

/* ///////////////////////////////////////////////////
// WATCHDOG: Reset Controller                       //
/////////////////////////////////////////////////// */

// #include <stdbool.h>

// #define USER_BUTTON_PIN 13

// static volatile bool s_button_pressed;

// void setup(void) {
//   pinMode(LED_BUILTIN, OUTPUT);                  // Initialise the LED_BUILTIN output
//   digitalWrite(LED_BUILTIN, LOW);   // Turn on the LED
//   pinMode(USER_BUTTON_PIN, INPUT_PULLUP);        // Intialise button input pin and activate internal pull-up resistor
//   attachInterrupt(USER_BUTTON_PIN, on_button_press, LOW);   // Activate a LOW level interrupt on the button pin
//   NVMCTRL->CTRLB.bit.SLEEPPRM = NVMCTRL_CTRLB_SLEEPPRM_DISABLED_Val;    // Prevent the flash memory from powering down in sleep mode
//   SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;             // Select standby sleep mode
// }

// void loop() {
//   digitalWrite(LED_BUILTIN, HIGH);  // Turn off the LED
//   SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;  // Disable SysTick interrupts
//   __DSB();                          // Ensure remaining memory accesses are complete
//   __WFI();                          // Enter sleep mode and Wait For Interrupt (WFI)
//   SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   // Enable SysTick interrupts  
//   delay(500);                       // Keep LED off long enough to see that we passed the __WFI
//   digitalWrite(LED_BUILTIN, LOW);   // Turn on the LED
//   delay(2000);                      // Wait a bit
// }

// void on_button_press(void) {
//   s_button_pressed = true;
// }