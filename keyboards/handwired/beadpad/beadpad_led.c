#include "beadpad_led.h"


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

void hsv_init(void) {

    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    }

    hsv_enable(true);
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

    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        uprintf("led %u is %u: %u, %u, %u\n",i, enabled[i], led[i].r, led[i].g, led[i].b);
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


void hsv_increase_hue(void) { hsv_set(increment_step(hsv.h, RGBLIGHT_HUE_STEP, UINT8_MAX), hsv.s, hsv.v); }
void hsv_decrease_hue(void) { hsv_set(decrement_step(hsv.h, RGBLIGHT_HUE_STEP), hsv.s, hsv.v); }
void hsv_increase_sat(void) { hsv_set(hsv.h, increment_step(hsv.s, RGBLIGHT_SAT_STEP, UINT8_MAX), hsv.v); }
void hsv_decrease_sat(void) { hsv_set(hsv.h, decrement_step(hsv.s, RGBLIGHT_SAT_STEP), hsv.v); }
void hsv_increase_val(void) { hsv_set(hsv.h, hsv.s, increment_step(hsv.v, RGBLIGHT_VAL_STEP, UINT8_MAX)); }
void hsv_decrease_val(void) { hsv_set(hsv.h, hsv.s, decrement_step(hsv.v, RGBLIGHT_VAL_STEP)); }


