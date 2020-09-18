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
#include QMK_KEYBOARD_H

/*
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     TODO - can this be a function? i.e. can i use a const max layer and toggles based on that
    [0] = LAYOUT(
        TO(1),       KC_F13,      KC_F14,       KC_F15,      KC_F16
    ),
    [1] = LAYOUT(
        TO(2),       KC_F17,      KC_F18,       KC_F19,      KC_F20
    ),
    [2] = LAYOUT(
        TO(3),       KC_F21,      KC_F22,       KC_F23,      KC_F24
    ),
    [3] = LAYOUT(
        TO(4),       C(KC_F13),      C(KC_F14),       C(KC_F15),      C(KC_F16)
    ),
    [4] = LAYOUT(
        TO(5),       C(KC_F17),      C(KC_F18),       C(KC_F19),      C(KC_F20)
    ),
    [5] = LAYOUT(
        TO(0),       C(KC_F21),      C(KC_F22),       C(KC_F23),      C(KC_F24)
    )

};
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* TODO - can this be a function? i.e. can i use a const max layer and toggles based on that */
    [0] = LAYOUT(
        TO(1),       KC_1,      KC_2,       KC_3,      KC_4
    ),
    [1] = LAYOUT(
        TO(2),       KC_5,      KC_6,       KC_7,      KC_8
    ),
    [2] = LAYOUT(
        TO(3),       KC_9,      KC_0,       KC_Q,      KC_W
    ),
    [3] = LAYOUT(
        TO(4),       S(KC_1),      S(KC_2),       S(KC_3),      S(KC_4)
    ),
    [4] = LAYOUT(
        TO(5),       S(KC_5),      S(KC_6),       S(KC_7),      S(KC_8)
    ),
    [5] = LAYOUT(
        TO(0),       S(KC_9),      S(KC_0),       S(KC_Q),      S(KC_W)
    )

};

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    //TODO - this needs to respond to whichever layer is active
    if (!clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
}
#endif
