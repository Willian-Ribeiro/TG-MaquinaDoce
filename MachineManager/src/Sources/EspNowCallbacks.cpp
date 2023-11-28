#include "../Headers/EspNowCallbacks.h"

Data EspNowCallbacks::incomingReadings = Data();

void EspNowCallbacks::initialize()
{
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

    esp_now_add_peer((uint8_t *)&Machines::mixer, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

// callback function that will be executed when data is received
void EspNowCallbacks::OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len)
{

    Serial.println(Machines::getMachineAddress(mac_addr));

    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

    digitalWrite(LED2, incomingReadings.led1);

    Serial.printf("LED1 received value: %d \n", incomingReadings.led1);
    Serial.println();
}

// Callback when data is sent
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

void EspNowCallbacks::sendMessage(const uint8_t *machine, Data &data)
{
    esp_now_send((uint8_t *) machine, (uint8_t *) &data, sizeof(data));
    Serial.println("Assync package sent");
}
