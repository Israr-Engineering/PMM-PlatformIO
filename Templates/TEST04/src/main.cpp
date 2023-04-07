#include <Arduino.h>
#include <ProjectDef.h>


void setup()
{

// STEP01: Read Flash ROM and update Settings


// STEP02: Initialize needed Modules MyWatchDoggy.attachShutdown(myshutdown);
  //PmmWatchDoggy.attachShutdown(Pmmshutdown);
  PmmWatchDoggy.setup(WDT_SOFTCYCLE16S);
  PMMInitializeEthernet(ip, mac);

// STEP03: Setup and configure services
  PmmModbus.PMMModBUSRTUServerSetup(1, SERIAL_8N1, 9600, 30, 9, 31, 1);
  PmmModbus.PMMModBUSRTUServerconfigure(false, 0, 10,false, 0, 10,true, 0, 10,false, 0, 10);

// STEP04: Start General services
  SerialUSB.begin(9600);
  Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
  Scheduler.startLoop(PMMTimers);
}

///////////////////////////////////////////////////
//Loop 01 :  Main loop start here                //
///////////////////////////////////////////////////

void loop()
{

//MyLoadCell.
  PmmWatchDoggy.clear();

  if ((millis() - MainLoopTimer) > 500)
  {
    // time_t now = PMMSetAnDatetime(53, 3, 22, 16, 0, 0);
    // SerialUSB.println(SunCalculationsStr(now, 31.5320459, 36.0276305, 3, 4, 11).c_str());
    MainLoopTimer = millis();
  }
}

///////////////////////////////////////////////////
//Loop 02 :  Configuration and commands updating //
///////////////////////////////////////////////////
void PMMConfiguration()
{


//ConfigurationTimer

  PMMReadCommands();

  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}


///////////////////////////////////////////////////
//Loop 03 : Communication updating loop          //
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
//Loop 04 : Timers updating loop          //
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
//WATCHDOG: Reset Controller                     //
///////////////////////////////////////////////////

