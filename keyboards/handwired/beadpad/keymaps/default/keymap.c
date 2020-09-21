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
#include <print.h>
#include QMK_KEYBOARD_H

#define MODES 6 // number of modes to switch through
//todo - make sure this number isn't higher than the number of modes supported

#define HSV_MODE_0 191, 255, 100 //purple
#define HSV_MODE_1 0, 255, 100 //red
#define HSV_MODE_2 85, 255, 100 //green
#define HSV_MODE_3 128, 255, 100 //cyan
#define HSV_MODE_4 170, 255, 100 //blue
#define HSV_MODE_5 43, 255, 100 //yellow

enum my_keycodes {
  MODE = SAFE_RANGE,
  KEY1,
  KEY2,
  KEY3,
  KEY4,
  KEY5
};

void update_mode_hsv(uint8_t mode) {

    switch (mode) {
        case 0:
            rgblight_sethsv_noeeprom(HSV_MODE_0);
            break;
        case 1:
            rgblight_sethsv_noeeprom(HSV_MODE_1);
            break;
        case 2:
            rgblight_sethsv_noeeprom(HSV_MODE_2);
            break;
        case 3:
            rgblight_sethsv_noeeprom(HSV_MODE_3);
            break;
        case 4:
            rgblight_sethsv_noeeprom(HSV_MODE_4);
            break;
        case 5:
            rgblight_sethsv_noeeprom(HSV_MODE_5);
            break;
        default:
            rgblight_sethsv_noeeprom(0,0,0);
            break;
    }

}

void set_mode_mods(uint8_t mode, bool press) {
    //press/release all mods based on layer

// 0 = no mod
// 1 = ctrl
// 2 = alt
// 3 = win
// 4 = ctrl+alt
// 5 = ctrl+win
// 6 = alt+win
// 7 = ctrl+alt+win
}

// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//     [0] = LAYOUT(
//         MODE,       KC_1,      KC_2,       KC_3,      KC_4
//     ),
//     [1] = LAYOUT(
//         MODE,       KC_5,      KC_6,       KC_7,      KC_8
//     ),
//     [2] = LAYOUT(
//         MODE,       KC_9,      KC_0,       KC_Q,      KC_W
//     ),
//     [3] = LAYOUT(
//         MODE,       S(KC_1),      S(KC_2),       S(KC_3),      S(KC_4)
//     ),
//     [4] = LAYOUT(
//         MODE,       S(KC_5),      S(KC_6),       S(KC_7),      S(KC_8)
//     ),
//     [5] = LAYOUT(
//         MODE,       S(KC_9),      S(KC_0),       S(KC_Q),      S(KC_W)
//     )

// };


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT( MODE, KEY1,    KEY2,    KEY3,    KEY4    ),
    [1] = LAYOUT( MODE, _______, _______, _______, _______ ),
    [2] = LAYOUT( MODE, _______, _______, _______, _______ ),
    [3] = LAYOUT( MODE, _______, _______, _______, _______ ),
    [4] = LAYOUT( MODE, _______, _______, _______, _______ ),
    [5] = LAYOUT( MODE, _______, _______, _______, _______ )

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    uprintf("\nKEY EVENT\n");
    uprintf("layer_state: %u\n", get_highest_layer(layer_state));

    switch (keycode) {
        case MODE:
            if (record->event.pressed) {
            uint8_t new_layer = (get_highest_layer(layer_state) + 1) % MODES;

            layer_move(new_layer);

            uprintf("layer should be: %u\n", new_layer);
            uprintf("layer is now: %u\n", get_highest_layer(layer_state));
            }
            return false; // Skip all further processing of this key
        case KEY1 ... KEY3:
            if (record->event.pressed) {
                uprintf("keycode: %u\n", keycode);
            }
            return false;
        case KEY4:
            if (record->event.pressed) {
                uprintf("key 4 is go");
            }
            return false;
        default:
            uprintf("HSV: %u, %u, %u\n", keycode);
            return true; // Process all other keycodes normally
    }
}


layer_state_t layer_state_set_user(layer_state_t state) {
    uprintf("\nSET STATE!!\n");
     //uprintf("layer_state: %u\n", get_highest_layer(layer_state));
     //uprintf("state: %u\n", get_highest_layer(state));

    update_mode_hsv(get_highest_layer(state));

    return state;
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom(); // enables Rgb, without saving settings
    update_mode_hsv(get_highest_layer(layer_state));
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

