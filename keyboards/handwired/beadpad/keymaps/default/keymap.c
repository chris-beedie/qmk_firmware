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




//on flash the number of modes and mode indicator colours will be set to what is defined below, to keep what is stored in the eeprom define PRESERVE_CUSTOMISATION
//#define PRESERVE_CUSTOMISATION

//How many modes? (Max=8)
#define MODE_COUNT 8 // number of modes to switch through

//indicator colour for each mode, all 8 need to be defined even if not in use
#define HSV_MODE_0 0,   255, 100
#define HSV_MODE_1 30,  255, 100
#define HSV_MODE_2 60,  255, 100
#define HSV_MODE_3 90,  255, 100
#define HSV_MODE_4 120, 255, 100
#define HSV_MODE_5 150, 255, 100
#define HSV_MODE_6 180, 255, 100
#define HSV_MODE_7 210, 255, 100


#define EDIT_HSV_ENABLE
#define EDIT_MODE_INDICATION_ENABLE
#define EDIT_MODE_COUNT_ENABLE


#define MODE_UP KEY1
#define MODE_DOWN KEY2


//not possible if using ROT_CW or ROT_CCW for MODE_UP
//can still be used even with dedicated down button
//#define MODE_HOLD_TERM 500

#define MODE_UP_HOLD KEY3
#define MODE_DOWN_HOLD KEY4

//#define MODE_ROT_ADJUST_ENABLED

//FEATURE - CHANGE MODES OTF
    //for modes - press key1 and rot and twist rot -> shows binary indication
    //need to write to eeprom and read from eeprom on boot and have setttings here available after flash.

#define MODE_INDICATION MI_MODE_UP


//##########################################################################################################################
//things you might want to change

//keys used to represent the control and status bits (should be keys that don't otherwise do anything)
#define TERMINATOR KC_F24
#define MODE_BITS { KC_F23, KC_F22, KC_F21 }
#define KEY_BITS  { KC_F20, KC_F19, KC_F18 }


//##########################################################################################################################
//things you shouldn't need to change

//all the code for managing the modes is in the mode.c file
#include "mode.c"

