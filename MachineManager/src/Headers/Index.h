#pragma once

#include <ArduinoJson.h>

class Index
{
    private:
        static const char webPage[] PROGMEM;
        static const char webPageUpdated[] PROGMEM;
        
    public:
        static const char* getIndexPage();
};