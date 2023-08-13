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
  void PMMModBUSRTUServerSetup(int16_t SlaveID, uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPortNumber = 1);
  void PMMModBUSRTUServerconfigure(
      bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
      bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
      bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
      bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity);

  int PMMModBUSRTUServercoilReadOneValue(int Address);
  int PMMModBUSRTUServerdiscreteInputReadOneValue(int address);
  long PMMModBUSRTUServerholdingRegisterReadOneValue(int address);
  long PMMModBUSRTUServerinputRegisterReadOneValue(int address);
  void PMMModBUSRTUServercoilWrite(int address, uint8_t value);
  void PMMModBUSRTUServerdiscreteInputWrite(int address, uint8_t value);
  void PMMModBUSRTUServerholdingRegisterWrite(int address, uint16_t value);
  void PMMModBUSRTUServerinputRegisterWrite(int address, uint16_t value);
  void ModbusServersUpdate(int serial);

  // RTU Client
  void PMMModBUSRTUClientSetup(uint16_t Config, int16_t BaudRate, int16_t TXPin, int16_t RXPin, int16_t SerialSelectionPin, int8_t SerialPortNumber = 1);
  int PMMModBUSRTUClientcoilRead(int SlaveID, int address);
  int PMMModBUSRTUClientdiscreteInputRead(int SlaveID, int address);
  long PMMModBUSRTUClientholdingRegisterRead(int SlaveID, int address);
  long PMMModBUSRTUClientinputRegisterRead(int SlaveID, int address);
  void PMMModBUSRTUClientcoilWrite(int SlaveID, int address, uint8_t value);
  void PMMModBUSRTUClientdiscreteInputWrite(int SlaveID, int address, uint8_t value);
  void PMMModBUSRTUClientholdingRegisterWrite(int SlaveID, int address, uint16_t value);
  void PMMModBUSRTUClientinputRegisterWrite(int SlaveID, int address, uint16_t value);



  // TCP SERVER


    void PMMmodbusTCPServerSetup(uint8_t *MACAddress, IPAddress IpAddress, int16_t ETHPORT, int16_t SlaveID);
    void PMMmodbusTCPServerSetup( int16_t ETHPORT, int16_t SlaveID);

    void PMMmodbusTCPServerconfigure(bool Coils, int16_t CoilsStartAddress, int16_t CoilsQauntity,
                                            bool InputRegisters, int16_t InputRegistersStartAddress, int16_t InputRegistersQauntity,
                                            bool HoldingRegisters, int16_t HoldingRegistersStartAddress, int16_t HoldingRegistersQauntity,
                                            bool DiscreteInputs, int16_t DiscreteInputsStartAddress, int16_t DiscreteInputsQauntity);   

    int PMMmodbusTCPServercoilRead(int address);
    int PMMmodbusTCPServerdiscreteInputRead(int address);
    long PMMmodbusTCPServerholdingRegisterRead(int address);
    long PMMmodbusTCPServerinputRegisterRead(int address);
    void PMMmodbusTCPServercoilWrite(int address, uint8_t value);
    void PMMmodbusTCPServerdiscreteInputWrite(int address, uint8_t value);
    void PMMmodbusTCPServerholdingRegisterWrite(int address, uint16_t value);
    void PMMmodbusTCPServerinputRegisterWrite(int address, uint16_t value);


  // TCP Client


};

//extern PmmModBus PmmModbus;

#endif
