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


//REFACTORRRRRR
/*
    move pack bits to config? make surte they add to 16
    hsv step values based on pack bits



    //files for:
        settings
        mode





*/


//TODO - refactor to allow number of keys to be changed, define these in setting.h?
//would need to do checks like making sure the KEY_BITS array and count all make sense
//can hsv increments be tied to number of bits?

#include "beadpad_eeprom.h"

#include "beadpad_mode.c"
#include "beadpad_settings.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{ KEYS, ROT_BUT }}};

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

enum keystate_t {
    NONE,
    PRESSED,
    SETTING,
    HELD
} keystate[TOTAL_KEY_COUNT];

uint16_t mode_hold_timer;

void beadpad_init(void) {

    //hsv_init();
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

        eeprom_update_hsv(0, HSV_MODE_0);
        eeprom_update_hsv(1, HSV_MODE_1);
        eeprom_update_hsv(2, HSV_MODE_2);
        eeprom_update_hsv(3, HSV_MODE_3);
        eeprom_update_hsv(4, HSV_MODE_4);
        eeprom_update_hsv(5, HSV_MODE_5);
        eeprom_update_hsv(6, HSV_MODE_6);
        eeprom_update_hsv(7, HSV_MODE_7);

        eeprom_set_valid(true);
    }
}

//convert and send our mode/key in encoded message
void send_keypress(uint16_t keycode) {
   //uprintf("keycode: %u\n", keycode);

    uint8_t input, bit;

    input = mode_current;
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

//helper to check if mode hold timer has expired
bool mode_change_held(void) {
    return timer_elapsed(mode_hold_timer) > MODE_HOLD_TERM;
}


//main method for porcessing what action to take given the state of the keys
void handle_key(uint16_t keycode, bool pressed) {

    //check and processs any finishing setting and exit further key handling
    if(settings_try_complete(keycode)) {
        return;
    }

     //only special cases occur on the press
    if(pressed) {
        keystate[keycode] = PRESSED;

        //special case - mode hold, start timers
        #ifdef KEY_MODE_UP_HOLD
        if (keycode == KEY_MODE_UP_HOLD) {
            mode_hold_timer = timer_read();
        }
        #endif
        #ifdef KEY_MODE_DOWN_HOLD
        if (keycode == KEY_MODE_DOWN_HOLD) {
            mode_hold_timer = timer_read();
        }
        #endif


    } else {
        keystate[keycode] = NONE;

        //special case - setting editing, check to see if a setting change is active, and handle it if so
        if(settings_try_update(keycode)) {
            return;
        }

        //special case - mode hold
        #ifdef KEY_MODE_UP_HOLD
        if (keycode == KEY_MODE_UP_HOLD  && mode_change_held()) {
            keycode = KEY_MODE_UP;
        }
        #endif
        #ifdef KEY_MODE_DOWN_HOLD
        if (keycode == KEY_MODE_DOWN_HOLD && mode_change_held()) {
            keycode = KEY_MODE_DOWN;
        }
        #endif

        //special case - rot mode change (press and turn)
        #ifdef MODE_ROT_ADJUST_ENABLED
        if (keystate[ROT_BUT] != NONE) {
            keycode = keycode == ROT_CW ? KEY_MODE_UP : KEY_MODE_DOWN;
            // if(keycode == ROT_CW)
            //     keycode = KEY_MODE_UP;
            // else if(keycode == ROT_CCW)
            //     keycode = KEY_MODE_DOWN;
        }
        #endif
        uprintf("l\n");
        //normal key processing
        switch (keycode) {
            case KEY_MODE_UP:
                mode_increment();
                break;
            case KEY_MODE_DOWN:
                mode_decrement();
                break;
            default:
               // send_keypress(keycode);
                break;
        }
    }
}

void matrix_scan_user(void) {

    //update indicator on mode hold so user knows they have held it long enough
    //mode change is committed by keyhandler in case an edit mode interupts

    #ifdef KEY_MODE_UP_HOLD
    if (keystate[KEY_MODE_UP_HOLD] == PRESSED && mode_change_held()) {
        keystate[KEY_MODE_UP_HOLD] = HELD;
        mode_increment_indicator();
    }
    #endif

    #ifdef KEY_MODE_DOWN_HOLD
    if (keystate[KEY_MODE_DOWN_HOLD] == PRESSED && mode_change_held()) {
        keystate[KEY_MODE_DOWN_HOLD] = HELD;
        mode_decrement_indicator();
    }
    #endif
}
