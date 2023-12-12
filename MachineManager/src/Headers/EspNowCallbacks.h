#pragma once

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
#include <espnow.h>

#include "../Configuration.h"
#include "MachineConstants.h"
#include "Data.h"

class EspNowCallbacks
{
    public:
        static Data incomingReadings;
        static boolean dataUpdated;

        static void initialize();
        static void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len);
        static void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);

        static void sendMessage(const uint8_t *machine, Data &data);

        boolean static updateData(Data *data);
};