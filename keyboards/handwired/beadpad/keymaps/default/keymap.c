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


//How many modes (Max=10)?
#define MODES 10 // number of modes to switch through

//indicator colour for each mode, all 10 need to be defined even if not in use
#define HSV_MODE_0 0,   255, 100
#define HSV_MODE_1 25,  255, 100
#define HSV_MODE_2 50,  255, 100
#define HSV_MODE_3 75,  255, 100
#define HSV_MODE_4 100, 255, 100
#define HSV_MODE_5 125, 255, 100
#define HSV_MODE_6 150, 255, 100
#define HSV_MODE_7 175, 255, 100
#define HSV_MODE_8 200, 255, 100
#define HSV_MODE_9 225, 255, 100



//##########################################################################################################################
//things you might want to change

//what each key is mapped to
#define KEY1 KC_1
#define KEY2 KC_2
#define KEY3 KC_3
#define KEY4 KC_4

#define ROT_BUT KC_5
#define ROT_CW  KC_6
#define ROT_CCW KC_7


//the modifiers for each layer (use qk codes to allow easy combos), all 10 need to be defined even if not in use
#define MODS_MODE_0 KC_NO
#define MODS_MODE_1 QK_LSFT
#define MODS_MODE_2 QK_LCTL
#define MODS_MODE_3 QK_LALT
#define MODS_MODE_4 QK_LGUI
#define MODS_MODE_5 (QK_LCTL | QK_LALT)
#define MODS_MODE_6 (QK_LCTL | QK_LSFT)
#define MODS_MODE_7 (QK_LCTL | QK_LGUI)
#define MODS_MODE_8 (QK_LCTL | QK_LALT | QK_LSFT)
#define MODS_MODE_9 (QK_LCTL | QK_LALT | QK_LGUI)


//##########################################################################################################################
//things you probably shouldn't be changing

//all the code for managing the modes is in the
#include "mode.c"
