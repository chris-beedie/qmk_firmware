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
//TODO - remove print import and cleanup
//TODO - refactor some of this to *_kb

#include <print.h>
#include QMK_KEYBOARD_H

#define MODES 6 // number of modes to switch through
//todo - make sure this number isn't higher than the number of modes supported

#define HSV_MODE_0 191, 255, 100 //purple
#define HSV_MODE_1 0, 255, 100 //red
#define HSV_MODE_2 85, 255, 100 //green











//TODO define elswhere up to a maximum number
#define HSV_DEFAULT_MODE_0 191, 255, 255 //purple
#define HSV_DEFAULT_MODE_1 0, 255, 255 //red
#define HSV_DEFAULT_MODE_2 85, 255, 255 //green
#define HSV_DEFAULT_MODE_3 128, 255, 100 //cyan
#define HSV_DEFAULT_MODE_4 170, 255, 100 //blue
#define HSV_DEFAULT_MODE_5 43, 255, 100 //yellow

#ifndef HSV_MODE_3
#define HSV_MODE_3 HSV_DEFAULT_MODE_3
#endif
#ifndef HSV_MODE_4
#define HSV_MODE_4 HSV_DEFAULT_MODE_4
#endif
#ifndef HSV_MODE_5
#define HSV_MODE_5 HSV_DEFAULT_MODE_5
#endif
#ifndef HSV_MODE_6
#define HSV_MODE_6 HSV_DEFAULT_MODE_6
#endif



//TODO - long press of mode goes down


static uint8_t current_mode;
static uint16_t current_modifiers;

#define KEY1 KC_1
#define KEY2 KC_2
#define KEY3 KC_3
#define KEY4 KC_4

#define MOD_MODE_0 KC_NO
#define MOD_MODE_1 QK_LSFT
#define MOD_MODE_2 QK_LCTL
#define MOD_MODE_3 QK_LALT
#define MOD_MODE_4 QK_LGUI
#define MOD_MODE_5 (QK_LCTL | QK_LALT)
#define MOD_MODE_6 (QK_LCTL | QK_LGUI)
#define MOD_MODE_7 (QK_LCTL | QK_LALT | QK_LGUI)

// 0 = no mod
// 1 = ctrl
// 2 = alt
// 3 = win
// 4 = ctrl+alt
// 5 = ctrl+win
// 6 = alt+win
// 7 = ctrl+alt+win


/*
  KC_LCTRL = 0xE0,
    KC_LSHIFT,
    KC_LALT,
    KC_LGUI,
    KC_RCTRL,
    KC_RSHIFT,
    KC_RALT,
    KC_RGUI

    */

enum my_keycodes {
  MODE = SAFE_RANGE,
  KEY1a,
  KEY2b,
  KEY3c,
  KEY4d
};

void set_mode(uint8_t mode) {

    current_mode = mode;

    switch (current_mode) {
        case 0:
            rgblight_sethsv_noeeprom(HSV_MODE_0);
            current_modifiers = MOD_MODE_0;
            break;
        case 1:
            rgblight_sethsv_noeeprom(HSV_MODE_1);
            current_modifiers = MOD_MODE_1;
            break;
        case 2:
            rgblight_sethsv_noeeprom(HSV_MODE_2);
            current_modifiers = MOD_MODE_2;
            break;
        case 3:
            rgblight_sethsv_noeeprom(HSV_MODE_3);
            current_modifiers = MOD_MODE_3;
            break;
        case 4:
            rgblight_sethsv_noeeprom(HSV_MODE_4);
            current_modifiers = MOD_MODE_4;
            break;
        case 5:
            rgblight_sethsv_noeeprom(HSV_MODE_5);
            current_modifiers = MOD_MODE_5;
            break;
        default:
            rgblight_sethsv_noeeprom(0,0,0);
            current_modifiers = KC_NO;
            break;
    }

}



void press_mode_kc(uint16_t keycode) {
    uprintf("modifiers: %u\tkeycode: %u\n", current_modifiers,keycode);
    register_code16(current_modifiers);
    tap_code(keycode);
    unregister_code16(current_modifiers);
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( MODE, KEY1, KEY2, KEY3, KEY4)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        uprintf("\nKEY EVENT\n");
        uprintf("current_mode: %u\n", current_mode);
    }

    switch (keycode) {
        case MODE:
            if (record->event.pressed) {
                set_mode((current_mode + 1) % MODES);

                uprintf("new mode: %u\n", current_mode);
            }
            return false;
        case KEY1 ... KEY4:
            if (record->event.pressed) {

                press_mode_kc(keycode);

            }
            return false;
        default:
            uprintf("INVALID KEY\n");
            return true; // Process all other keycodes normally
    }
}

void keyboard_post_init_user(void) {
    //TODO - read/write value in eeprom to remember last mode - https://beta.docs.qmk.fm/using-qmk/guides/custom_quantum_functions#persistent-configuration-eeprom

    rgblight_enable_noeeprom(); // enables Rgb, without saving settings
    set_mode(0);
}








//todo add encoder button to matrix or at least as direct switch

// do we need to set: #define TAP_CODE_DELAY 100 in your config.h

// #ifdef ENCODER_ENABLE
// void encoder_update_user(uint8_t index, bool clockwise) {
//     //TODO - this needs to respond to whichever layer is active
//     if (clockwise) {
//         tap_code(KC_VOLU);
//     } else {
//         tap_code(KC_VOLD);
//     }
// }
// #endif

//todo - light indicating mode
//ifdef rgbendable
//blah blah reddit



/* encoder layers

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {

    switch (get_highest_layer(layer_state)) {
        case 0:

            break;
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        default: //  for any other layers, or the default layer
            break;
    }


    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
}
#endif

*/

