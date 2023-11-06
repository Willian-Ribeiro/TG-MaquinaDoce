#pragma once

#include <Arduino.h>
#include <espnow.h>

#include "../Configuration.h"
#include "MachineConstants.h"

class EspNowCallbacks
{
    public:
        static Data incomingReadings;

        static void initialize();
        static void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len);
        static void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
};