#include <Arduino.h>
#include "../Configuration.h"
#include "./Headers/EspNowCallbacks.h"
#include "./Headers/Index.h"

#if MACHINE_SERVER
  #include "./Headers/Servidor.h"

  Servidor* server = new Servidor(Index::getIndexPage());

  void setup(void)
  {
    Serial.begin(BAUDRATE);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    
    server->initialize();
  }

  void loop(void)
  {
    server->websockets.loop();

    server->loopServer();
  }
#endif

#if MACHINE_MIXER
  
  #include "./Headers/MixerMain.h"

  MixerMain* mixerMain = new MixerMain();

  void setup(void)
  {
    mixerMain->setupMixer();
  }

  void loop(void)
  {
    mixerMain->loopMixer();
  }
#endif

#if MACHINE_MODELER
  
  #include "./Headers/ModelerMain.h"

  ModelerMain* modelerMain = new ModelerMain();

  void setup(void)
  {
    modelerMain->setupModeler();
  }

  void loop(void)
  {
    modelerMain->loopModeler();
  }
#endif

#if MACHINE_GRANULATOR
  
  #include "./Headers/GranulatorMain.h"

  GranulatorMain* granulatorMain = new GranulatorMain();

  void setup(void)
  {
    granulatorMain->setupGranulator();
  }

  void loop(void)
  {
    granulatorMain->loopGranulator();
  }
#endif
