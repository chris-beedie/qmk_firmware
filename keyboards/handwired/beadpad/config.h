

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

//these settings specify how the hardware how has been configured
//the default keymap allows settings and behaviour to be configured


#pragma once


//how many keys are there, exluding rotary encoder - expects them to be hooked up as a single row
#define KEY_COUNT 5
#define KEYS KEY1, KEY2, KEY3, KEY4, KEY5

//key pins
#define KEY_ROW_PIN F7
#define KEY_COL_PINS { C6, D7, B5, B4, E6, D1 }


//rotary encoder
#define ENCODER_PAD_A D0
#define ENCODER_PAD_B D4
//TODO - dedicated PIN



//LED config - assumes there is an rgb LED per key (excluding rotary encoder)
#define LED_PIN B6


//==============================================================================================================
//keys for sending data

//keys used to represent the control and status bits
//these are used to encode the message from the beadpad to the awating python script
//these should be keys that don't otherwise do anything, e.g. the high F keys

//terminator signifies message completion and begins processing
#define TERMINATOR KC_F24

//there are 8 modes, and therefore we need 3 bits (and therefore keys) to cover each mode
#define MODE_BITS { KC_F23, KC_F22, KC_F21 }


//KEY_BIT_SIZE is how many bits are required to represent the total number of keys (defined above) including 3 for encoder (button, ccw, cw)
//can be calculated as CEILING(LOG2(KEY_COUNT+3))
//there should be a key mapped to each bit
#define KEY_BITS_SIZE 3
#define KEY_BITS  { KC_F20, KC_F19, KC_F18 }

//beadpad_config.h includes the standard config.h definitions, updated according to above, which shouldn't need to be modified.
#include "lib/beadpad_config.h"
