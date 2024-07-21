#include "../Headers/GranulatorMain.h"

EspNowCallbacks GranulatorMain::espnow = EspNowCallbacks();

void GranulatorMain::setupGranulator(void)
{
    Serial.begin(BAUDRATE);

    // Limit Switch pinout config
    pinMode(ROTATING_BLADES_PIN, OUTPUT);
    pinMode(MAT_MOTORS_PIN, OUTPUT);


    // ESP NOW communication
    EspNowCallbacks::initialize();
    myData->dataSource = DATA_SOURCE_GRANULATOR;
}

void GranulatorMain::loopGranulator()
{
    // update machine operation state data
    if(EspNowCallbacks::updateData(myData))
    {
        // update extruder state
        rotationBladesOn = myData->operating;
        rotationBladesSpeed = constrain(myData->motorSpeed1, ROTATING_BLADES_MIN_SPEED, ROTATING_BLADES_MAX_SPEED);

        // update molding block state
        matMotorsOn = myData->operating;
        matMotorsSpeed = constrain(myData->motorSpeed2, MAT_MOTORS_MIN_SPEED, MAT_MOTORS_MAX_SPEED);

        _dataUpdated = true;
    }

    if(_dataUpdated)
    {
        if(rotationBladesOn)
            setRotatingBladesSpeed();
        else
            offRotatingBlades();
            
        if(matMotorsOn)
            setMatMotorsSpeed();
        else
            offMatMotors();
        
        _dataUpdated = false;
    }
}

void GranulatorMain::setRotatingBladesSpeed()
{
    int mappedSpeed = map(rotationBladesSpeed, ROTATING_BLADES_MIN_SPEED, ROTATING_BLADES_MAX_SPEED, PWM_MIN_SPEED, PWM_MAX_SPEED);
    analogWrite(ROTATING_BLADES_PIN, mappedSpeed);
}

void GranulatorMain::setMatMotorsSpeed()
{
    int mappedSpeed = map(matMotorsSpeed, MAT_MOTORS_MIN_SPEED, MAT_MOTORS_MAX_SPEED, PWM_MIN_SPEED, PWM_MAX_SPEED);
    analogWrite(MAT_MOTORS_PIN, mappedSpeed);
}

void GranulatorMain::offRotatingBlades()
{
    rotationBladesSpeed = ROTATING_BLADES_MIN_SPEED;
    analogWrite(ROTATING_BLADES_PIN, rotationBladesSpeed);
}

void GranulatorMain::offMatMotors()
{
    matMotorsSpeed = MAT_MOTORS_MIN_SPEED;
    analogWrite(MAT_MOTORS_PIN, matMotorsSpeed);
}

void GranulatorMain::updateDataAndSend()
{
    myData->operating = rotationBladesOn && matMotorsOn;
    myData->motorSpeed1 = rotationBladesSpeed;
    myData->motorSpeed2 = matMotorsSpeed;

    EspNowCallbacks::sendMessage(Machines::server, *myData);
}
