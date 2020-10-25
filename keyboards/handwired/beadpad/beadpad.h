/* Copyright 2020 Chris Beedie
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "quantum.h"

#include "beadpad_eeprom.h"
#include "beadpad_led.h"

#define MAX_MODES 8
#define MODE_BITS_SIZE 3
#define TOTAL_KEY_COUNT KEY_COUNT + 3 //KEYS + ROT (but, ccw, cw)

#define KEYS KEY1, KEY2, KEY3, KEY4, KEY5
#define ROT_KEYS ROT_BUT, ROT_CCW, ROT_CW

enum key_bit_t {
    KEYS,
    ROT_KEYS
};

enum keystate_t {
    NONE,
    PRESSED,
    SETTINGS,
    HELD
} keystate[TOTAL_KEY_COUNT];

