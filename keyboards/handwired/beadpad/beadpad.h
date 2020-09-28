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


#define LAYOUT( \
    k00, k01, k02, k03, k04, k05  \
) \
{ \
    { k00, k01, k02, k03, k04, k05 } \
}
