#include "../Headers/Data.h"

void Data::dataFromJson(String message)
{
    DynamicJsonDocument doc(200);
    DeserializationError error = deserializeJson(doc, message);
    // parse the parameters we expect to receive (TO-DO: error handling)
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Object (named doc) in struct format becomes available with data received
    led1 = doc["LED1"];
    led2 = doc["LED2"];
}