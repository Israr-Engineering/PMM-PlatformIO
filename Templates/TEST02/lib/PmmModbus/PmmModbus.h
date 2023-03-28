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

   

      bool Coils[128]; // 64 word,int16 strating from 0x000 to 0x9999
      bool DiscretetInputs[128]; // 64 word,int16 strating from 0x10000 to 0x1999
      int16_t HoldingRegisters[96]; // strating from 0x40000 to 0x4999
      int16_t InputRegisters[96];  // strating from  0x30000 to 0x3999

       PmmModBus();
		virtual ~PmmModBus();

    // RtuServerStart();



};
 extern  PmmModBus PmmModbus;



#endif
