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
#include <print.h>
#include QMK_KEYBOARD_H




// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//     /* TODO - can this be a function? i.e. can i use a const max layer and toggles based on that */
//     [0] = LAYOUT(
//         TO(1),       KC_1,      KC_2,       KC_3,      KC_4
//     ),
//     [1] = LAYOUT(
//         TO(2),       KC_5,      KC_6,       KC_7,      KC_8
//     ),
//     [2] = LAYOUT(
//         TO(3),       KC_9,      KC_0,       KC_Q,      KC_W
//     ),
//     [3] = LAYOUT(
//         TO(4),       S(KC_1),      S(KC_2),       S(KC_3),      S(KC_4)
//     ),
//     [4] = LAYOUT(
//         TO(5),       S(KC_5),      S(KC_6),       S(KC_7),      S(KC_8)
//     ),
//     [5] = LAYOUT(
//         TO(0),       S(KC_9),      S(KC_0),       S(KC_Q),      S(KC_W)
//     )

// };

/*


for encoder, we could reserve f22/23/24



0 = no mod
1 = ctrl
2 = alt
3 = win
4 = ctrl+alt
5 = ctrl+win
6 = alt+win
7 = ctrl+alt+win


hanlde everything as custom kc for bp1m bp2 etc
then add layer based modifiers
this could be a function so that rotary encoder does it too



*/




//todo add encoder button to matrix or at least as direct switch

// do we need to set: #define TAP_CODE_DELAY 100 in your config.h



/* allowing max layer to be specified

//should we explictly set layer at startup?

//do we need current_layer, can we not just do get_highest_layer(layer_state)
//this also solves thr problem of know what boot status is
// static uint8_t current_layer = 0;
*/
#define HIGHEST_LAYER 6 // number of layers to switch through
//todo - make sure this number isn't higher than the number of layers supported

enum my_keycodes {
  MODE = SAFE_RANGE,
  KEY1,
  KEY2,
  KEY3,
  KEY4,
  KEY5
};

#define HSV_MODE_0 191, 255, 100 //purple
#define HSV_MODE_1 0, 255, 100 //red
#define HSV_MODE_2 85, 255, 100 //green
#define HSV_MODE_3 128, 255, 100 //cyan
#define HSV_MODE_4 170, 255, 100 //blue
#define HSV_MODE_5 43, 255, 100 //yellow

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
        default: //  for any other layers, or the default layer
            rgblight_sethsv_noeeprom(0,0,0);
            break;
    }

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

// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//     [0] = LAYOUT(
//         MODE,       KC_F13,      KC_F14,       KC_F15,      KC_F16
//     ),
//     [1] = LAYOUT(
//         MODE,       KC_F17,      KC_F18,       KC_F19,      KC_F20
//     ),
//     [2] = LAYOUT(
//         MODE,       KC_F21,      KC_F22,       KC_F23,      KC_F24
//     ),
//     [3] = LAYOUT(
//         MODE,       C(KC_F13),      C(KC_F14),       C(KC_F15),      C(KC_F16)
//     ),
//     [4] = LAYOUT(
//         MODE,       C(KC_F17),      C(KC_F18),       C(KC_F19),      C(KC_F20)
//     ),
//     [5] = LAYOUT(
//         MODE,       C(KC_F21),      C(KC_F22),       C(KC_F23),      C(KC_F24)
//     )

// };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        MODE,       RGB_HUI,      RGB_HUD,       RGB_SAI,      RGB_SAD
    ),
    [1] = LAYOUT(
        MODE,       KC_TRNS,      KC_TRNS,       KC_TRNS,      KC_TRNS
    ),
    [2] = LAYOUT(
        MODE,       KC_TRNS,      KC_TRNS,       KC_TRNS,      KC_TRNS
    ),
    [3] = LAYOUT(
        MODE,       KC_TRNS,      KC_TRNS,       KC_TRNS,      KC_TRNS
    ),
    [4] = LAYOUT(
        MODE,       KC_TRNS,      KC_TRNS,       KC_TRNS,      KC_TRNS
    ),
    [5] = LAYOUT(
        MODE,       KC_TRNS,      KC_TRNS,       KC_TRNS,      KC_TRNS
    )

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

     uprintf("\nKEY EVENT\n");
     uprintf("layer_state: %u\n", get_highest_layer(layer_state));

  switch (keycode) {
    case MODE:
      if (record->event.pressed) {
        uint8_t new_layer = (get_highest_layer(layer_state) + 1) % HIGHEST_LAYER;


        // uprintf("new layer: %u\n",(get_highest_layer(layer_state) + 1) % HIGHEST_LAYER);


        layer_move(new_layer);

         uprintf("layer should be: %u\n", new_layer);
         uprintf("layer is now: %u\n", get_highest_layer(layer_state));
      }
      return false; // Skip all further processing of this key
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
  //rgblight_sethsv_noeeprom(HSV_LAYER_5); // sets the color to teal/cyan without saving
//rgblight_setrgb (0x00,  0x00, 0xFF);
    update_mode_hsv(get_highest_layer(layer_state));
}








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

