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

 /*
    ESP8266 comes pre set with a watchdog that resets the board if no feed is sent after 3.2 seconds
    For functions that might need to process for longer than 3.2 s the function yield() can be used to
    send feed signal.
 /*