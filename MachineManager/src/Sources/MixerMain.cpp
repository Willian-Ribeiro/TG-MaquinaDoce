#include "../Headers/MixerMain.h"

EspNowCallbacks MixerMain::espnow = EspNowCallbacks();

void MixerMain::setupMixer(void)
{
    Serial.begin(BAUDRATE);

    // Motor relay
    pinMode(RELAY_1, OUTPUT);
    pinMode(RELAY_2, OUTPUT);
    setMixerSpeed(SPEED_OFF);

    // ESP NOW communication
    EspNowCallbacks::initialize();
    myData->dataSource = DATA_SOURCE_MIXER;
        
    // LCD initialization
    tft.init();
    tft.setRotation(1);       // Set the rotation before we calibrate
    touch_calibrate();        // call screen calibration
    tft.fillScreen(TFT_BLUE); // clear screen

    drawFrame1();
    v1Btn();
    drawFrame2();
    timeBtn();
    drawFrame3();
    stopBtn();
}

void MixerMain::loopMixer(void)
{
    lcdCheckTouch();
    if(EspNowCallbacks::updateData(myData))
    {
        // update current values
        operationTime = myData->operationTime;

        // map incoming value to motor speed
        if(myData->motorSpeed1 > 50)
            currentSpeed = SPEED_V2;
        else if(myData->motorSpeed1 > 0)
            currentSpeed = SPEED_V1;
        else
            currentSpeed = SPEED_OFF;

        // check what operations should be done
        if(!machineOperating && operationTime > 0)
            startMixer(millis());
        else if(machineOperating && operationTime == 0)
            stopMixer();
    }

    if(machineOperating)
        updateTimer();
}

void MixerMain::updateTimer(void)
{
    unsigned long int timeNow = millis();
    if(timeNow - previousTime >= 1000) // update every second
    {
        previousTime = timeNow;

        // check end of operation
        if(timeNow >= finishTime) // check end of operation
        {
            machineOperating = false;
            operationTime = 0;
            setMixerSpeed(SPEED_OFF);
            myData->operationTime = operationTime;
            stopBtn();
        }
        else
        {
            operationTime = finishTime - timeNow;
            myData->operationTime = operationTime;
            timeBtn();
        }

        EspNowCallbacks::sendMessage(Machines::server, *myData);
    }
}

void MixerMain::startMixer(unsigned long int currentTime)
{
    // start a counter and make it update myData.operationTime
    machineOperating = true;

    startTime = currentTime;
    finishTime = startTime + operationTime;

    // set default speed
    if(currentSpeed == SPEED_OFF || currentSpeed == SPEED_V1)
    {
        setMixerSpeed(SPEED_V1);
        v1Btn();
    }
    else
    {
        setMixerSpeed(currentSpeed);
        v2Btn();
    }
    myData->operationTime = operationTime;

    EspNowCallbacks::sendMessage(Machines::server, *myData);
};

void MixerMain::stopMixer()
{
    // stop a counter
    operationTime = 0;
    myData->operationTime = 0;

    setMixerSpeed(SPEED_OFF);

    // must be set agter mixer speed
    machineOperating = false;

    stopBtn();

    EspNowCallbacks::sendMessage(Machines::server, *myData);
};

void MixerMain::setMixerSpeed(int speed)
{
    switch (speed)
    {
    case SPEED_V1:
        if (machineOperating)
        {
            digitalWrite(RELAY_1, !HIGH);
            digitalWrite(RELAY_2, !LOW);
        }
        currentSpeed = SPEED_V1;
        myData->motorSpeed1 = currentSpeed;
        break;
    case SPEED_V2:
        if (machineOperating)
        {
            digitalWrite(RELAY_1, !HIGH);
            digitalWrite(RELAY_2, !HIGH);
        }
        currentSpeed = SPEED_V2;
        myData->motorSpeed1 = currentSpeed;
        break;
    default:
        digitalWrite(RELAY_1, !LOW);
        digitalWrite(RELAY_2, !LOW);
        currentSpeed = SPEED_OFF;
        myData->motorSpeed1 = currentSpeed;
        break;
    }
}

