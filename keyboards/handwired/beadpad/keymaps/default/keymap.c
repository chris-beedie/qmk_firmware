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

//indicator colour for each mode
#define HSV_MODE_0 0,   255, 50
#define HSV_MODE_1 30,  255, 50
#define HSV_MODE_2 60,  255, 50
#define HSV_MODE_3 90,  255, 50
#define HSV_MODE_4 120, 255, 50
#define HSV_MODE_5 150, 255, 50
#define HSV_MODE_6 180, 255, 50
#define HSV_MODE_7 210, 255, 50

//how should the current mode be displayed (MI_NONE, MI_MODE_UP, MI_MODE_DOWN, MI_MODE_UP_DOWN, MI_ALL, MI_BINARY)
#define MODE_INDICATION MI_MODE_UP

//How many modes? (Max=8)
#define MODE_COUNT 8 // number of modes to switch through

//==============================================================================================
//enable settinguration settings during operation to permit changing the settings above after flashing

//  - hold the setting key (indicated below) and press other keys to adjust settings,
//  - releasing setting key saves to eeprom
//  - these will be reset during flash unless PRESERVE_SETTINGS is defined
#define SETTING_HSV_ENABLE //HOLD KEY1 (SETTING_HSV_KEY) - KEY2/KEY3 = Sat, KEY4/KEY5 = Val, ROT_CW/ROT_CCW = Hue
#define SETTING_MODE_INDICATION_ENABLE //HOLD KEY2 (SETTING_MODE_INDICATION_KEY) - ROT_CW/ROT_CCW change mode indication setting
#define SETTING_MODE_COUNT_ENABLE //HOLD KEY3 (SETTING_MODE_COUNT_KEY) - ROT_CW/ROT_CCW change number of modes (count indicated in binary)

//#define PRESERVE_SETTINGS

//==============================================================================================
//define how modes are switched between

//dedicated buttons (optional) - these button presses will not be relayed to the computer
#define KEY_MODE_UP KEY1
#define KEY_MODE_DOWN KEY2

//hold button (optional) - can be layer on top of dedicated mode keys above
//ROT_CW or ROT_CCW can't be held, so are not valid for mode hold change keys
#define KEY_MODE_UP_HOLD KEY3
#define KEY_MODE_DOWN_HOLD KEY4

//define how long (ms) the button must be held before changing mode (500ms default)
//#define MODE_HOLD_TERM 300

//use the rotarty encoder to change by pressing and twisting
//not available if using ROT_CW or ROT_CCW as dedicated mode change options above
#define MODE_ROT_ADJUST_ENABLED


//##########################################################################################################################
//things you might want to change




//##########################################################################################################################
//things you shouldn't need to change

//include the library code that handles processing of these values.
#include "beadpad_core.c"

