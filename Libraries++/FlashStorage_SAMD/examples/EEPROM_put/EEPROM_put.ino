void setup(void) {
  pinMode(A1, INPUT_PULLUP);                      // Intialise button input pin and activate internal pull-up resistor
  pinMode(LED_BUILTIN, OUTPUT);                   // Initialise the LED_BUILTIN output
  attachInterrupt(A1, dummyFunc, LOW);                 // Activate a LOW level interrupt on the button pin
  NVMCTRL->CTRLB.bit.SLEEPPRM = NVMCTRL_CTRLB_SLEEPPRM_DISABLED_Val;    // Prevent the flash memory from powering down in sleep mode
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;              // Select standby sleep mode
  SerialUSB.begin(115200);                        // Intialise the native USB port
  while (!SerialUSB);                             // Wait for the console to open
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);                 // Turn off the LED
  SerialUSB.println(F("Sleeping Zzzz...wait for button to wake"));  // Send sleep message to the console
  USBDevice.detach();                             // Detach the native USB port
  __DSB();                                        // Ensure remaining memory accesses are complete
  __WFI();                                        // Enter sleep mode and Wait For Interrupt (WFI)
  USBDevice.attach();                             // Re-attach the native USB port
  delay(500);                                     // Wait for half a second (seems to be necessary to give time for the USB port to re-attach)
  while(!SerialUSB);                              // Wait for the console to re-open
  SerialUSB.println();                            // Add a newline
  SerialUSB.println(F("Button depress...waking up")); // Send a wake up message
  digitalWrite(LED_BUILTIN, LOW);                 // Turn on the LED
  delay(500);                                     // Wait half a second
}

void dummyFunc() {}