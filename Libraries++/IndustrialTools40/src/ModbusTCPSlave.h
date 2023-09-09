#ifndef __Modbus_ModbusTCPSlave_H__
#define __Modbus_ModbusTCPSlave_H__

#include "ModbusTCP.h"
#include "ModbusSlave.h"

#if defined(ethernet_h) || defined(ethernet_h_)

class ModbusTCPSlave : public ModbusSlave {
	public:
		explicit ModbusTCPSlave(uint16_t port = 502);

	public:
		void begin();

		virtual void update();

	private:
		bool receiveRequest();
		bool sendResponse();

	private:
		// State properties
		EthernetServer _server;
		EthernetClient _currentClient;
		uint32_t _lastRequestTime;

		uint8_t _adu[MODBUS_TCP_ADU_SIZE];
};

#endif

#endif
