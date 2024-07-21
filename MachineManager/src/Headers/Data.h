#pragma once

#include <ArduinoJson.h>
#include "../Configuration.h"

class Data
{
    public:
        String dataSource;
        boolean operating;
        unsigned long int operationTime;
        unsigned long int motorSpeed1;
        unsigned long int motorSpeed2;

        // receive and send data to webpage
        void dataFromJson(String message);
        String sendJsonDataServer();

        // create log
        void printData();
};