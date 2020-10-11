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
#include "beadpad.h"
#include "beadpad_util.c"
#include "beadpad_eeprom.c"
#include "beadpad_led.c"

uint8_t mode_count = MAX_MODES;
uint8_t mode_current = 0; //keep track of current mode
uint8_t mode_prev = 0;

__attribute__((weak)) void handle_key(uint16_t keycode, bool pressed) {  }

//mode operations ==================================================================================================================

//set the mode indicator only, used when mode up/down hold is activated, but may need to be undone if an edit function overrides
void mode_set_indicator(uint8_t mode) {

    HSV mode_hsv = eeprom_read_hsv(mode);

    uprintf("h: %u,s: %u, v: %u\n", mode_hsv.h, mode_hsv.s, mode_hsv.v);

    hsv_enable(false);

    switch (mode_indication) {
        case MI_MODE_UP:
            hsv_enable_at(key_mode_up,true);
            break;
        case MI_MODE_DOWN:
            hsv_enable_at(key_mode_down, true);
            break;
        case MI_MODE_UP_DOWN:
            hsv_enable_at(key_mode_up,true);
            hsv_enable_at(key_mode_down, true);
            break;
        case MI_ALL:
            hsv_enable(true);
            break;
        case MI_BINARY:
            hsv_enable_binary(mode);
            break;
        default:
            break;
    }

    hsv_set(mode_hsv.h, mode_hsv.s, mode_hsv.v);
}

// sets the mode - just a case of changin colour and setting the var
void mode_set(uint8_t mode) {

    mode_prev = mode_current;
    mode_current = mode;

    mode_set_indicator(mode);
}



void mode_increment_indicator() {
    mode_set_indicator(increment_wrap(mode_current, mode_count));
}

void mode_decrement_indicator() {
    mode_set_indicator(decrement_wrap(mode_current, mode_count));
}

void mode_increment() {
    mode_set(increment_wrap(mode_current, mode_count));
}

void mode_decrement() {
    mode_set(decrement_wrap(mode_current, mode_count));
}


//edit operations ==================================================================================================================

void edit_hsv_update(uint16_t keycode) {

    switch (keycode) {
        case KEY_EDIT_HSV_SAD:
            hsv_decrease_sat();
            break;
        case KEY_EDIT_HSV_SAI:
            hsv_increase_sat();
            break;
        case KEY_EDIT_HSV_VAD:
            hsv_decrease_val();
            break;
        case KEY_EDIT_HSV_VAI:
            hsv_increase_val();
            break;
        case KEY_EDIT_HSV_HUD:
            hsv_decrease_hue();
            break;
        case KEY_EDIT_HSV_HUI:
            hsv_increase_hue();
            break;
        default:
            break;
    }
}

void edit_hsv_exit(void) {
    HSV hsv = hsv_get_current();
    eeprom_update_hsv(mode_current, hsv.h, hsv.s, hsv.v);
}

void edit_mode_indication_update(uint16_t keycode) {

    switch (keycode) {
        case KEY_EDIT_MODE_INDICATION_UP:
            mode_indication = increment(mode_indication, MI_LAST);
            break;
        case KEY_EDIT_MODE_INDICATION_DOWN:
            mode_indication = decrement(mode_indication);
            break;
        default:
            break;
    }

    mode_set(mode_current);
}

void edit_mode_indication_exit(void) {
    eeprom_update_mode_indication(mode_indication);
}

void edit_mode_count_update(uint16_t keycode) {
    uprintf("mode_update pre %u, %u\n", mode_count < MAX_MODES);
    switch (keycode) {
        case KEY_EDIT_MODE_COUNT_UP:
            mode_count = increment(mode_count, MAX_MODES);
            break;
        case KEY_EDIT_MODE_COUNT_DOWN:
            mode_count = decrement(mode_count);
            break;
        default:
            break;
    }
    uprintf("mode_update post %u, %u\n", mode_count < MAX_MODES);
    hsv_enable_binary(mode_count);
    hsv_set(0,0,hsv_get_current().v);
}

void edit_mode_count_exit(void) {
    eeprom_update_mode_count(mode_count);
    mode_set(0);
}

//qmk overrides ==================================================================================================================

void eeconfig_init_kb() {
   beadpad_eeprom_init(true);
}

void keyboard_post_init_kb(void) {

    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    } else {
        beadpad_eeprom_init(false);
    }

    hsv_init();

   // rgblight_enable_noeeprom();

    mode_count = eeprom_read_mode_count();
    mode_indication = eeprom_read_mode_indication();

    wait_ms(100);
    mode_set(0);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    handle_key(keycode, record->event.pressed);
    return false;
}

void encoder_update_kb(uint8_t index, bool clockwise) {
   handle_key(clockwise ? ROT_CW : ROT_CCW, false);
}

void suspend_power_down_kb(void) {
    hsv_enable(false);
}

void suspend_wakeup_init_kb(void) {
    mode_set(mode_current);
}


// layout isn't intended to be changed, behaviour is controlled by a listener script on the host
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KEY1, KEY2, KEY3, KEY4, KEY5, ROT_BUT)
};
