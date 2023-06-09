#include <Arduino.h>
#include <ProjectDef.h>

bool x = false;
long Timer1 = 0;
long Timer2 = 0;
long Timer3 = 0;
int StartingAddress = 11;
int Quantity = 10;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 55);

PmmWDTZero PmmWatchDoggy;
PmmModBus PmmModbus;



void setup()
{

// STEP01: Read Flash ROM and update Settings
// STEP02: 
  PmmWatchDoggy.setup(WDT_SOFTCYCLE2M);
  PMMInitializeEthernet(ip, mac);


   PmmModbus.PMMModBUSRTUServerSetup(1, SERIAL_8N1, 9600, 30, 9, 31, 1);
  PmmModbus.PMMModBUSRTUServerconfigure(
       false, 0, 10,
       false, 0, 10,
       true, 0, 10,
       false, 0, 10);

//STEP04: Start General services
  SerialUSB.begin(9600);
  Scheduler.startLoop(PMMConfiguration);
  Scheduler.startLoop(PMMCommunication);
}

//////////////////////////////////////
//Loop 01 :  Main loop start here   //
//////////////////////////////////////

void loop()
{


  PmmWatchDoggy.clear();

  if ((millis() - Timer1) > 500)
  {
    // time_t now = PMMSetAnDatetime(53, 3, 22, 16, 0, 0);
    // SerialUSB.println(SunCalculationsStr(now, 31.5320459, 36.0276305, 3, 4, 11).c_str());
    Timer1 = millis();
  }
}

//////////////////////////////////////////////////
//Loop 02 :  Configuration and commands updating //
//////////////////////////////////////////////////
void PMMConfiguration()
{

  PMMReadCommands();

  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}


//////////////////////////////////////////////////
//Loop 03 : Communication updating loop         //
//////////////////////////////////////////////////
void PMMCommunication()
{


   if ((millis() - Timer2) > 500)
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

     Timer2 = millis();
   }


  // We must call 'yield' at a regular basis to pass control to other tasks.
  yield();
}
