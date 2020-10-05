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



enum KEY_BIT {
    KEY1,
    KEY2,
    KEY3,
    KEY4,
    KEY5,
    ROT_BUT,
    ROT_CW,
    ROT_CCW
};

enum MODE_INDICATION {
    MI_NONE,
    MI_MODE_UP,
    MI_MODE_DOWN,
    MI_MODE_UP_DOWN,
    MI_ALL,
    MI_BINARY,
    MI_BINARY_COLOR
};


#define MAX_MODES 8
#define KEY_COUNT 8
#define MODE_BITS_SIZE 3
#define KEY_BITS_SIZE 3

#define SIZE_OF_BYTE 8

#define HUE_PACK_BITS 7
#define SAT_PACK_BITS 6
#define VAL_PACK_BITS 3

#define HUE_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS)
#define SAT_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS - SAT_PACK_BITS)
#define VAL_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS - SAT_PACK_BITS - VAL_PACK_BITS)




#if HUE_PACK_BITS + SAT_PACK_BITS + VAL_PACK_BITS > 16
#error HSV too big to pack
#endif


#define KEY_EDIT_HSV KEY1
#define KEY_EDIT_HSV_SAD KEY2
#define KEY_EDIT_HSV_SAI KEY3
#define KEY_EDIT_HSV_VAD KEY4
#define KEY_EDIT_HSV_VAI KEY5
#define KEY_EDIT_HSV_HUD ROT_CW
#define KEY_EDIT_HSV_HUI ROT_CCW


#define KEY_EDIT_MODE_COUNT KEY2
#define KEY_EDIT_MODE_COUNT_UP ROT_CW
#define KEY_EDIT_MODE_COUNT_DOWN ROT_CCW

#define LAYOUT( \
    k00, k01, k02, k03, k04, k05  \
) \
{ \
    { k00, k01, k02, k03, k04, k05 } \
}
