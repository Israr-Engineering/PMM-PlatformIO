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
#include "PmmModbusRTUServer.h"
#include "PmmModbusTCPClient.h"
#include "PmmModbusTCPServer.h"
#include "PmmEthernet.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


class PmmModBus
{
public:
  bool RCoils[128];              // 64 word,int16 strating from 0x000 to 0x9999
  bool DiscretetInputs[128];    // 64 word,int16 strating from 0x10000 to 0x1999
  int16_t HoldingRegisters[96]; // strating from 0x40000 to 0x4999
  int16_t InputRegisters[96];   // strating from  0x30000 to 0x3999

  PmmModBus();
  virtual ~PmmModBus();

// RTU Server
  void RtuServerStart();
  void PmmModBusRTUServerSetup(int16_t SlaveID, uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPortNumber = 1);
  void PmmModBusRTUServerconfigure(
      bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
      bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
      bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
      bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity);

  int PmmModBusRTUServerCoilRead(int Address);
  int PmmModBusRTUServerDiscreteRead(int address);
  long PmmModBusRTUServerHoldingRead(int address);
  long PmmModBusRTUServerInputRead(int address);
  void PMMModBUSRTUServercoilWrite(int address, uint8_t value);
  void PmmModBusRTUServerDiscreteWrite(int address, uint8_t value);
  void PmmModBusRTUServerHoldingWrite(int address, uint16_t value);
  void PmmModBusRTUServerInputWrite(int address, uint16_t value);
  void ModbusServersUpdate(int serial);

  // RTU Client
  void PmmModBusRTUClientSetup(uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPortNumber = 1);
  int PmmModBusRTUClientCoilRead(int SlaveID, int address);
  int PmmModBusRTUClientDiscreteRead(int SlaveID, int address);
  long PmmModBusRTUClientHoldingRead(int SlaveID, int address);
  long PmmModBusRTUClientInputRead(int SlaveID, int address);
  void PmmModBusRTUClientCoilWrite(int SlaveID, int address, uint8_t value);
  void PmmModBusRTUClientDiscreteWrite(int SlaveID, int address, uint8_t value);
  void PmmModBusRTUClientHoldingWrite(int SlaveID, int address, uint16_t value);
  void PmmModBusRTUClientInputWrite(int SlaveID, int address, uint16_t value);



  // TCP SERVER


    void PmmModBusTCPServerSetup(uint8_t *MACAddress, IPAddress IpAddress, int16_t ETHPORT, int16_t SlaveID);
    void PmmModBusTCPServerSetup( int16_t ETHPORT, int16_t SlaveID);
    void PmmModBusTCPServerConfigure(bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
                                            bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
                                            bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
                                            bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity);   

    int PmmModBusTCPServerCoilRead(int address);
    int PmmModBusTCPServerDiscreteRead(int address);
    long PmmModBusTCPServerHoldingRead(int address);
    long PmmModBusTCPServerInputRead(int address);
    void PmmModBusTCPServerCoilWrite(int address, uint8_t value);
    void PmmModBusTCPServerDiscreteWrite(int address, uint8_t value);
    void PmmModBusTCPServerHoldingWrite(int address, uint16_t value);
    void PmmModBusTCPServerInputWrite(int address, uint16_t value);


  // TCP Client
    
  void PmmModBusTCPClientSetup(IPAddress ip, uint16_t port = 502);
  int PmmModBusTCPClientCoilRead(/*IPAddress ip, uint16_t port,*/int SlaveID, int address);
  int PmmModBusTCPClientDiscreteRead(/*IPAddress ip, uint16_t port,*/int SlaveID, int address);
  long PmmModBusTCPClientHoldingRead(/*IPAddress ip, uint16_t port,*/int SlaveID, int address);
  long PmmModBusTCPClientInputRead(/*IPAddress ip, uint16_t port,*/int SlaveID, int address);
  void PmmModBusTCPClientCoilWrite(/*IPAddress ip, uint16_t port,*/int SlaveID, int address, uint8_t value);
  void PmmModBusTCPClientDiscreteWrite(/*IPAddress ip, uint16_t port,*/int SlaveID, int address, uint8_t value);
  void PmmModBusTCPClientHoldingWrite(/*IPAddress ip, uint16_t port,*/int SlaveID, int address, uint16_t value);
  void PmmModBusTCPClientInputWrite(int SlaveID, int address, uint16_t value);
    


};

//extern PmmModBus PmmModbus;

#endif
