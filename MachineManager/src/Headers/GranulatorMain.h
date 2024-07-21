#pragma once

#include "./Headers/EspNowCallbacks.h"

class GranulatorMain
{
    private:
        boolean _dataUpdated = false;

        // Rotation blades
        boolean rotationBladesOn = false;
        int rotationBladesSpeed = ROTATING_BLADES_MIN_SPEED;
        
        // Mat motors
        boolean matMotorsOn = false;
        int matMotorsSpeed = MAT_MOTORS_MIN_SPEED;

        Data* myData = new Data();
        static EspNowCallbacks espnow;

    public:
        void setupGranulator();
        void loopGranulator();

        void setRotatingBladesSpeed();
        void setMatMotorsSpeed();

        void offRotatingBlades();
        void offMatMotors();

        void updateDataAndSend();
};