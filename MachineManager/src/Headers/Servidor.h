#pragma once

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif

#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include "EspNowCallbacks.h"

#include "../Configuration.h"
#include "MachineConstants.h"
#include "Data.h"

class Servidor
{
    private:
        static const char* webPage;
        static EspNowCallbacks espnow;

    public:
        static AsyncWebServer server; // server port 80
        static Data myData;
        static WebSocketsServer websockets;

        Servidor(const char* webPage);

        static void loopServer();

        static void notFound(AsyncWebServerRequest *request);

        static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

        static int32_t getWiFiChannel(const char *ssid);

        static void initialize();
        
};
