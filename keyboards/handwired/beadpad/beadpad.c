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
#include "beadpad_eeprom.c"

uint8_t mode_count = MAX_MODES;
uint8_t current_mode = 0; //keep track of cuurent layer

__attribute__((weak)) void handle_key(uint16_t keycode, bool pressed) {  }
//__attribute__((weak)) void set_mode(uint8_t mode) {  }

//mode operations ==================================================================================================================

// sets the mode - just a case of changin colour and setting the var
void mode_set(uint8_t mode) {

    current_mode = mode;
    HSV mode_hsv = eeprom_read_hsv(mode);

    rgblight_sethsv_noeeprom(mode_hsv.h, mode_hsv.s, mode_hsv.v);

    uprintf("new mode: %u\n", current_mode);
}

void mode_increment(void) {
    //sets mode by incrementing and calling function, using modulus to restart at 0 if we go beyond the last one
    mode_set((current_mode + 1) % mode_count);
}

void mode_decrement(void) {
    if(current_mode == 0)
        current_mode = mode_count;

    mode_set(current_mode - 1);
}

//edit operations ==================================================================================================================

void edit_hsv_update(uint16_t keycode) {

    switch (keycode) {
        case KEY_EDIT_HSV_SAD:
            rgblight_decrease_sat_noeeprom();
            break;
        case KEY_EDIT_HSV_SAI:
            rgblight_increase_sat_noeeprom();
            break;
        case KEY_EDIT_HSV_VAD:
            rgblight_decrease_val_noeeprom();
            break;
        case KEY_EDIT_HSV_VAI:
            rgblight_increase_val_noeeprom();
            break;
        case KEY_EDIT_HSV_HUD:
            rgblight_decrease_hue_noeeprom();
            break;
        case KEY_EDIT_HSV_HUI:
            rgblight_decrease_hue_noeeprom();
            break;
        default:
            break;
    }
}

void edit_hsv_exit(void) {
    eeprom_update_hsv(current_mode, rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val());
}

void edit_mode_count_update(uint16_t keycode) {

        switch (keycode) {
        case KEY_EDIT_MODE_COUNT_UP:
            if(current_mode < mode_count)
                current_mode++;
            break;
        case KEY_EDIT_MODE_COUNT_DOWN:
            if(current_mode > 0)
                current_mode--;
            break;
        default:
            break;
    }

}

void edit_mode_count_exit(void) {
    eeprom_update_mode_count(current_mode);
    mode_set(0);
}

//qmk overrides ==================================================================================================================

void eeconfig_init_kb() {
   eeprom_init_hsv(true);
}

void keyboard_post_init_kb(void) {

    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    } else {
        eeprom_init_hsv(false);
    }

    mode_set(0);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    handle_key(keycode, record->event.pressed);
    return false;
}

void encoder_update_kb(uint8_t index, bool clockwise) {
    handle_key(clockwise ? ROT_CW : ROT_CCW, false);
}



// layout isn't intended to be changed, behaviour is controlled by a listener script on the host
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KEY1, KEY2, KEY3, KEY4, KEY5, ROT_BUT)
};
