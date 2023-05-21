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

#include <errno.h>

extern "C" {
#include "modbus.h"
#include "modbus-tcp.h"
}

#include "PmmModbusTCPServer.h"

PmmModbusTCPServer::PmmModbusTCPServer() :
  _client(NULL)
{
}

PmmModbusTCPServer::~PmmModbusTCPServer()
{
}

int PmmModbusTCPServer::begin(int id)
{

 
  modbus_t* mb = modbus_new_tcp(NULL, IPAddress(192,168,1,110), 502);

  if (!PmmModbusServer::begin(mb, id)) {
    return 0;
  }

  if (modbus_tcp_listen(mb) != 0) {
    return 0;
  }

  return 1;
}

void PmmModbusTCPServer::accept(Client& client)
{
  if (modbus_tcp_accept(_mb, &client) == 0) {
    _client = &client;
  }
}

int PmmModbusTCPServer::poll()
{
  if (_client != NULL) {
    uint8_t request[MODBUS_TCP_MAX_ADU_LENGTH];

    int requestLength = modbus_receive(_mb, request);

    if (requestLength > 0) {
      modbus_reply(_mb, request, requestLength, &_mbMapping);
      return 1;
    }
  }
  return 0;
}
