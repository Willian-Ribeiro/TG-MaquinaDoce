#include <Arduino.h>
#include <ArduinoJson.h>

#include "../Configuration.h"
#include "MachineConstants.h"
#include "Servidor.h"
#include "EspNowCallbacks.h"

Servidor* server = new Servidor();
EspNowCallbacks* espnow = new EspNowCallbacks();

void setup(void)
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);

  server->initialize();
  espnow->initialize();
  
}


void loop(void)
{
  server->websockets.loop();
}
