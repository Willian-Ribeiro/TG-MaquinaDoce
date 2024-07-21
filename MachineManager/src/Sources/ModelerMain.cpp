#include "../Headers/ModelerMain.h"

EspNowCallbacks ModelerMain::espnow = EspNowCallbacks();

void ModelerMain::setupModeler(void)
{
    Serial.begin(BAUDRATE);

    // Limit Switch pinout config
    pinMode(LIMIT_SWITCH, INPUT);

    // extruder - with speed control board
    pinMode (EXTRUDER_STEP, OUTPUT);
    pinMode (EXTRUDER_DIR, OUTPUT);

    digitalWrite(EXTRUDER_DIR, HIGH);

    // molding block - const speed setup for EOC calibration
    stepperMoldingBlockM1 = AccelStepper(AccelStepper::HALF4WIRE, MB_M1_IN1, MB_M1_IN3, MB_M1_IN2, MB_M1_IN4);
    stepperMoldingBlockM1.setMaxSpeed(MB_MAX_SPEED);
    stepperMoldingBlockM1.setAcceleration(MB_ACCELERATION);
    stepperMoldingBlockM1.setSpeed(-MB_EOC_CALIBRATION_SPEED); // positive: CW, negative: Counter CW

    // ESP NOW communication
    EspNowCallbacks::initialize();
    myData->dataSource = DATA_SOURCE_MODELER;
}

void ModelerMain::loopModeler()
{
    // update machine operation state data
    if(EspNowCallbacks::updateData(myData))
    {
        // update extruder state
        extruderOn = myData->operating;
        extruderSpeed = myData->motorSpeed1;

        // update molding block state
        moldingBlockOn = myData->operating;
        moldingBlockM1Speed = myData->motorSpeed2;
    }

    if(calibrateMoldingBlockMotors && extruderOn && moldingBlockOn) // calibrates to find EOC (end of cursor)
    {
        calibrate();
    }
    else if (digitalRead(LIMIT_SWITCH) == LOW || !extruderOn || !moldingBlockOn)
    {
        if(_motorsON)
            stopMotors();
    }
    else
    {
        runMotors();
    }
}

void ModelerMain::calibrate()
{
    // move constantly
    stepperMoldingBlockM1.runSpeed();

    if(digitalRead(LIMIT_SWITCH) == LOW){
        // Stepper Molding Block - stepper bounce mode setup
        stepperMoldingBlockM1.stop();

        // move a little forward to no longer touch limit switch
        stepperMoldingBlockM1.move(MB_MOVE_FROM_LIMIT_SWITCH);
        stepperMoldingBlockM1.runToPosition();

        // set EOC move distance
        stepperMoldingBlockM1.move(EOC_TARGET_POSITION);

        calibrateMoldingBlockMotors = false;
    }
}

void ModelerMain::runMotors()
{
    runExtruderMotor();
    runMoldingBlockMotors();
    _motorsON = true;
}

// move the stepper motor (one step at a time)
void ModelerMain::runExtruderMotor()
{
    // it is not possible to use delay to send another digitalwrite command, so
    // it is required to skip the command before being issued again
    if(_skipCounterExtruder > 0){
        _skipCounterExtruder--;
        return;
    }
    
    digitalWrite(EXTRUDER_STEP, highLow);
    _skipCounterExtruder = EXTRUDER_SKIPS - extruderSpeed;
    highLow = !highLow;
}

void ModelerMain::runMoldingBlockMotors()
{
    if(_skipCounterMB > 0){
        _skipCounterMB--;
        return;
    }

    // check if motor reached EOC, and set new EOC
    if (stepperMoldingBlockM1.distanceToGo() == 0){
        stepperMoldingBlockM1.move(_moldingBlockM1MoveDirection*EOC_TARGET_POSITION);
        _moldingBlockM1MoveDirection *= -1;
    }

    // run motors
    stepperMoldingBlockM1.run();
    _skipCounterMB = MB_SKIPS - moldingBlockM1Speed;
}

void ModelerMain::stopMotors()
{
    stopExtruderMotor();
    stopMoldingBlockM1();
    updateDataAndSend();
    _motorsON = false;
}

void ModelerMain::stopExtruderMotor()
{
    digitalWrite(EXTRUDER_STEP, LOW);
    extruderOn = false;
}

void ModelerMain::stopMoldingBlockM1()
{
    stepperMoldingBlockM1.stop();
    moldingBlockOn = false;
    _moldingBlockM1MoveDirection = -1;
    stepperMoldingBlockM1.setSpeed(-MB_EOC_CALIBRATION_SPEED);
    calibrateMoldingBlockMotors = true; // after a stop, a calibration is required
}

void ModelerMain::updateDataAndSend()
{
    myData->operating = extruderOn && moldingBlockOn;
    myData->motorSpeed1 = extruderSpeed;
    myData->motorSpeed2 = moldingBlockM1Speed;

    EspNowCallbacks::sendMessage(Machines::server, *myData);
}
