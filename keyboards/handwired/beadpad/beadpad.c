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

uint16_t key_mode_up;
uint16_t key_mode_down;

__attribute__((weak)) void beadpad_init(void) {  }
__attribute__((weak)) void check_mode_hold(void) {  }
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


//setting operations ==================================================================================================================

bool check_setting_update(uint16_t keycode) {
    //loop though each setting, if enabled and the key is down, action the new key

    for (uint8_t i = 0; i < SETTING_COUNT; i++) {
        struct setting setting = settings[i];

        if (setting.enabled && keycode != setting.keycode && keystate[setting.keycode] != NONE) {
            setting.update(keycode);
            keystate[setting.keycode] = CONFIG;
            return true;
        }
    }
    return false;
}

bool check_setting_complete(uint16_t keycode) {

    //loop though each setting, if enabled and the key was active, complete the setting
    for (uint8_t i = 0; i < SETTING_COUNT; i++) {
        struct setting setting = settings[i];

        if (setting.enabled && keycode == setting.keycode && keystate[setting.keycode] == CONFIG) {
            setting.complete();
            keystate[setting.keycode] = NONE;
            return true;
        }
    }
    return false;
}

void setting_update_hsv(uint16_t keycode) {

    switch (keycode) {
        case SETTING_HSV_KEY_SAD:
            hsv_decrease_sat();
            break;
        case SETTING_HSV_KEY_SAI:
            hsv_increase_sat();
            break;
        case SETTING_HSV_KEY_VAD:
            hsv_decrease_val();
            break;
        case SETTING_HSV_KEY_VAI:
            hsv_increase_val();
            break;
        case SETTING_HSV_KEY_HUD:
            hsv_decrease_hue();
            break;
        case SETTING_HSV_KEY_HUI:
            hsv_increase_hue();
            break;
        default:
            break;
    }
}

void setting_complete_hsv(void) {
    HSV hsv = hsv_get_current();
    eeprom_update_hsv(mode_current, hsv.h, hsv.s, hsv.v);
}

void setting_update_mode_indication(uint16_t keycode) {

    switch (keycode) {
        case SETTING_MODE_INDICATION_KEY_UP:
            mode_indication = increment(mode_indication, MI_LAST);
            break;
        case SETTING_MODE_INDICATION_KEY_DOWN:
            mode_indication = decrement(mode_indication, 0);
            break;
        default:
            break;
    }

    mode_set(mode_current);
}

void setting_complete_mode_indication(void) {
    eeprom_update_mode_indication(mode_indication);
}

void setting_update_mode_count(uint16_t keycode) {

    switch (keycode) {
        case SETTING_MODE_COUNT_KEY_UP:
            mode_count = increment(mode_count, MAX_MODES);
            break;
        case SETTING_MODE_COUNT_KEY_DOWN:
            mode_count = decrement(mode_count, 1);
            break;
        default:
            break;
    }

    hsv_enable_binary(mode_count);
    hsv_set(0,0,hsv_get_current().v);
}

void setting_complete_mode_count(void) {
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

    mode_count = eeprom_read_mode_count();
    mode_indication = eeprom_read_mode_indication();

    settings[SETTING_HSV_IDX].keycode = SETTING_HSV_KEY;
    settings[SETTING_HSV_IDX].update = setting_update_hsv;
    settings[SETTING_HSV_IDX].complete = setting_complete_hsv;

    settings[SETTING_MODE_INDICATION_IDX].keycode = SETTING_MODE_INDICATION_KEY;
    settings[SETTING_MODE_INDICATION_IDX].update = setting_update_mode_indication;
    settings[SETTING_MODE_INDICATION_IDX].complete = setting_complete_mode_indication;

    settings[SETTING_MODE_COUNT_IDX].keycode = SETTING_MODE_COUNT_KEY;
    settings[SETTING_MODE_COUNT_IDX].update = setting_update_mode_count;
    settings[SETTING_MODE_COUNT_IDX].complete = setting_complete_mode_count;

    hsv_init();
    beadpad_init();

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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{ KEYS, ROT_BUT }}};



