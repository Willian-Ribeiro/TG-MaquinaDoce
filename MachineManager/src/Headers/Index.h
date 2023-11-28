#pragma once

#include <ArduinoJson.h>

class Index
{
    private:
        static const char webPage[] PROGMEM;
        
    public:
        static const char* getIndexPage();
};