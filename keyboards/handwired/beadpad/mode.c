
//TODO - update readme
//TODO - update info.json
//TODO - add ifndefs for all users vars
//TODO - long press should give feedback when the timer is hit rather than when released
//TODO is there more eepromm, where does via store things?
//TODO - remove need for rgblight_enable and use completely custom rgb code?
//TODO - remove need for eeprom keymaps?
//TODO - disable, QMK use of eeprom completely?
//TODO - use *_kb functions and weakly define downstream funcs
//TODO - comment

//FEATURE - SPECIFY WHICH LEDS INDICATE STATE
//FEATURE - BINARY MODE INDICATION


// to use rgblight, can we disbale it in rules, but then include the file in rules?
//or go completely custom like: /qmk_firmware/keyboards/wilba_tech/wt_rgb_backlight.c

#include "beadpad_eeprom.h"

//mode count check
#if MODE_COUNT > MAX_MODES
#error MODES must be <= 8
#endif

const int MODE_BITS_ARR[] = MODE_BITS ;
const int KEY_BITS_ARR[] = KEY_BITS ;

#ifndef MODE_HOLD_TERM
#define MODE_HOLD_TERM 500
#endif

#ifndef MODE_UP
#define MODE_UP 0xF0
#endif

#ifndef MODE_DOWN
#define MODE_DOWN 0xF1
#endif

uint16_t mode_hold_timer;

void eeprom_init_hsv(bool force) {

#ifdef PRESERVE_CUSTOMISATION
    if (!force)
        return;
#endif

    if(force || !eeprom_is_valid()) {

        eeprom_update_hsv(0, HSV_MODE_0);
        eeprom_update_hsv(1, HSV_MODE_1);
        eeprom_update_hsv(2, HSV_MODE_2);
        eeprom_update_hsv(3, HSV_MODE_3);
        eeprom_update_hsv(4, HSV_MODE_4);
        eeprom_update_hsv(5, HSV_MODE_5);
        eeprom_update_hsv(6, HSV_MODE_6);
        eeprom_update_hsv(7, HSV_MODE_7);

        eeprom_update_mode_count(MODE_COUNT);

        eeprom_set_valid(true);
    }

}

void send_keypress(uint16_t keycode) {
   uprintf("keycode: %u\n", keycode);

    uint8_t input, bit;

    input = current_mode;
    for(bit = 0; bit < MODE_BITS_SIZE; ++bit, input >>= 1) {
       uprintf("mode, bit %u:\t%u\t%u\n", bit, input & 1, MODE_BITS_ARR[bit]);
        if (input & 1)
            tap_code(MODE_BITS_ARR[bit]);
    }
    input = keycode;
    for(bit = 0; bit < KEY_BITS_SIZE; ++bit, input >>= 1) {
       uprintf("key, bit %u:\t%u\t%u\n", bit, input & 1, KEY_BITS_ARR[bit]);
        if (input & 1)
            tap_code(KEY_BITS_ARR[bit]);
    }

    tap_code(TERMINATOR);
    uprintf("mode: %u\n", current_mode);
}




void handle_key(uint16_t keycode, bool pressed) {


    // if(keycode == KEY4) {
    //     for(uint16_t addr = EECONFIG_SIZE; addr < 0xFF; addr++ ) {
    //         uint8_t *paddr = (uint8_t *)addr;
    //         eeprom_update_byte(paddr, addr % 0xFF);
    //         wait_ms(300);
    //         uprintf("addr: %u, val: %u\n",addr, eeprom_read_byte(paddr));
    //     }
    // }

    // if(keycode == KEY5) {
    //     for(uint16_t addr = EECONFIG_SIZE; addr < 0xFF; addr++ ) {
    //         uint8_t *paddr = (uint8_t *)addr;
    //         //eeprom_update_byte(paddr, addr % 0xFF);
    //         wait_ms(300);
    //         uprintf("addr: %u, val: %u\n",addr, eeprom_read_byte(paddr));
    //     }
    // }

    //if a key is active and it changes, then reset status and return
    if(keystate[keycode] == ACTIVE) {
        keystate[keycode] = NONE;

        #ifdef EDIT_HSV_ENABLE
        if (keycode == KEY_EDIT_HSV) {
            edit_hsv_exit();
        }
        #endif

        #ifdef EDIT_MODE_COUNT_ENABLE
        if (keycode == KEY_EDIT_MODE_COUNT) {
            edit_mode_count_exit();
        }
        #endif

        return;
    } else {
        keystate[keycode] = pressed ? PRESSED : NONE;
    }

    //only special cases occur on the press
    if(pressed) {

        //special case - mode hold
        #ifdef MODE_UP_HOLD
        if (keycode == MODE_UP_HOLD) {
            mode_hold_timer = timer_read();
        }
        #endif
        #ifdef MODE_DOWN_HOLD
        if (keycode == MODE_DOWN_HOLD) {
            mode_hold_timer = timer_read();
        }
        #endif

    } else {

        //special case - set RGB for current layer
        #ifdef EDIT_HSV_ENABLE
        if (keycode != KEY_EDIT_HSV && keystate[KEY_EDIT_HSV] != NONE) {

            edit_hsv_update(keycode);

            keystate[KEY_EDIT_HSV] = ACTIVE;
            //edit_state = ES_HSV;

            return;
        }
        #endif

        #ifdef EDIT_MODE_COUNT_ENABLE
        if (keycode != KEY_EDIT_MODE_COUNT && keystate[KEY_EDIT_MODE_COUNT] != NONE) {

            edit_mode_count_update(keycode);

            keystate[KEY_EDIT_MODE_COUNT] = ACTIVE;
            //edit_state = ES_HSV;

            return;
        }
        #endif

        //special case - mode hold
        #ifdef MODE_UP_HOLD
        if (keycode == MODE_UP_HOLD && timer_elapsed(mode_hold_timer) > MODE_HOLD_TERM) {
            keycode = MODE_UP;
        }
        #endif
        #ifdef MODE_DOWN_HOLD
        if (keycode == MODE_DOWN_HOLD && timer_elapsed(mode_hold_timer) > MODE_HOLD_TERM) {
            keycode = MODE_DOWN;
        }
        #endif

        //special case - rot mode change (press and turn)
        #ifdef MODE_ROT_ADJUST_ENABLED
        if (keystate[ROT_BUT] != NONE) {
            if(keycode == ROT_CW)
                keycode = MODE_UP;
            else if(keycode == ROT_CCW)
                keycode = MODE_DOWN;
        }
        #endif

        //normal key processing
        switch (keycode) {
            case MODE_UP:
                mode_increment();
                break;
            case MODE_DOWN:
                mode_decrement();
                break;
            default:
               // send_keypress(keycode);
                break;
        }
    }
}

