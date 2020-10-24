
#include "beadpad_mode.h"

uint8_t mode_count = MAX_MODES;
uint8_t mode_current = 0; //keep track of current mode



void mode_init(void) {

    mode_count = eeprom_read_mode_count();
    mode_indication = eeprom_read_mode_indication();

}




//set the mode indicator only, used when mode up/down hold is activated, but may need to be undone if an edit function overrides
void mode_set_indicator(uint8_t mode) {

    HSV mode_hsv = eeprom_read_hsv(mode);

    uprintf("h: %u,s: %u, v: %u\n", mode_hsv.h, mode_hsv.s, mode_hsv.v);

    hsv_enable(false);

    switch (mode_indication) {
        case MI_MODE_UP:
            hsv_enable_at(KEY_MODE_UP,true);
            break;
        case MI_MODE_DOWN:
            hsv_enable_at(KEY_MODE_DOWN, true);
            break;
        case MI_MODE_UP_DOWN:
            hsv_enable_at(KEY_MODE_UP,true);
            hsv_enable_at(KEY_MODE_DOWN, true);
            break;
        case MI_ALL:
            hsv_enable(true);
            break;
        case MI_BINARY:
            hsv_enable_binary(mode);
            break;
        default:
            break;
    }

    hsv_set(mode_hsv.h, mode_hsv.s, mode_hsv.v);
}

// sets the mode - just a case of changin colour and setting the var
void mode_set(uint8_t mode) {
    mode_current = mode;
    mode_set_indicator(mode);
}

void mode_refresh(void) {
    mode_set(mode_current);
}

void mode_increment_indicator() {
    mode_set_indicator(increment_wrap(mode_current, mode_count));
}

void mode_decrement_indicator() {
    mode_set_indicator(decrement_wrap(mode_current, mode_count));
}

void mode_increment() {
    mode_set(increment_wrap(mode_current, mode_count));
}

void mode_decrement() {
    mode_set(decrement_wrap(mode_current, mode_count));
}
