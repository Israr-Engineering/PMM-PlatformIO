#include <Arduino.h>
#include <ProjectDef.h>





void setup()
{

  // STEP00: Start usb serial for debug 
   SerialUSB.begin(9600);

  // STEP01: Read Flash ROM and update Settings

  ThisProduct.PmmGeneral.DeviceName = 1010;

  // STEP02: Initialize needed Modules MyWatchDoggy.attachShutdown(myshutdown);
  // PmmWatchDoggy.attachShutdown(Pmmshutdown);
  PmmWatchDoggy.setup(WDT_SOFTCYCLE32S);
  PMMInitializeEthernet(ip, mac);

  

  // STEP03: Setup and configure services
  PmmModbus.PMMModBUSRTUServerSetup(1, SERIAL_8N1, 9600, 35, 36, 31, 1);
  PmmModbus.PMMModBUSRTUServerconfigure(false, 0, 10, false, 0, 10, true, 0, 10, false, 0, 10);

  // STEP04: Start General services
  
  Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
  // Scheduler.startLoop(PMMTimers);

  // STEP05: Warmup 2 seconds
  
  SerialUSB.println("New Start");
  // SerialUSB.setTimeout(30000);
  // string CMDResult = PMMReadFromFlashAllSettings();
  // SerialUSB.println(CMDResult.c_str());
}

///////////////////////////////////////////////////
// Loop 01 :  Main loop start here                //
///////////////////////////////////////////////////

int x1 = 0;

void loop()
{

  PmmWatchDoggy.clear();

  

  if ((millis() - MainLoopTimer) > 1000)
  {

    // PmmModbusRTUServer.poll();
    // if (PmmModbusRTUServer.holdingRegisterWrite(0, x1) == 1)
    // {
    //   SerialUSB.println(" Write done");
    //   SerialUSB.println(PmmModbusRTUServer.holdingRegisterRead(0));
    // }
    // else
    // {
    //   SerialUSB.println(" Write fail");
    // }
    // SerialUSB.print(x1);
    // SerialUSB.println(" ..Running...");
    MainLoopTimer = millis();
    x1++;
  }
  yield();
}

///////////////////////////////////////////////////
// Loop 02 : Configuration and commands updating //
///////////////////////////////////////////////////
void PMMConfiguration()
{

  if ((millis() - ConfigurationTimer) > 1000)
  {

    string CMDResult = PMMReadCommands();
    SerialUSB.println(CMDResult.c_str());
    ConfigurationTimer = millis();
  }
  // ConfigurationTimer

  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

///////////////////////////////////////////////////
// Loop 03 : Communication updating loop         //
///////////////////////////////////////////////////
void PMMCommunication()
{

  if ((millis() - CommunicationTimer) > 500)
  {

    //   long* RegisrersValues = PMMModBUSRTUServerholdingRegisterRead(StartingAddress,Quantity);
    //   for (int address = StartingAddress; address < (StartingAddress+Quantity); address++)
    //   {
    //     int index = address - StartingAddress;
    //     //long val = PMMModBUSRTUServerholdingRegisterRead(address);
    //     long val = RegisrersValues[index];
    //     SerialUSB.print("Value Of Address ");
    //     SerialUSB.print(address);
    //     SerialUSB.print(" : ");
    //     SerialUSB.println(val);
    //   }

    CommunicationTimer = millis();
  }

  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

///////////////////////////////////////////////////
// Loop 04 : Timers updating loop                //
///////////////////////////////////////////////////
void PMMTimers()
{

  if ((micros() - CommunicationTimer) > 10000)
  {

    CommunicationTimer = micros();
  }

  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}

///////////////////////////////////////////////////
// WATCHDOG: Reset Controller                    //
///////////////////////////////////////////////////