// veifies if a touch happened and where
void MixerMain::lcdCheckTouch()
{
    uint16_t x, y;

    if (tft.getTouch(&x, &y))
    {
        // check click cooldown
        unsigned long int currentTime = millis();
        if(currentTime - lcdPreviousClick < LCD_CLICK_COOLDOWN)
            return;
            
        lcdPreviousClick = currentTime;

        // Draw a black spot to show where touch was calculated to be
        #ifdef BLACK_SPOT
            tft.fillCircle(x, y, 2, TFT_BLACK);
        #endif

        // check Start button
        if ((x > START_BTN_X) && (x < (START_BTN_X + START_BTN_W)))
        {
            if ((y > START_BTN_Y) && (y <= (START_BTN_Y + START_BTN_H)))
            {
                Serial.println("Start btn hit");
                startBtn();
                startMixer(currentTime);
            }
        }

        // check Stop button
        if ((x > STOP_BTN_X) && (x < (STOP_BTN_X + STOP_BTN_W)))
        {
            if ((y > STOP_BTN_Y) && (y <= (STOP_BTN_Y + STOP_BTN_H)))
            {
                Serial.println("STOP btn hit");
                stopMixer();
            }
        }

        // check V1 button
        if ((x > V1_BTN_X) && (x < (V1_BTN_X + V1_BTN_W)))
        {
            if ((y > V1_BTN_Y) && (y <= (V1_BTN_Y + V1_BTN_H)))
            {
                Serial.println("V1 btn hit");
                v1Btn();
                setMixerSpeed(SPEED_V1);
            }
        }
        // check V2 button
        if ((x > V2_BTN_X) && (x < (V2_BTN_X + V2_BTN_W)))
        {
            if ((y > V2_BTN_Y) && (y <= (V2_BTN_Y + V2_BTN_H)))
            {
                Serial.println("V2 btn hit");
                v2Btn();
                setMixerSpeed(SPEED_V2);
            }
        }
            
        if(machineOperating == false)
        {
            // check onePlus button
            if ((x > ONEPLUS_BTN_X) && (x < (ONEPLUS_BTN_X + ONEPLUS_BTN_W)))
            {
                if ((y > ONEPLUS_BTN_Y) && (y <= (ONEPLUS_BTN_Y + ONEPLUS_BTN_H)))
                {
                    Serial.println("OnePlus btn hit");
                    // onePlusBtn();
                    operationTime += 1 * 60 * 1000; // millis
                    if(operationTime > 99 * 60 * 1000) // max time is 99min
                        operationTime = 99 * 60 * 1000;

                    timeBtn();
                }
            }

            // check fivePlus button
            if ((x > FIVEPLUS_BTN_X) && (x < (FIVEPLUS_BTN_X + FIVEPLUS_BTN_W)))
            {
                if ((y > FIVEPLUS_BTN_Y) && (y <= (FIVEPLUS_BTN_Y + FIVEPLUS_BTN_H)))
                {
                    Serial.println("Five Plus btn hit");
                    // fivePlusBtn();
                    operationTime += 5 * 60 * 1000;
                    if(operationTime > 99 * 60 * 1000) // max time is 99min
                        operationTime = 99 * 60 * 1000;

                    timeBtn();
                }
            }

            // check time button
            if ((x > TIME_BTN_X) && (x < (TIME_BTN_X + TIME_BTN_W)))
            {
                if ((y > TIME_BTN_Y) && (y <= (TIME_BTN_Y + TIME_BTN_H)))
                {
                    Serial.println("Time btn hit");
                    // timeBtn();
                }
            }

            // check oneMinus button
            if ((x > ONEMINUS_BTN_X) && (x < (ONEMINUS_BTN_X + ONEMINUS_BTN_W)))
            {
                if ((y > ONEMINUS_BTN_Y) && (y <= (ONEMINUS_BTN_Y + ONEMINUS_BTN_H)))
                {
                    Serial.println("OneMinus btn hit");
                    // oneMinusBtn();
                    
                    if(operationTime < 1 * 60 * 1000)
                        operationTime = 0;
                    else
                        operationTime -= 1 * 60 * 1000;

                    timeBtn();
                }
            }

            // check fiveMinus button
            if ((x > FIVEMINUS_BTN_X) && (x < (FIVEMINUS_BTN_X + FIVEMINUS_BTN_W)))
            {
                if ((y > FIVEMINUS_BTN_Y) && (y <= (FIVEMINUS_BTN_Y + FIVEMINUS_BTN_H)))
                {
                    Serial.println("FiveMinus btn hit");
                    // fiveMinusBtn();
                    
                    if(operationTime < 5 * 60 * 1000)
                        operationTime = 0;
                    else
                        operationTime -= 5 * 60 * 1000;

                    timeBtn();
                }
            }
        }
        
    }
}

