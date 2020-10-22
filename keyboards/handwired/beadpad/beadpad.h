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

#define MAX_MODES 8
#define MODE_BITS_SIZE 3
#define TOTAL_KEY_COUNT KEY_COUNT + 3 //KEYS + ROT (but, ccw, cw)

#define KEYS KEY1, KEY2, KEY3, KEY4, KEY5
#define ROT_KEYS ROT_BUT, ROT_CCW, ROT_CW

enum key_bit_t {
    KEYS,
    ROT_KEYS
};

#define SETTING_HSV_IDX 0
#define SETTING_HSV_KEY KEY1
#define SETTING_HSV_KEY_SAD KEY2
#define SETTING_HSV_KEY_SAI KEY3
#define SETTING_HSV_KEY_VAD KEY4
#define SETTING_HSV_KEY_VAI KEY5
#define SETTING_HSV_KEY_HUD ROT_CCW
#define SETTING_HSV_KEY_HUI ROT_CW

#define SETTING_MODE_INDICATION_IDX 1
#define SETTING_MODE_INDICATION_KEY KEY2
#define SETTING_MODE_INDICATION_KEY_UP ROT_CW
#define SETTING_MODE_INDICATION_KEY_DOWN ROT_CCW

#define SETTING_MODE_COUNT_IDX 2
#define SETTING_MODE_COUNT_KEY KEY3
#define SETTING_MODE_COUNT_KEY_UP ROT_CW
#define SETTING_MODE_COUNT_KEY_DOWN ROT_CCW

#define SETTING_COUNT 3

struct setting {
    bool enabled;
    uint16_t keycode;
    void (*update)(uint16_t);
    void (*complete)(void);
};

struct setting settings[SETTING_COUNT];

enum mode_indication_t {
    MI_NONE,
    MI_MODE_UP,
    MI_MODE_DOWN,
    MI_MODE_UP_DOWN,
    MI_ALL,
    MI_BINARY,

} mode_indication;

#define MI_LAST MI_BINARY

enum keystate_t {
    NONE,
    PRESSED,
    CONFIG,
    HELD
} keystate[TOTAL_KEY_COUNT];

extern uint16_t key_mode_up;
extern uint16_t key_mode_down;

extern uint8_t mode_current;

void mode_set_indicator(uint8_t mode);
void mode_increment_indicator(void);
void mode_decrement_indicator(void);

void mode_set(uint8_t mode);
void mode_increment(void);
void mode_decrement(void);

bool check_setting_update(uint16_t keycode);
bool check_setting_complete(uint16_t keycode);
