#include <Arduino.h>
#include "../Configuration.h"
#include "./Headers/Servidor.h"
#include "./Headers/EspNowCallbacks.h"
#include "./Headers/Index.h"

#if MACHINE_SERVER == true
  Servidor* server = new Servidor(Index::getIndexPage());
  EspNowCallbacks* espnow = new EspNowCallbacks();

  void setup(void)
  {
    Serial.begin(BAUDRATE);
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
#else
  EspNowCallbacks* espnow = new EspNowCallbacks();

  void setup(void)
  {
    Serial.begin(BAUDRATE);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    
    espnow->initialize();
  }

  void loop(void)
  {
    Serial.println("Outra maquina seleciuonada");
    delay(1000);
  }
#endif

