
//TODO - update readme
//TODO - update info.json
//TODO - add ifndefs for all users vars
//TODO - long press should give feedback when the timer is hit rather than when released
//TODO - comment


//BUG
// mode down not working
//mode hold goes active, which means it triggers the edit end function


//do we need to update lighting as the user changes edit setting?
//is there a way to indicate mode hold has kicked in without getting in the way?




#include "beadpad_eeprom.h"
#include "beadpad_led.h"

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

uint16_t mode_hold_timer;

uint16_t key_mode_up = KEY_MODE_UP;
uint16_t key_mode_down = KEY_MODE_DOWN;

void beadpad_eeprom_init(bool force) {

    #ifdef PRESERVE_CUSTOMISATION
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


bool mode_change_held(uint16_t keycode) {
    return keystate[keycode] == PRESSED && timer_elapsed(mode_hold_timer) > MODE_HOLD_TERM;
}

void matrix_scan_user(void) {

    #ifdef KEY_MODE_UP_HOLD
    if (mode_change_held(KEY_MODE_UP_HOLD)) {
        keystate[KEY_MODE_UP_HOLD] = ACTIVE;
        mode_increment_indicator();
    }
    #endif

    #ifdef KEY_MODE_DOWN_HOLD
    if (mode_change_held(KEY_MODE_DOWN_HOLD)) {
        keystate[KEY_MODE_DOWN_HOLD] = ACTIVE;
        mode_decrement_indicator();
    }
    #endif
}

//hod to know if this has happened? store a bool? do the same check?
//if we store a bool, when do we reset?


void handle_key(uint16_t keycode, bool pressed) {




    //if a key is active and it changes, then handle and return, this is used for editing eeprom

    // uprintf("keycode: %u, pressed:%u, mode: %u, mode_in: %u\n", keycode,pressed,mode_current, mode_indication);

    // for(uint8_t k=0;k<8;k++) {
    // uprintf("keycode: %u, state: %u\n", k,  keystate[k]);
    // }

    //to do mode hold chnage on ready:
        //matrix scan kb would need to be running if the value is true, then set the colour (could yuo set mode? - would have to)
        //but if an edit kicks in...
        //clear trigger - to cancel matrix scan check
        //needs to go back to real mode,
        //alternnatively, could blink light.

    //the same key could be an edit, and we've just set it to active... that's ok
    //how do we roll back layer if we enter an edit mode?




    if(keystate[keycode] == ACTIVE) {
        keystate[keycode] = NONE;

        #ifdef EDIT_HSV_ENABLE
        if (keycode == KEY_EDIT_HSV) {
            edit_hsv_exit();
            uprintf("a\n");
        }
        #endif

        #ifdef EDIT_MODE_INDICATION_ENABLE
        if (keycode == KEY_EDIT_MODE_INDICATION) {
            edit_mode_indication_exit();
            uprintf("b\n");
        }
        #endif

        #ifdef EDIT_MODE_COUNT_ENABLE
        if (keycode == KEY_EDIT_MODE_COUNT) {
            edit_mode_count_exit();
            uprintf("c\n");
        }
        #endif

        return;
    } else {
        keystate[keycode] = pressed ? PRESSED : NONE;
        uprintf("d\n");
    }

    //only special cases occur on the press
    if(pressed) {

        //special case - mode hold
        #ifdef KEY_MODE_UP_HOLD
        if (keycode == KEY_MODE_UP_HOLD) {
            mode_hold_timer = timer_read();
            uprintf("e\n");
        }
        #endif
        #ifdef KEY_MODE_DOWN_HOLD
        if (keycode == KEY_MODE_DOWN_HOLD) {
            mode_hold_timer = timer_read();
            uprintf("f\n");
        }
        #endif

    } else {

        //special case - editing on the fly
        #ifdef EDIT_HSV_ENABLE
        if (keycode != KEY_EDIT_HSV && keystate[KEY_EDIT_HSV] != NONE) {
            edit_hsv_update(keycode);
            keystate[KEY_EDIT_HSV] = ACTIVE;
            uprintf("g\n");
            return;
        }
        #endif

        #ifdef EDIT_MODE_INDICATION_ENABLE
        if (keycode != KEY_EDIT_MODE_INDICATION && keystate[KEY_EDIT_MODE_INDICATION] != NONE) {
            edit_mode_indication_update(keycode);
            keystate[KEY_EDIT_MODE_INDICATION] = ACTIVE;
            uprintf("h\n");
            return;
        }
        #endif

        #ifdef EDIT_MODE_COUNT_ENABLE
        if (keycode != KEY_EDIT_MODE_COUNT && keystate[KEY_EDIT_MODE_COUNT] != NONE) {
            edit_mode_count_update(keycode);
            keystate[KEY_EDIT_MODE_COUNT] = ACTIVE;
            uprintf("i\n");
            return;
        }
        #endif

        //special case - mode hold
        #ifdef KEY_MODE_UP_HOLD
        if (mode_change_held(KEY_MODE_UP_HOLD)) {
            keycode = KEY_MODE_UP;
            uprintf("j\n");
        }
        #endif
        #ifdef KEY_MODE_DOWN_HOLD
        if (mode_change_held(KEY_MODE_DOWN_HOLD)) {
            keycode = KEY_MODE_DOWN;
            uprintf("k\n");
        }
        #endif

        //special case - rot mode change (press and turn)
        #ifdef MODE_ROT_ADJUST_ENABLED
        if (keystate[ROT_BUT] != NONE) {
            if(keycode == ROT_CW)
                keycode = KEY_MODE_UP;
            else if(keycode == ROT_CCW)
                keycode = KEY_MODE_DOWN;
        }
        #endif
        uprintf("l\n");
        //normal key processing
        switch (keycode) {
            case KEY_MODE_UP:
                mode_increment();
                uprintf("m\n");
                break;
            case KEY_MODE_DOWN:
                mode_decrement();
                uprintf("n\n");
                break;
            default:
               // send_keypress(keycode);
               uprintf("o\n");
                break;
        }
    }
}

