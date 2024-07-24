#pragma once

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#include "./Headers/EspNowCallbacks.h"

// definitions to draw LCD layout
// FRAME1 position and size
#define FRAME1_X (2*LCD_WIDTH/32)
#define FRAME1_W (28*LCD_WIDTH/32)
#define FRAME1_Y (2*LCD_HIGH/24)
#define FRAME1_H (4*LCD_HIGH/24)

// V1 zone size
#define V1_BTN_X (FRAME1_X)
#define V1_BTN_W (FRAME1_W/3)
#define V1_BTN_Y FRAME1_Y
#define V1_BTN_H FRAME1_H

// V2 zone size
#define V2_BTN_X (V1_BTN_X + 2*V1_BTN_W)
#define V2_BTN_W (FRAME1_W/3)
#define V2_BTN_Y FRAME1_Y
#define V2_BTN_H FRAME1_H

// FRAME2 position and size
#define FRAME2_X (2*LCD_WIDTH/32)
#define FRAME2_W (28*LCD_WIDTH/32)
#define FRAME2_Y (8*LCD_HIGH/24)
#define FRAME2_H (8*LCD_HIGH/24)

// OnePlus zone size
#define ONEPLUS_BTN_X (FRAME2_X)
#define ONEPLUS_BTN_W (4*FRAME2_W/28)
#define ONEPLUS_BTN_Y FRAME2_Y
#define ONEPLUS_BTN_H FRAME2_H

// FivePlus zone size
#define FIVEPLUS_BTN_X (ONEPLUS_BTN_X + ONEPLUS_BTN_W + FRAME2_W/28)
#define FIVEPLUS_BTN_W (4*FRAME2_W/28)
#define FIVEPLUS_BTN_Y FRAME2_Y
#define FIVEPLUS_BTN_H FRAME2_H

// Time zone size
#define TIME_BTN_X (FIVEPLUS_BTN_X + FIVEPLUS_BTN_W + 1*FRAME2_W/28)
#define TIME_BTN_W (8*FRAME2_W/28)
#define TIME_BTN_Y FRAME2_Y
#define TIME_BTN_H FRAME2_H

// OneMinus zone size
#define ONEMINUS_BTN_X (TIME_BTN_X + TIME_BTN_W + 1*FRAME2_W/28)
#define ONEMINUS_BTN_W (4*FRAME2_W/28)
#define ONEMINUS_BTN_Y FRAME2_Y
#define ONEMINUS_BTN_H FRAME2_H

// FiveMinus zone size
#define FIVEMINUS_BTN_X (ONEMINUS_BTN_X + ONEMINUS_BTN_W + FRAME2_W/28)
#define FIVEMINUS_BTN_W (4*FRAME2_W/28)
#define FIVEMINUS_BTN_Y FRAME2_Y
#define FIVEMINUS_BTN_H FRAME2_H

// FRAME3 position and size
#define FRAME3_X (2*LCD_WIDTH/32)
#define FRAME3_W (28*LCD_WIDTH/32)
#define FRAME3_Y (18*LCD_HIGH/24)
#define FRAME3_H (4*LCD_HIGH/24)

// Start zone size
#define START_BTN_X (FRAME3_X)
#define START_BTN_W (FRAME1_W/3)
#define START_BTN_Y FRAME3_Y
#define START_BTN_H FRAME3_H

// Stop zone size
#define STOP_BTN_X (START_BTN_X + 2*START_BTN_W)
#define STOP_BTN_W (FRAME1_W/3)
#define STOP_BTN_Y FRAME3_Y
#define STOP_BTN_H FRAME3_H

class MixerMain
{
    private:
        unsigned long int currentSpeed = SPEED_OFF; // can be only 0, 1 or 2
        unsigned long int operationTime = 0; // millis
        unsigned long int previousTime = 0; // used to update Time in LCD every second
        unsigned long int startTime = 0; // operation start time
        unsigned long int finishTime = 0; // set at what time the process will finish
        boolean machineOperating = false;

        unsigned long int lcdPreviousClick = 0;

        Data* myData = new Data();
        static EspNowCallbacks espnow;
        TFT_eSPI tft = TFT_eSPI(); // library to control LCD

    public:
        void setupMixer();
        void loopMixer();
        
        // state changing functions
        void updateTimer();
        void setMixerSpeed(int speed);
        void lcdCheckTouch();
        void timeBtn();
        void addTime(int time); // time in minutes
        void reduceTime(int time); // time in minutes
        void startMixer(unsigned long int currentTime);
        void stopMixer();
        void updateData();

        // draw on LCD screen functions
        void drawFrame1();
        void drawFrame2();
        void offBtn();
        void v1Btn();
        void v2Btn();
        void drawFrame3();
        void startBtn();
        void stopBtn();
        void touch_calibrate();

        
};