#pragma once

#include <ArduinoJson.h>

class Data
{
    public:
        int led1;
        int led2;

        void dataFromJson(String message);
};