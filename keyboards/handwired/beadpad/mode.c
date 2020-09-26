//TODO - long press of mode goes down
//TODO - option to use depressed rotary encoder to flick through layers?
//TODO - update readme
//TODO - update info.json

//mode count check
#if MODES > 8
#error MODES must be <= 8
#endif

enum KEY_BIT {
    MODE = -1, //mode is a special button
    KEY1 = 0,
    KEY2,
    KEY3,
    KEY4,
    ROT_BUT,
    ROT_CW,
    ROT_CCW
};

// layout isn't intended to be changed (other than maybe changing which one acts as the mode key), behaviour is controlled by a listener script on the host
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( MODE, KEY1, KEY2, KEY3, KEY4, ROT_BUT)
};

static uint8_t current_mode; //keep track of cuurent layer

// sets the mode - just a case of changin colour and setting the var
void set_mode(uint8_t mode) {

    current_mode = mode;

    //uprintf("new mode: %u\n", current_mode);

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

void keypress(uint16_t keycode) {
   // uprintf("keycode: %u\n", keycode);

    uint8_t input;

    input = current_mode;
    for(uint8_t bit = 0; bit < MODE_BITS_SIZE; ++bit, input >>= 1) {
        if (input & 1)
            tap_code(MODE_BITS[bit]);
    }

    input = keycode;
    for(uint8_t bit = 0; bit < KEY_BITS_SIZE; ++bit, input >>= 1) {
        if (input & 1)
            tap_code(KEY_BITS[bit]);
    }

    tap_code(TERMINATOR);
    //uprintf("mode: %u\n", current_mode);
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    set_mode(0);
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        switch (keycode) {
            case MODE:
                //sets mode by incrementing and calling function, using modulus to restart at 0 if we go beyond the last one
                set_mode((current_mode + 1) % MODES);
                break;
            default:
                keypress(keycode);
                break; // shouldn't get here, but if we do let's just carry on
        }
    }
    return false;
}

void encoder_update_user(uint8_t index, bool clockwise) {

    if (clockwise) {
        keypress(ROT_CW);
    } else {
        keypress(ROT_CCW);
    }
}
