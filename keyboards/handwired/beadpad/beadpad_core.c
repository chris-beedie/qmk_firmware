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

//TODO - update readme
//TODO - update info.json
//TODO - add ifndefs for all users vars
//TODO - comment
//TODO check invalid define options here
//TODO - change rot button to dedicated pin to clean up keymap
//TODO sleep not working







//mode count check
#if MODE_COUNT > MAX_MODES
#error MODES must be <= 8
#endif

const int MODE_BITS_ARR[] = MODE_BITS ;
const int KEY_BITS_ARR[] = KEY_BITS ;

#ifndef MODE_HOLD_TERM
#define MODE_HOLD_TERM 500
#endif

//mode up and down need to be defined with out of range and different values.
#ifndef KEY_MODE_UP
#define KEY_MODE_UP 0xF0
#endif

#ifndef KEY_MODE_DOWN
#define KEY_MODE_DOWN 0xF1
#endif

#include "beadpad_util.c"
#include "beadpad_led.c"
#include "beadpad_eeprom.c"
#include "beadpad_mode.c"
#include "beadpad_settings.c"

//convert and send our mode/key in encoded message
void send_keypress(uint8_t mode, uint16_t keycode) {
   //uprintf("keycode: %u\n", keycode);

    uint8_t input, bit;

    input = mode;
    for(bit = 0; bit < MODE_BITS_SIZE; ++bit, input >>= 1) {
       //uprintf("mode, bit %u:\t%u\t%u\n", bit, input & 1, MODE_BITS_ARR[bit]);
        if (input & 1)
            tap_code(MODE_BITS_ARR[bit]);
    }
    input = keycode;
    for(bit = 0; bit < KEY_BITS_SIZE; ++bit, input >>= 1) {
       //uprintf("key, bit %u:\t%u\t%u\n", bit, input & 1, KEY_BITS_ARR[bit]);
        if (input & 1)
            tap_code(KEY_BITS_ARR[bit]);
    }

    tap_code(TERMINATOR);
    //uprintf("mode: %u\n", mode_current);
}


//main method for porcessing what action to take given the state of the keys
void handle_key(uint16_t keycode, bool pressed) {


    uprintf("here: %u, %u, %u, %u\n", KEY2, KEY3, ROT_BUT, ROT_CCW);


    //check and processs any finishing setting and exit further key handling
    if(settings_try_complete(keycode)) {
        return;
    }

     //only special cases occur on the press
    if(pressed) {
        keystate[keycode] = PRESSED;

        mode_hold_try_start(keycode);
    } else {
        keystate[keycode] = NONE;

        if(settings_try_update(keycode)) {
            return;
        }

        keycode = mode_hold_try_action(keycode);
        keycode = mode_rot_adjust_try_action(keycode);
;
        switch (keycode) {
            case KEY_MODE_UP:
                mode_increment();
                break;
            case KEY_MODE_DOWN:
                mode_decrement();
                break;
            default:
               // send_keypress(mode_get_current(), keycode);
                break;
        }
    }
}

//initialisation =========================================================================================================

void beadpad_init(void) {

    mode_init();
    settings_init();

}

//store the user defined settings in eeprom
void beadpad_eeprom_init(bool force) {

    #ifdef PRESERVE_SETTINGS
    if (!force)
        return;
    #endif

    if(force || !eeprom_is_valid()) {

        eeprom_update_mode_count(MODE_COUNT);
        eeprom_update_mode_indication(MODE_INDICATION);

        eeprom_update_mode_hsv(0, hsv_pack(HSV_MODE_0));
        eeprom_update_mode_hsv(1, hsv_pack(HSV_MODE_1));
        eeprom_update_mode_hsv(2, hsv_pack(HSV_MODE_2));
        eeprom_update_mode_hsv(3, hsv_pack(HSV_MODE_3));
        eeprom_update_mode_hsv(4, hsv_pack(HSV_MODE_4));
        eeprom_update_mode_hsv(5, hsv_pack(HSV_MODE_5));
        eeprom_update_mode_hsv(6, hsv_pack(HSV_MODE_6));
        eeprom_update_mode_hsv(7, hsv_pack(HSV_MODE_7));

        eeprom_set_valid(true);
    }
}

// quantum overrides =================================================================================================

void eeconfig_init_user() {
   beadpad_eeprom_init(true);
}

void keyboard_post_init_user(void) {

    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    } else {
        beadpad_eeprom_init(false);
    }

    beadpad_init();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    handle_key(keycode, record->event.pressed);
    return false;
}

void encoder_update_user(uint8_t index, bool clockwise) {
   handle_key(clockwise ? ROT_CW : ROT_CCW, false);
}

void matrix_scan_user(void) {
    mode_hold_check();
}


// void suspend_power_down_kb(void) {
//     hsv_enable(false);
// }

// void suspend_wakeup_init_kb(void) {
//     mode_refresh();
// }





