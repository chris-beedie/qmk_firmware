//TODO - long press of mode goes down
//TODO - option to use depressed rotary encoder to flick through layers?
//TODO - update readme
//TODO - update info.json
//TODO have a prep file that parses the user config seprate from the functions?
//TODO - add ifndefs for all users vars
//TODO - long press should give feedback when the timer is hit rather than when released

//FEATURE - CHANGE MODE COLOURS OTF
//FEATURE - CHANGE MODES OTF
//FEATURE - SPECIFY WHICH LEDS INDICATE STATE
//FEATURE - BINARY MODE INDICATION


//mode count check
#if MODES > 8
#error MODES must be <= 8
#endif


#define KEY_COUNT 8

#define MODE_BITS_SIZE 3
const int MODE_BITS_ARR[MODE_BITS_SIZE] = MODE_BITS ;

#define KEY_BITS_SIZE 3
const int KEY_BITS_ARR[KEY_BITS_SIZE] = KEY_BITS ;

#ifndef MODE_LONG_PRESS
#define MODE_LONG_PRESS 0
#endif

#ifdef MODE_ROT_ADJUST_ENABLED
#define MODE_ROT_ADJUST 1
#else
#define MODE_ROT_ADJUST 0
#endif

#ifndef MODE_UP
#define MODE_UP 0xFF
#endif

#ifndef MODE_DOWN
#define MODE_DOWN 0xFF
#endif



// layout isn't intended to be changed, behaviour is controlled by a listener script on the host
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KEY1, KEY2, KEY3, KEY4, KEY5, ROT_BUT)
};

static uint8_t current_mode; //keep track of cuurent layer

uint16_t mode_hold_timer;

enum keystate_t {
    NONE,
    PRESSED,
    IGNORED
} keystate[KEY_COUNT];

// sets the mode - just a case of changin colour and setting the var
void set_mode(uint8_t mode) {

    current_mode = mode;

    uprintf("new mode: %u\n", current_mode);

    switch (current_mode) {
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
        case 6:
            rgblight_sethsv_noeeprom(HSV_MODE_6);
            break;
        case 7:
            rgblight_sethsv_noeeprom(HSV_MODE_7);
            break;
        default:
            rgblight_sethsv_noeeprom(0,0,0);
            break;
    }

}

void increment_mode(void) {
    //sets mode by incrementing and calling function, using modulus to restart at 0 if we go beyond the last one
    set_mode((current_mode + 1) % MODES);
}

void decrement_mode(void) {
    if(current_mode == 0)
        current_mode = MODES;

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


void set_mode_hsv(uint16_t keycode) {

    switch (keycode) {
        case KEY2:
            rgblight_decrease_sat_noeeprom();
            break;
        case KEY3:
            rgblight_increase_sat_noeeprom();
            break;
        case KEY4:
            rgblight_decrease_val_noeeprom();
            break;
        case KEY5:
            rgblight_increase_val_noeeprom();
            break;
        case ROT_CW:
            rgblight_decrease_hue_noeeprom();
            break;
        case ROT_CCW:
            rgblight_decrease_hue_noeeprom();
            break;
        default:
            break;
    }

 // uint8_t hue = rgblight_get_hue();
    // uint8_t sat = rgblight_get_sat();
    // uint8_t val = rgblight_get_val();


}

#define SIZE_OF_BYTE 8

#define HUE_PACK_BITS 8
#define SAT_PACK_BITS 4
#define VAL_PACK_BITS 4

#define HUE_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS)
#define SAT_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS - SAT_PACK_BITS)
#define VAL_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS - SAT_PACK_BITS - VAL_PACK_BITS)

uint8_t downsample(uint8_t value, uint8_t bits) {

    //add on the bit below the mask, means the value will get rounded rather than truncated
    uint8_t _value = value + (1 << (SIZE_OF_BYTE - bits -1));

    //check operation above didn't cause an overflow
    if (_value < value)
        _value = value;

    //shift it until we are left with the desired number of bits
    return _value >> (SIZE_OF_BYTE - bits);
}

uint16_t pack_hsv(uint8_t hue, uint8_t sat, uint8_t val) {

    //downsamples the values to the desired number of bits and packages them into a uint16
    return (
        downsample(hue, HUE_PACK_BITS) << HUE_PACK_OFFSET |
        downsample(sat, SAT_PACK_BITS) << SAT_PACK_OFFSET |
        downsample(val, VAL_PACK_BITS) << VAL_PACK_OFFSET
    );

}

HSV unpack_hsv_test(uint16_t packed_hsv) {

    HSV hsv;

    hsv.h = 220;
    hsv.s = 210;
    hsv.v = 100;

    return hsv;

}

uint8_t unpack(uint16_t packed, uint8_t bits, uint8_t offset ) {

    //shift it back to the end, mask it, then restore to 8bit
    return ((packed >> offset) & ((1 << bits) - 1)) << (SIZE_OF_BYTE - bits);
}

HSV unpack_hsv(uint16_t packed_hsv) {

    return (HSV){
        .h = unpack(packed_hsv, HUE_PACK_BITS, HUE_PACK_OFFSET),
        .s = unpack(packed_hsv, SAT_PACK_BITS, SAT_PACK_OFFSET),
        .v = unpack(packed_hsv, VAL_PACK_BITS, VAL_PACK_OFFSET)
    };

}


void handle_keypress(uint16_t keycode, bool pressed) {

    //pack test
    uint16_t pack1 = pack_hsv(67,95,12);
    uint16_t pack2 = pack_hsv(2,245,45);

    uprintf("pack_hsv1: %u\n", pack1);
    uprintf("pack_hsv2: %u\n", pack2);

    HSV unpack1 = unpack_hsv(pack1);
    HSV unpack2 = unpack_hsv(pack2);

    uprintf("unpack_hsv1: %u, %u, %u\n",unpack1.h, unpack1.s, unpack1.v);
    uprintf("unpack_hsv2: %u, %u, %u\n",unpack2.h, unpack2.s, unpack2.v);


    //if ignoring a key, reset state and resturn
    if(keystate[keycode] == IGNORED) {
        keystate[keycode] = NONE;
        return;
    } else {
        keystate[keycode] = pressed ? PRESSED : NONE;
    }

    //only special cases occur on the press
    if(pressed) {

        //special case - mode hold
        if (keycode == MODE_UP && MODE_LONG_PRESS) {
            mode_hold_timer = timer_read();
        }



    } else {

        //special case - set RGB for current layer
        if (keycode != KEY1 && keystate[KEY1] != NONE) {

            set_mode_hsv(keycode);

            keystate[KEY1] = IGNORED;
            return;
        }


        //special case - mode hold
        if (keycode == MODE_UP && MODE_LONG_PRESS && timer_elapsed(mode_hold_timer) > MODE_LONG_PRESS) {
            keycode = MODE_DOWN;
        }

        //special case - rot mode change (press and turn)
        if ( MODE_ROT_ADJUST && keystate[ROT_BUT] != NONE) {
            if(keycode == ROT_CW)
                keycode = MODE_UP;
            else if(keycode == ROT_CCW)
                keycode = MODE_DOWN;
        }

        //normal key processing
        switch (keycode) {
            case MODE_UP:
                increment_mode();
                break;
            case MODE_DOWN:
                decrement_mode();
                break;
            default:
                send_keypress(keycode);
                break;
        }
    }
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    set_mode(0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    handle_keypress(keycode, record->event.pressed);
    return false;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    handle_keypress(clockwise ? ROT_CW : ROT_CCW, false);
}
