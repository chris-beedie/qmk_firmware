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

#include QMK_KEYBOARD_H


//##########################################################################################################################
//things you will want to change


//How many modes? (Max=8)
#define MODES 8 // number of modes to switch through

//indicator colour for each mode, all 8 need to be defined even if not in use
#define HSV_MODE_0 0,   255, 100
#define HSV_MODE_1 30,  255, 100
#define HSV_MODE_2 60,  255, 100
#define HSV_MODE_3 90,  255, 100
#define HSV_MODE_4 120, 255, 100
#define HSV_MODE_5 150, 255, 100
#define HSV_MODE_6 180, 255, 100
#define HSV_MODE_7 210, 255, 100




//##########################################################################################################################
//things you might want to change

//keys used to represent the control and status bits (should be keys that don't otherwise do anything)
#define TERMINATOR KC_F24

#define MODE_BITS_SIZE 3
const int MODE_BITS[MODE_BITS_SIZE] = {
    KC_F23,
    KC_F22,
    KC_F21
};

#define KEY_BITS_SIZE 3
const int KEY_BITS[KEY_BITS_SIZE] = {
    KC_F20,
    KC_F19,
    KC_F18
};


//##########################################################################################################################
//things you shouldn't need to change

//all the code for managing the modes is in the mode.c file
#include "mode.c"