void MixerMain::drawFrame1()
{
    tft.drawRect(FRAME1_X, FRAME1_Y, FRAME1_W, FRAME1_H, TFT_BLACK);
}

void MixerMain::offBtn()
{
    tft.fillRect(V1_BTN_X, V1_BTN_Y, V1_BTN_W, V1_BTN_H, TFT_DARKGREY);
    tft.fillRect(V2_BTN_X, V2_BTN_Y, V2_BTN_W, V2_BTN_H, TFT_DARKGREY);
    drawFrame1();
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("V1", V1_BTN_X + (V1_BTN_W / 2), V1_BTN_Y + (V1_BTN_H / 2));
    tft.drawString("V2", V2_BTN_X + (V2_BTN_W / 2), V2_BTN_Y + (V2_BTN_H / 2));
}

void MixerMain::v1Btn()
{
    tft.fillRect(V1_BTN_X, V1_BTN_Y, V1_BTN_W, V1_BTN_H, TFT_GREEN);
    tft.fillRect(V2_BTN_X, V2_BTN_Y, V2_BTN_W, V2_BTN_H, TFT_DARKGREY);
    drawFrame1();
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("V1", V1_BTN_X + (V1_BTN_W / 2), V1_BTN_Y + (V1_BTN_H / 2));
    tft.drawString("V2", V2_BTN_X + (V2_BTN_W / 2), V2_BTN_Y + (V2_BTN_H / 2));
}

void MixerMain::v2Btn()
{
    tft.fillRect(V1_BTN_X, V1_BTN_Y, V1_BTN_W, V1_BTN_H, TFT_DARKGREY);
    tft.fillRect(V2_BTN_X, V2_BTN_Y, V2_BTN_W, V2_BTN_H, TFT_GREEN);
    drawFrame1();
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("V1", V1_BTN_X + (V1_BTN_W / 2), V1_BTN_Y + (V1_BTN_H / 2));
    tft.drawString("V2", V2_BTN_X + (V2_BTN_W / 2), V2_BTN_Y + (V2_BTN_H / 2));
}

void MixerMain::drawFrame2()
{
    tft.drawRect(FRAME2_X, FRAME2_Y, FRAME2_W, FRAME2_H, TFT_BLACK);
}

void MixerMain::timeBtn()
{
    tft.fillRect(ONEPLUS_BTN_X, ONEPLUS_BTN_Y, ONEPLUS_BTN_W, ONEPLUS_BTN_H, TFT_OLIVE);
    tft.fillRect(FIVEPLUS_BTN_X, FIVEPLUS_BTN_Y, FIVEPLUS_BTN_W, FIVEPLUS_BTN_H, TFT_ORANGE);
    tft.fillRect(ONEMINUS_BTN_X, ONEMINUS_BTN_Y, ONEMINUS_BTN_W, ONEMINUS_BTN_H, TFT_PINK);
    tft.fillRect(FIVEMINUS_BTN_X, FIVEMINUS_BTN_Y, FIVEMINUS_BTN_W, FIVEMINUS_BTN_H, TFT_PURPLE);
    tft.fillRect(TIME_BTN_X, TIME_BTN_Y, TIME_BTN_W, TIME_BTN_H, TFT_WHITE);
    drawFrame2();
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);

    unsigned long int timeS = operationTime /1000 % 60; // remainder of seconds
    unsigned long int timeM = (operationTime - timeS*1000) / 60 / 1000; // seconds to min
        
    String timeTxt = String(timeM) + ":" + String(timeS);
    if (timeS < 10)
        timeTxt = String(timeM) + ":0" + String(timeS);
        
    tft.drawString(String(timeTxt.c_str()), TIME_BTN_X + (TIME_BTN_W / 2), TIME_BTN_Y + (TIME_BTN_H / 2));
    tft.drawString("+1", ONEPLUS_BTN_X + (ONEPLUS_BTN_W / 2), ONEPLUS_BTN_Y + (ONEPLUS_BTN_H / 2));
    tft.drawString("+5", FIVEPLUS_BTN_X + (FIVEPLUS_BTN_W / 2), FIVEPLUS_BTN_Y + (FIVEPLUS_BTN_H / 2));
    tft.drawString("-1", ONEMINUS_BTN_X + (ONEMINUS_BTN_W / 2), ONEMINUS_BTN_Y + (ONEMINUS_BTN_H / 2));
    tft.drawString("-5", FIVEMINUS_BTN_X + (FIVEMINUS_BTN_W / 2), FIVEMINUS_BTN_Y + (FIVEMINUS_BTN_H / 2));
}

