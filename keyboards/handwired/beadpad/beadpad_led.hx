
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

#include "ws2812.h"
#include "color.h"


HSV hsv_get_current(void);

//__attribute__((weak)) void hsv_call_driver(LED_TYPE *start_led, uint8_t num_leds);

void hsv_enable(bool enable);
void hsv_enable_at(uint8_t index, bool enable);

void hsv_init(void);

void hsv_refresh(void);
void hsv_set(uint8_t hue, uint8_t sat, uint8_t val);

void hsv_increase_hue(void);
void hsv_decrease_hue(void);
void hsv_increase_sat(void);
void hsv_decrease_sat(void);
void hsv_increase_val(void);
void hsv_decrease_val(void);

void hsv_set_binary(uint16_t bin_val, uint8_t hue, uint8_t sat, uint8_t val);

// HSV hsv_mode_read(uint8_t mode);
// void hsv_mode_write(uint8_t mode, uint8_t hue, uint8_t sat, uint8_t val);
// void hsv_mode_write_current(uint8_t mode);

uint16_t hsv_pack(uint8_t hue, uint8_t sat, uint8_t val);
HSV hsv_unpack(uint16_t packed_hsv);
