#include "../Headers/Data.h"

void Data::dataFromJson(String message)
{
    DynamicJsonDocument doc(200);
    DeserializationError error = deserializeJson(doc, message);
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Object (named doc) in struct format becomes available with data received
    // Used only when server receives data from webpage
    dataSource = String(doc["DATA_SOURCE"]);
    operating = doc["OPERATING"];
    operationTime = doc["OPERATION_TIME"];
    motorSpeed1 = doc["MOTOR_SPEED1"];
    motorSpeed2 = doc["MOTOR_SPEED2"];

    // Serial.println("DEserialized Data: ");
    // printData();
}

String Data::sendJsonDataServer()
{
    DynamicJsonDocument doc(200);
    doc["DATA_SOURCE"] = dataSource;
    doc["OPERATING"] = operating;
    doc["OPERATION_TIME"] = operationTime;
    doc["MOTOR_SPEED1"] = motorSpeed1;
    doc["MOTOR_SPEED2"] = motorSpeed2;

    String msg;
    serializeJson(doc, msg);
    Serial.println("serialized Data: ");
    Serial.println(msg);
    return msg;
}

void Data::printData()
{
    String msg =
    "Data source: " + String(dataSource) +
    " operating: " + String(operating) +
    " operation time: " + String(operationTime) +
    " motorSpeed1: " + String(motorSpeed1) +
    " motorSpeed2: " + String(motorSpeed2);

    Serial.println(msg);
}