void MixerMain::drawFrame3()
{
    tft.drawRect(FRAME3_X, FRAME3_Y, FRAME3_W, FRAME3_H, TFT_BLACK);
}

void MixerMain::startBtn()
{
    tft.fillRect(START_BTN_X, START_BTN_Y, START_BTN_W, START_BTN_H, TFT_GREEN);
    tft.fillRect(STOP_BTN_X, STOP_BTN_Y, STOP_BTN_W, STOP_BTN_H, TFT_DARKGREY);
    drawFrame3();
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("START", START_BTN_X + (START_BTN_W / 2), START_BTN_Y + (START_BTN_H / 2));
    tft.drawString("STOP", STOP_BTN_X + (STOP_BTN_W / 2), STOP_BTN_Y + (STOP_BTN_H / 2));
}

void MixerMain::stopBtn()
{
    offBtn();
    timeBtn();
    tft.fillRect(START_BTN_X, START_BTN_Y, START_BTN_W, START_BTN_H, TFT_DARKGREY);
    tft.fillRect(STOP_BTN_X, STOP_BTN_Y, STOP_BTN_W, STOP_BTN_H, TFT_RED);
    drawFrame3();
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("START", START_BTN_X + (START_BTN_W / 2), START_BTN_Y + (START_BTN_H / 2));
    tft.drawString("STOP", STOP_BTN_X + (STOP_BTN_W / 2), STOP_BTN_Y + (STOP_BTN_H / 2));
}

// Calibrate LCD screen function 
void MixerMain::touch_calibrate()
{
    uint16_t calData[5];
    uint8_t calDataOK = 0;

    // check file system exists
    if (!SPIFFS.begin())
    {
        Serial.println("Formatting file system");
        SPIFFS.format();
        SPIFFS.begin();
    }

    // check if calibration file exists and size is correct
    if (SPIFFS.exists(CALIBRATION_FILE))
    {
        if (REPEAT_CAL)
        {
            // Delete if we want to re-calibrate
            SPIFFS.remove(CALIBRATION_FILE);
        }
        else
        {
            File f = SPIFFS.open(CALIBRATION_FILE, "r");
            if (f)
            {
                if (f.readBytes((char *)calData, 14) == 14)
                    calDataOK = 1;
                f.close();
            }
        }
    }

    if (calDataOK && !REPEAT_CAL)
    {
        // calibration data valid
        tft.setTouch(calData);
    }
    else
    {
        // data not valid so recalibrate
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(20, 0);
        tft.setTextFont(2);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);

        tft.println("Touch corners as indicated");

        tft.setTextFont(1);
        tft.println();

        if (REPEAT_CAL)
        {
            tft.setTextColor(TFT_RED, TFT_BLACK);
            tft.println("Set REPEAT_CAL to false to stop this running again!");
        }

        tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.println("Calibration complete!");

        // store data
        File f = SPIFFS.open(CALIBRATION_FILE, "w");
        if (f)
        {
            f.write((const unsigned char *)calData, 14);
            f.close();
        }
    }
}