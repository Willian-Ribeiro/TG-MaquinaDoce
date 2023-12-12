#include "../Headers/Servidor.h"

AsyncWebServer Servidor::server(80);
WebSocketsServer Servidor::websockets(81);
Data Servidor::myData = Data();
EspNowCallbacks Servidor::espnow = EspNowCallbacks();
const char* Servidor::webPage = "empty";

Servidor::Servidor(const char* webPage)
{
  this->webPage = webPage;
}

void Servidor::loopServer()
{
    if(EspNowCallbacks::updateData(&myData))
    {
        Serial.print("\n\n Data to serialize by function: ");
        Serial.println(myData.motorSpeed);
        websockets.broadcastTXT(myData.sendJsonDataServer().c_str());
    }
}

void Servidor::initialize()
{
    EspNowCallbacks::initialize();

    server.on("/", [](AsyncWebServerRequest * request)
    { 
        request->send_P(200, "text/html", webPage);
    });

    server.onNotFound(notFound);
    
    server.begin();  // it will start webserver
    websockets.begin();
    websockets.onEvent(webSocketEvent);

    // Begin WiFi
    WiFi.softAP("esp_server", "");
    Serial.println("softap");
    Serial.println("");
    Serial.println(WiFi.softAPIP());


    if (MDNS.begin("esp")) { //esp.local/ //
        Serial.println("MDNS responder started");
    }

    int32_t channel = getWiFiChannel(WIFI_SSID);

    WiFi.printDiag(Serial); // Uncomment to verify channel number before
    wifi_promiscuous_enable(1);
    wifi_set_channel(channel);
    wifi_promiscuous_enable(0);
    WiFi.printDiag(Serial); // Uncomment to verify channel change after
}

void Servidor::notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Page Not found");
}

void Servidor::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_ERROR:
        Serial.printf("[%u] Error in websocket!\n", num);
        break;
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    }
    break;
    case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);
        String message = String((char *)(payload));
        Serial.println(message);

        // parse Json to Data object
        myData.dataFromJson(message);

        // Serial.print("myData operation Time ");Serial.print(myData.operationTime);
        // Serial.print("myData motor speed ");Serial.println(myData.motorSpeed);

        EspNowCallbacks::sendMessage(Machines::mixer, myData);
        // EspNowCallbacks::sendMessage(Machines::modeler, myData);
        // EspNowCallbacks::sendMessage(Machines::packer, myData);
    }

}

int32_t Servidor::getWiFiChannel(const char *ssid)
{
    if (int32_t n = WiFi.scanNetworks()) {
        for (uint8_t i=0; i<n; i++)
        {
            if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
                return WiFi.channel(i);
            }
        }
    }
    return 0;
}
