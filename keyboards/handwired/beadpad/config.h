/*
Copyright 2020 Chris Beedie

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xBEAD
#define PRODUCT_ID      0xCDBD
#define DEVICE_VER      0x0001
#define MANUFACTURER    bead
#define PRODUCT         beadpad
#define DESCRIPTION     macropad

/* key matrix size */
#define MATRIX_ROWS 1
#define MATRIX_COLS 6
#define ENCODER_RESOLUTION 4

#define MATRIX_ROW_PINS { F7 }
#define MATRIX_COL_PINS { C6, D7, B5, B4, E6, D1 }
#define ENCODERS_PAD_A { D0 }
#define ENCODERS_PAD_B { D4 }
#define UNUSED_PINS

#define DIODE_DIRECTION COL2ROW

#define RGB_DI_PIN B6
// #define RGBLED_NUM 5
// #define RGBLIGHT_SLEEP

#define DEBOUNCE 5

#define TAP_CODE_DELAY 0




// disable debug print
//#define NO_DEBUG

// disable print
//#define NO_PRINT

// disable action features
#define NO_ACTION_LAYER
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
