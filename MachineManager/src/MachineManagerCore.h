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


#if HAS_TFT_LVGL_UI
  #include "lcd/extui/lib/mks_ui/tft_lvgl_configuration.h"
  #include "lcd/extui/lib/mks_ui/draw_ui.h"
  #include "lcd/extui/lib/mks_ui/mks_hardware_test.h"
  #include <lvgl.h>
#endif