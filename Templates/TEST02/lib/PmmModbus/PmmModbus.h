/*
  This file is part of the ArduinoModbus library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _ARDUINO_MODBUS_H_INCLUDED
#define _ARDUINO_MODBUS_H_INCLUDED

#include "PmmModbusRTUClient.h"
#include "PMMModBusRTUClientLib.h"
#include "PmmModbusRTUServer.h"
#include "PMMModBusRTUServerLib.h"

#include "PmmModbusTCPClient.h"

#include "PmmModbusTCPServer.h"
#include "PMMModBusTCPServerLib.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

 

class PmmModBus
{
  public:

   

      bool Coils[1024];
      bool DiscretetInputs[1024];
      int16_t HoldingRegisters[128];
      int16_t InputRegisters[128];

       PmmModBus();
		virtual ~PmmModBus();

};
 extern  PmmModBus PmmModbus;



#endif
