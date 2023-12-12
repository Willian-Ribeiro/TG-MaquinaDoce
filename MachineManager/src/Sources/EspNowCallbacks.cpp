#include "../Headers/EspNowCallbacks.h"

Data EspNowCallbacks::incomingReadings = Data();
boolean EspNowCallbacks::dataUpdated = false;

void EspNowCallbacks::initialize()
{
    // Set ESP as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(EspNowCallbacks::OnDataRecv);
    esp_now_register_send_cb(EspNowCallbacks::OnDataSent);

    if(!MACHINE_SERVER)
        esp_now_add_peer((uint8_t *)&Machines::server, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    if(!MACHINE_MIXER)
        esp_now_add_peer((uint8_t *)&Machines::mixer, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    // if(!MACHINE_MODELER)
    //     esp_now_add_peer((uint8_t *)&Machines::modeler, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    // if(!MACHINE_PACKER)
    //     esp_now_add_peer((uint8_t *)&Machines::packer, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    dataUpdated = false;
}

// callback function that will be executed when data is received
void EspNowCallbacks::OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len)
{
    Serial.println(Machines::getMachineAddress(mac_addr));

    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
    dataUpdated = true;

    Serial.printf("NOW OPERATION_TIME received value: %d \n", incomingReadings.operationTime);
    Serial.printf("NOW MOTOR_SPEED received value: %d \n", incomingReadings.motorSpeed);
    Serial.println();
}

void EspNowCallbacks::sendMessage(const uint8_t *machine, Data &data)
{
    esp_now_send((uint8_t *) machine, (uint8_t *) &data, sizeof(data));
    Serial.println("Assync package sent");
    Serial.printf("NOW OPERATION_TIME sent value: %d \n", data.operationTime);
    Serial.printf("NOW MOTOR_SPEED sent value: %d \n", data.motorSpeed);
    Serial.println();
}

// Callback after sendMessage is executed
void EspNowCallbacks::OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0)
    {
        Serial.println("Delivery success");
    }
    else
    {
        Serial.println("Delivery fail");
    }
}

boolean EspNowCallbacks::updateData(Data *data)
{
    if(dataUpdated)
    {
        Serial.println("Updating myData");
        // memcpy(data, &incomingReadings, sizeof(data)); // discover why not working with motorSpeed
        dataUpdated = false;

        data->operationTime = incomingReadings.operationTime;
        data->motorSpeed = incomingReadings.motorSpeed;

        Serial.printf("Updated time value: %d \n", data->operationTime);
        Serial.printf("updated speed value: %d \n", data->motorSpeed);

        return true;
    }
    return false;
}