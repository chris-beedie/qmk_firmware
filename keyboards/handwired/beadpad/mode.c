
//TODO - update readme
//TODO - update info.json
//TODO - add ifndefs for all users vars
//TODO - long press should give feedback when the timer is hit rather than when released
//TODO is there more eepromm, where does via store things?
//TODO - REFACTOR/move code into own files
//TODO - remove need for rgblight_enable and use completely custom rgb code?
//TODO - remove need for eeprom keymaps?
//TODO - disable, QMK use of eeprom completely?
//TODO - use *_kb functions and weakly define downstream funcs
//TODO - comment

//FEATURE - SPECIFY WHICH LEDS INDICATE STATE
//FEATURE - BINARY MODE INDICATION


// to use rgblight, can we disbale it in rules, but then include the file in rules?
//or go completely custom like: /qmk_firmware/keyboards/wilba_tech/wt_rgb_backlight.c

//where are things like unicode init'd



#include "buildinfo.h"

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


// layout isn't intended to be changed, behaviour is controlled by a listener script on the host
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KEY1, KEY2, KEY3, KEY4, KEY5, ROT_BUT)
};


uint8_t mode_count = MODE_COUNT;
uint8_t current_mode; //keep track of cuurent layer
uint16_t mode_hold_timer;

enum keystate_t {
    NONE,
    PRESSED,
    ACTIVE
} keystate[KEY_COUNT];

uint16_t pack(uint8_t value, uint8_t bits, uint8_t offset) {

    //add on the bit below the mask, means the value will get rounded rather than truncated
    uint8_t _value = value + (1 << (SIZE_OF_BYTE - bits -1));

    //check operation above didn't cause an overflow
    if (_value < value)
        _value = 0xFF;

    //shift it until we are left with the desired number of bits and then reposition for packing
    return (_value >> (SIZE_OF_BYTE - bits)) << offset;
}

uint8_t unpack(uint16_t packed, uint8_t bits, uint8_t offset ) {

    //shift it back to the end, mask it, then restore to 8bit
    return ((packed >> offset) & ((1 << bits) - 1)) << (SIZE_OF_BYTE - bits);
}

uint16_t pack_hsv(uint8_t hue, uint8_t sat, uint8_t val) {

    //downsamples the values to the desired number of bits and packages them into a uint16
    //uprintf("hue: %u, sat: %u, val: %u\n", hue, sat, val);

    return (
        pack(hue, HUE_PACK_BITS, HUE_PACK_OFFSET) |
        pack(sat, SAT_PACK_BITS, SAT_PACK_OFFSET) |
        pack(val, VAL_PACK_BITS, VAL_PACK_OFFSET)
    );

}

HSV unpack_hsv(uint16_t packed_hsv) {

    return (HSV){
        .h = unpack(packed_hsv, HUE_PACK_BITS, HUE_PACK_OFFSET),
        .s = unpack(packed_hsv, SAT_PACK_BITS, SAT_PACK_OFFSET),
        .v = unpack(packed_hsv, VAL_PACK_BITS, VAL_PACK_OFFSET)
    };

}

//don't use rgblight address as this is written if enabling rgblight which is needed to include the rgb functions
//addresses defined clobber: keyboard, user, velocikey, haptic, rgbmatrix
const uint16_t mode_address[] = { 16, 18, 20, 22, 24, 26, 28, 30 };

#define BUILD_ADDR 5
#define MODE_COUNT_ADDR 7
#include "eeconfig.h"

uint8_t eeprom_read_mode_count(void) {
    return eeprom_read_byte((uint8_t *) MODE_COUNT_ADDR);
}

void eeprom_update_mode_count(uint8_t mode_count) {
    eeprom_update_byte((uint8_t *) MODE_COUNT_ADDR, mode_count);
}

//  uint16_t eeprom_read_hsv(uint8_t mode) { return eeprom_read_word((uint16_t *) mode_address[mode]); }
//  void eeprom_update_hsv(uint8_t mode, uint16_t val) { eeprom_update_word((uint16_t *) mode_address[mode], val); }

HSV eeprom_read_hsv(uint8_t mode) {
     return unpack_hsv(eeprom_read_word((uint16_t *) mode_address[mode]));
}

void eeprom_update_hsv(uint8_t mode, uint8_t hue, uint8_t sat, uint8_t val) {
    eeprom_update_word((uint16_t *) mode_address[mode], pack_hsv(hue, sat, val));
}

//build_id is used as a volatile to allow code to run post-flash
//build_id is written during build (by rules.mk), taking the 16 least significant bits of the epoch
//this will clobber eeprom: mousekey_accel, backlight

bool eeprom_is_valid(void) {
    return (eeprom_read_word((uint16_t *)BUILD_ADDR) == BUILD_ID);
}

void eeprom_set_valid(bool valid) {
    if (eeprom_is_valid() != valid) {
        eeprom_update_word((uint16_t *)BUILD_ADDR, valid ? BUILD_ID : 0xFFFF);
    }
}


// void store_mode_hsv(uint8_t mode, uint8_t hue, uint8_t sat, uint8_t val ) {
//     eeprom_update_hsv(mode, pack_hsv(hue, sat, val));
//     uprintf("store %u:\t%u\t%u, %u, %u\n",mode,pack_hsv(hue, sat, val),hue, sat, val);
// }

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

// sets the mode - just a case of changin colour and setting the var
void set_mode(uint8_t mode) {

    current_mode = mode;
    HSV mode_hsv = eeprom_read_hsv(mode);

    rgblight_sethsv_noeeprom(mode_hsv.h, mode_hsv.s, mode_hsv.v);

    uprintf("new mode: %u\n", current_mode);
}

void increment_mode(void) {
    //sets mode by incrementing and calling function, using modulus to restart at 0 if we go beyond the last one
    set_mode((current_mode + 1) % mode_count);
}

void decrement_mode(void) {
    if(current_mode == 0)
        current_mode = mode_count;

    set_mode(current_mode - 1);
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
    set_mode(0);
}


void handle_key(uint16_t keycode, bool pressed) {

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
                increment_mode();
                break;
            case MODE_DOWN:
                decrement_mode();
                break;
            default:
               // send_keypress(keycode);
                break;
        }
    }
}

void keyboard_post_init_user(void) {

    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    } else {
        eeprom_init_hsv(false);
    }


    //rgblight_enable_noeeprom();
    set_mode(0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    handle_key(keycode, record->event.pressed);
    return false;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    handle_key(clockwise ? ROT_CW : ROT_CCW, false);
}

void eeconfig_init_user() {
   eeprom_init_hsv(true);
}
