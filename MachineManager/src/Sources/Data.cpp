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
    // Used only when server receives data from webpage
    operationTime = doc["OPERATION_TIME"];
    motorSpeed = doc["MOTOR_SPEED"];
    Serial.println("DEserialized Data: ");
    String msg = String(operationTime) + " speed: " + String(motorSpeed);
    Serial.println(msg);
}

String Data::sendJsonDataServer()
{
    DynamicJsonDocument doc(200);
    doc["OPERATION_TIME"] = operationTime;
    doc["MOTOR_SPEED"] = motorSpeed;

    // Serial.print("serializeJson() succesfull");

    String msg;
    serializeJson(doc, msg);
    Serial.println("serialized Data: ");
    Serial.println(msg);
    return msg;
}