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

#include "ws2812.h"
#include "color.h"


#define SIZE_OF_BYTE 8

#ifndef HUE_RESOLUTION
#define HUE_RESOLUTION 6
#endif
#ifndef SAT_RESOLUTION
#define SAT_RESOLUTION 6
#endif
#ifndef VAL_RESOLUTION
#define VAL_RESOLUTION 4
#endif

#define HUE_STEP (1 << (SIZE_OF_BYTE - HUE_RESOLUTION))
#define SAT_STEP (1 << (SIZE_OF_BYTE - SAT_RESOLUTION))
#define VAL_STEP (1 << (SIZE_OF_BYTE - VAL_RESOLUTION))

#define HUE_OFFSET (2*SIZE_OF_BYTE - HUE_RESOLUTION)
#define SAT_OFFSET (2*SIZE_OF_BYTE - HUE_RESOLUTION - SAT_RESOLUTION)
#define VAL_OFFSET (2*SIZE_OF_BYTE - HUE_RESOLUTION - SAT_RESOLUTION - VAL_RESOLUTION)

#if HUE_RESOLUTION + SAT_RESOLUTION + VAL_RESOLUTION > 16
#error HSV too big to pack, must be <=16 bits total
#endif

// hsv storage =========================================================================================================

uint16_t pack(uint8_t value, uint8_t bits, uint8_t offset) {
    //compresses (lossy) a byte to the desired number of bits

    //add on the bit below the mask, means the value will get rounded rather than truncated
    uint8_t _value = value + (1 << (SIZE_OF_BYTE - bits -1));

    //check operation above didn't cause an overflow
    if (_value < value)
        _value = UINT8_MAX;

    //shift it until we are left with the desired number of bits and then reposition for packing
    return (_value >> (SIZE_OF_BYTE - bits)) << offset;
}

uint8_t unpack(uint16_t packed, uint8_t bits, uint8_t offset ) {
    //restores a packed number of bits to a byte representation

    //shift it back to the end, mask it, then restore to 8bit
    return ((packed >> offset) & ((1 << bits) - 1)) << (SIZE_OF_BYTE - bits);
}

uint16_t hsv_pack(uint8_t hue, uint8_t sat, uint8_t val) {

    //compresses/downsamples the hsv values (3x byte) to a word (1x16 bits) using the desired number of bits per value

    return (
        pack(hue, HUE_RESOLUTION, HUE_OFFSET) |
        pack(sat, SAT_RESOLUTION, SAT_OFFSET) |
        pack(val, VAL_RESOLUTION, VAL_OFFSET)
    );

}

HSV hsv_unpack(uint16_t packed_hsv) {

    //restores the word (1x16 bit) to 3x bytes as the hsv struct

    return (HSV){
        .h = unpack(packed_hsv, HUE_RESOLUTION, HUE_OFFSET),
        .s = unpack(packed_hsv, SAT_RESOLUTION, SAT_OFFSET),
        .v = unpack(packed_hsv, VAL_RESOLUTION, VAL_OFFSET)
    };

}


// hsv updating  =========================================================================================================


static bool enabled[RGBLED_NUM];
static LED_TYPE led[RGBLED_NUM];
static HSV hsv;

HSV hsv_get_current(void) { return hsv; }

void hsv_enable(bool enable) {
    for (uint8_t index = 0; index < RGBLED_NUM; index++) {
        enabled[index] = enable;
    }
}

void hsv_enable_at(uint8_t index, bool enable) {
     if (index >= RGBLED_NUM)
        return;

    enabled[index] = enable;
}

void hsv_enable_binary(uint16_t bin_val) {

    for(uint8_t bit = 0; bit < RGBLED_NUM; ++bit, bin_val >>= 1) {
        hsv_enable_at(RGBLED_NUM - bit - 1, bin_val & 1);
    }

}

__attribute__((weak)) void hsv_call_driver(LED_TYPE *start_led, uint8_t num_leds) { ws2812_setleds(start_led, num_leds); }

void hsv_refresh(void) {

    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        if (!enabled[i]) {
            led[i].r = 0;
            led[i].g = 0;
            led[i].b = 0;
            #ifdef RGBW
            led[i].w = 0;
            #endif
        }
    }

    hsv_call_driver(led, RGBLED_NUM);
}


void hsv_set(uint8_t hue, uint8_t sat, uint8_t val) {

    hsv.h = hue;
    hsv.s = sat;
    hsv.v = val;

    RGB rgb = hsv_to_rgb(hsv);

    for (uint8_t index = 0; index < RGBLED_NUM; index++) {
        led[index].r = rgb.r;
        led[index].g = rgb.g;
        led[index].b = rgb.b;
        #ifdef RGBW
        led[index].w = 0;
        #endif
    }

    hsv_refresh();
}

//change the hsv values by the step amount, allow hue to wrap
void hsv_increase_hue(void) { hsv_set(increment_step_wrap(hsv.h, HUE_STEP, UINT8_MAX), hsv.s, hsv.v); }
void hsv_decrease_hue(void) { hsv_set(decrement_step_wrap(hsv.h, HUE_STEP, UINT8_MAX), hsv.s, hsv.v); }
void hsv_increase_sat(void) { hsv_set(hsv.h, increment_step(hsv.s, SAT_STEP, UINT8_MAX), hsv.v); }
void hsv_decrease_sat(void) { hsv_set(hsv.h, decrement_step(hsv.s,SAT_STEP, 0), hsv.v); }
void hsv_increase_val(void) { hsv_set(hsv.h, hsv.s, increment_step(hsv.v, VAL_STEP, UINT8_MAX)); }
void hsv_decrease_val(void) { hsv_set(hsv.h, hsv.s, decrement_step(hsv.v, VAL_STEP, 0)); }



