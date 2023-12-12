/**
 * Machine Manager Firmware
 * Copyright (c) 2020 MachineManagerFirmware [https://github.com/Willian-Ribeiro/TG-MaquinaDoce.git]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Configuration.h
 *
 * Basic settings such as:
 *
 * - Type of electronics
 * - Type of temperature sensor
 * - Printer geometry
 * - Endstop configuration
 * - LCD controller
 * - Extra features
 *
 * Advanced settings can be found in Configuration_adv.h
 *
 */
#define CONFIGURATION_H_VERSION 0001
#define STRING_CONFIG_H_AUTHOR "(Willian Ribeiro, default config)"
#define CUSTOM_MACHINE_NAME "My Brigadeiro Machine"

//===========================================================================
//============================= Getting Started =============================
//===========================================================================

/**
 * Set what type of machine this board will operate
 * :[MACHINE_SERVER, MACHINE_MIXER, MACHINE_MODELER, MACHINE_PACKER]
 * OBS! : Only ONE option must be set as true!!!
 */
#define MACHINE_SERVER true
#define MACHINE_MIXER false
#define MACHINE_MODELER false
#define MACHINE_PACKER false

/**
 * Set communication baud rate, default is 115200
 * :[2400, 9600, 19200, 38400, 57600, 115200, 250000, 500000, 1000000]
*/
#define BAUDRATE 115200

/**
 * MAC Address, machine's unique ID.
 * Must be defined for each controller in the project
*/
#define SERVER_MAC_ADDRESS {0xB4, 0xE6, 0x2D, 0x37, 0x1C, 0x1F}
#define MIXER_MAC_ADDRESS {0xF4, 0xCF, 0xA2, 0xEF, 0x40, 0xF6}
#define MODELER_MAC_ADDRESS {0x48, 0x3F, 0xDA, 0xAA, 0x51, 0xCF}
#define PACKER_MAC_ADDRESS {0x48, 0x3F, 0xDA, 0xAA, 0x51, 0xCF}

/**
 * General configs.
 * Definitions used throughout the projetct by more than one machine
*/
#define WIFI_SSID "esp_server"

//=============================================================================
//============================= Server Settings ===============================
//=============================================================================
#if MACHINE_SERVER == true

#define WIFI_SSID "esp_server"

//===================== Pinout Definition =====================

#define LED1 D0
#define LED2 D4 // same as LED_BUILTIN

#endif

//===========================================================================
//============================= MIXER Settings ==============================
//===========================================================================

// This is the file name used to store the touch coordinate
// calibration data. Change the name to start a new calibration.
#define CALIBRATION_FILE "/TouchCalData3"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

// Comment out to stop drawing black spots
#define BLACK_SPOT

// Speed settings
#define SPEED_OFF 0
#define SPEED_V1 1
#define SPEED_V2 2
// #define SPEED_V3 3

// LCD definitions 240x320
#define LCD_HIGH 240
#define LCD_WIDTH 320
#define LCD_CLICK_COOLDOWN 200 // milliseconds

//===================== Pinout Definition =====================
// dummy definitions
#define RELAY_1 50
#define RELAY_2 50
#define MIXER_PWM 50

#if MACHINE_MIXER == true

#define RELAY_1 D0
#define RELAY_2 D1
#define MIXER_PWM D2
// #define TOUCH_CS PIN_D4
/**
 * Pins D3 until D8 are defined by the LCD display and touch
 * on TFT_eSPI own library, to change access user_setup file
 * Is possible to manage SPI ports to use ESP's dedicated inputs
 * in order to free up pins D3 till D8, but wont be possible
 * to use touch display 
*/

#endif

//=============================================================================
//============================= Modeler Settings ===============================
//=============================================================================
#if MACHINE_MODELER == true



//===================== Pinout Definition =====================

#define LED1 D0
#define LED2 D4 // same as LED_BUILTIN

#endif

//=============================================================================
//============================= Packer Settings ===============================
//=============================================================================
#if MACHINE_PACKER == true



//===================== Pinout Definition =====================

#define LED1 D0
#define LED2 D4 // same as LED_BUILTIN

#endif
