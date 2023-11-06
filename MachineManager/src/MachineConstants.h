#pragma once

#include <ArduinoJson.h>
#include "../Configuration.h"

class Machines
{
    public:
        static const uint8_t server[];
        static const uint8_t mixer[];

        static char* getMachineAddress(const uint8_t* mac_addr)
        {
			static char macStr[18];
			
			snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
				   mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
			
			return macStr;
		}
};

class Data
{
    public:
        int led1;
        int led2;
};
