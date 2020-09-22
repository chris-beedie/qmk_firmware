//TODO - long press of mode goes down
//TODO - option to use depressed rotary encoder to flick through layers?
//TODO - eeprom to store/load last mode
//TODO - update readme
//TODO - update info.json

//mode count check
#if MODES > 10
#error MODES must be <= 10
#endif

#define MODE SAFE_RANGE

// layout isn't intended to be changed, behaviour is controlled by a listener script on the host
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( MODE, KEY1, KEY2, KEY3, KEY4, ROT_BUT)
};

static uint8_t current_mode; //keep track of cuurent layer
static uint16_t current_modifiers; //keep track of mods for the currennt layer


// sets the mode - just a case of changin colour and setting the mods
void set_mode(uint8_t mode) {

    current_mode = mode;

    uprintf("new mode: %u\n", current_mode);

    switch (current_mode) {
        case 0:
            rgblight_sethsv_noeeprom(HSV_MODE_0);
            current_modifiers = MODS_MODE_0;
            break;
        case 1:
            rgblight_sethsv_noeeprom(HSV_MODE_1);
            current_modifiers = MODS_MODE_1;
            break;
        case 2:
            rgblight_sethsv_noeeprom(HSV_MODE_2);
            current_modifiers = MODS_MODE_2;
            break;
        case 3:
            rgblight_sethsv_noeeprom(HSV_MODE_3);
            current_modifiers = MODS_MODE_3;
            break;
        case 4:
            rgblight_sethsv_noeeprom(HSV_MODE_4);
            current_modifiers = MODS_MODE_4;
            break;
        case 5:
            rgblight_sethsv_noeeprom(HSV_MODE_5);
            current_modifiers = MODS_MODE_5;
            break;
        case 6:
            rgblight_sethsv_noeeprom(HSV_MODE_6);
            current_modifiers = MODS_MODE_6;
            break;
        case 7:
            rgblight_sethsv_noeeprom(HSV_MODE_7);
            current_modifiers = MODS_MODE_7;
            break;
        case 8:
            rgblight_sethsv_noeeprom(HSV_MODE_8);
            current_modifiers = MODS_MODE_8;
            break;
        case 9:
            rgblight_sethsv_noeeprom(HSV_MODE_9);
            current_modifiers = MODS_MODE_9;
            break;
        default:
            rgblight_sethsv_noeeprom(0,0,0);
            current_modifiers = KC_NO;
            break;
    }

}

void keypress(uint16_t keycode) {
    uprintf("modifiers: %u\tkeycode: %u\n", current_modifiers, keycode);
    register_code16(current_modifiers);
    tap_code(keycode);
    unregister_code16(current_modifiers);
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

// do we need to set: #define TAP_CODE_DELAY 100 in your config.h

void encoder_update_user(uint8_t index, bool clockwise) {
    //TODO - this needs to respond to whichever layer is active
    if (clockwise) {
        keypress(ROT_CW);
    } else {
        keypress(ROT_CCW);
    }
}
