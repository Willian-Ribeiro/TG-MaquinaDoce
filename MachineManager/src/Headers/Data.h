#pragma once

#include <ArduinoJson.h>
#include "../Configuration.h"

class Data
{
    public:
        unsigned long int operationTime;
        unsigned long int motorSpeed;

        void dataFromJson(String message);
        String sendJsonDataServer();
};