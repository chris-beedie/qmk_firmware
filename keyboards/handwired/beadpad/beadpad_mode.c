
//#include "beadpad_mode.h"
//#include "beadpad_eeprom.h"
//#include "beadpad_led.h"

static uint8_t mode_count = MAX_MODES;
static uint8_t mode_current = 0;
static uint16_t mode_hold_timer = 0;

enum mode_indication_t {
    MI_NONE,
    MI_MODE_UP,
    MI_MODE_DOWN,
    MI_MODE_UP_DOWN,
    MI_ALL,
    MI_BINARY,
} mode_indication;

#define MI_LAST MI_BINARY

uint8_t mode_get_current(void) {
    return mode_current;
}

// eeprom read/write  =========================================================================================================


HSV mode_hsv_read_for(uint8_t mode) { return hsv_unpack(eeprom_read_mode_hsv(mode)); }
void mode_hsv_write_for(uint8_t mode) {
    HSV hsv = hsv_get_current();
    eeprom_update_mode_hsv(mode, hsv_pack(hsv.h, hsv.s, hsv.v));
}

HSV mode_hsv_read(void) { return mode_hsv_read_for(mode_current); }
void mode_hsv_write(void) { mode_hsv_write_for(mode_current); }

uint8_t mode_indication_read(void) { return eeprom_read_mode_indication(); }
void mode_indication_write(void) { eeprom_update_mode_indication(mode_indication); }

uint8_t mode_count_read(void) { return eeprom_read_mode_count(); }
void mode_count_write(void) { eeprom_update_mode_count(mode_count); }


//manipulate indicator only, not changing the set mode. Used when mode up/down hold is activated, but may need to be undone if a setting function overrides
void mode_set_indicator(uint8_t mode) {

    HSV mode_hsv = mode_hsv_read_for(mode);


    //uprintf("h: %u,s: %u, v: %u\n", mode_hsv.h, mode_hsv.s, mode_hsv.v);

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

void mode_refresh_indicator(void) {
    mode_set_indicator(mode_current);
}

void mode_increment_indicator(void) {
    mode_set_indicator(increment_wrap(mode_current, mode_count));
}

void mode_decrement_indicator(void) {
    mode_set_indicator(decrement_wrap(mode_current, mode_count));
}



void mode_set(uint8_t mode) {
    mode_current = mode;
    mode_set_indicator(mode);
}

void mode_refresh(void) {
    mode_set(mode_current);
}

void mode_increment(void) {
    mode_set(increment_wrap(mode_current, mode_count));
}

void mode_decrement(void) {
    mode_set(decrement_wrap(mode_current, mode_count));
}



// change the mode indication setting
void mode_indication_increment(void) {
    mode_indication = increment(mode_indication, MI_LAST);
}

void mode_indication_decrement(void) {
    mode_indication = decrement(mode_indication, 0);
}

// change mode count setting
void mode_count_increment(void) {
    mode_count = increment(mode_count, MAX_MODES);
}

void mode_count_decrement(void) {
    mode_count = decrement(mode_count, 1);
}

// display the current mode_count in binary
// set to white, but same val as current for consistency
void mode_count_display(void) {
    hsv_enable_binary(mode_count);
    hsv_set(0,0,hsv_get_current().v);
}

//helper to check if mode hold timer has expired
bool mode_change_held(void) {
    return timer_elapsed(mode_hold_timer) > MODE_HOLD_TERM;
}

void mode_hold_try_start(uint16_t keycode) {

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
}

uint16_t mode_hold_try_action(uint16_t keycode) {

    #ifdef KEY_MODE_UP_HOLD
    if (keycode == KEY_MODE_UP_HOLD  && mode_change_held()) {
        return KEY_MODE_UP;
    }
    #endif

    #ifdef KEY_MODE_DOWN_HOLD
    if (keycode == KEY_MODE_DOWN_HOLD && mode_change_held()) {
        return KEY_MODE_DOWN;
    }
    #endif

    return keycode;
}

void mode_hold_check(void) {

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

uint16_t mode_rot_adjust_try_action(uint16_t keycode) {
    #ifdef MODE_ROT_ADJUST_ENABLED
    if (keystate[ROT_BUT] != NONE) {
        return keycode == ROT_CW ? KEY_MODE_UP : KEY_MODE_DOWN;
    }
    #endif

    return keycode;
}

void mode_init(void) {

    mode_count = mode_count_read();
    mode_indication = mode_indication_read();
    mode_set(0);

}
