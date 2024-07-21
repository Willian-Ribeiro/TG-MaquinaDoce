#pragma once

#include <AccelStepper.h>

#include "./Headers/EspNowCallbacks.h"

class ModelerMain
{
    private:
        // extruder
        boolean extruderOn = false;
        int extruderSpeed = EXTRUDER_SPEED;
        boolean highLow = false;
        unsigned int _skipCounterExtruder = 0;
        
        // Molding Block
        boolean moldingBlockOn = false;
        boolean calibrateMoldingBlockMotors = true;
        boolean _motorsON = false;
        int _moldingBlockM1MoveDirection = -1; // used internally only
        unsigned int _skipCounterMB = 0;
        // M1
        AccelStepper stepperMoldingBlockM1;
        int moldingBlockM1Speed = MB_EOC_CALIBRATION_SPEED;

        Data* myData = new Data();
        static EspNowCallbacks espnow;

    public:
        void setupModeler();
        void loopModeler();

        void calibrate();

        void runMotors();
        void runExtruderMotor();
        void runMoldingBlockMotors();

        void stopMotors();
        void stopExtruderMotor();
        void stopMoldingBlockM1();

        void updateDataAndSend();
};