
#include "beadpad_settings.h"
#include "beadpad_eeprom.h"


bool settings_try_update(uint16_t keycode) {
    //loop though each setting, if enabled and the key is down, action the new key

    for (uint8_t i = 0; i < SETTING_COUNT; i++) {
        struct setting setting = settings[i];

        if (setting.enabled && keycode != setting.keycode && keystate[setting.keycode] != NONE) {
            setting.update(keycode);
            keystate[setting.keycode] = SETTING;
            return true;
        }
    }
    return false;
}

bool settings_try_complete(uint16_t keycode) {

    //loop though each setting, if enabled and the key was active, complete the setting
    for (uint8_t i = 0; i < SETTING_COUNT; i++) {
        struct setting setting = settings[i];

        if (setting.enabled && keycode == setting.keycode && keystate[setting.keycode] == CONFIG) {
            setting.complete();
            keystate[setting.keycode] = NONE;
            return true;
        }
    }
    return false;
}

void settings_update_hsv(uint16_t keycode) {

    switch (keycode) {
        case SETTING_HSV_KEY_SAD:
            hsv_decrease_sat();
            break;
        case SETTING_HSV_KEY_SAI:
            hsv_increase_sat();
            break;
        case SETTING_HSV_KEY_VAD:
            hsv_decrease_val();
            break;
        case SETTING_HSV_KEY_VAI:
            hsv_increase_val();
            break;
        case SETTING_HSV_KEY_HUD:
            hsv_decrease_hue();
            break;
        case SETTING_HSV_KEY_HUI:
            hsv_increase_hue();
            break;
        default:
            break;
    }
}

void settings_complete_hsv(void) {
    HSV hsv = hsv_get_current();
    eeprom_update_hsv(mode_current, hsv.h, hsv.s, hsv.v);
}

void settings_update_mode_indication(uint16_t keycode) {

    switch (keycode) {
        case SETTING_MODE_INDICATION_KEY_UP:
            mode_indication = increment(mode_indication, MI_LAST);
            break;
        case SETTING_MODE_INDICATION_KEY_DOWN:
            mode_indication = decrement(mode_indication, 0);
            break;
        default:
            break;
    }

    mode_set(mode_current);
}

void settings_complete_mode_indication(void) {
    eeprom_update_mode_indication(mode_indication);
}

void settings_update_mode_count(uint16_t keycode) {

    switch (keycode) {
        case SETTING_MODE_COUNT_KEY_UP:
            mode_count = increment(mode_count, MAX_MODES);
            break;
        case SETTING_MODE_COUNT_KEY_DOWN:
            mode_count = decrement(mode_count, 1);
            break;
        default:
            break;
    }

    hsv_enable_binary(mode_count);
    hsv_set(0,0,hsv_get_current().v);
}

void settings_complete_mode_count(void) {
    eeprom_update_mode_count(mode_count);
    mode_set(0);
}

void settings_init(void) {

    settings[SETTING_HSV_IDX].keycode = SETTING_HSV_KEY;
    settings[SETTING_HSV_IDX].update = settings_update_hsv;
    settings[SETTING_HSV_IDX].complete = settings_complete_hsv;
    #ifdef SETTING_HSV_ENABLE
    settings[SETTING_HSV_IDX].enabled = true;
    #endif

    settings[SETTING_MODE_INDICATION_IDX].keycode = SETTING_MODE_INDICATION_KEY;
    settings[SETTING_MODE_INDICATION_IDX].update = settings_update_mode_indication;
    settings[SETTING_MODE_INDICATION_IDX].complete = settings_complete_mode_indication;
    #ifdef SETTING_MODE_INDICATION_ENABLE
    settings[SETTING_MODE_INDICATION_IDX].enabled = true;
    #endif

    settings[SETTING_MODE_COUNT_IDX].keycode = SETTING_MODE_COUNT_KEY;
    settings[SETTING_MODE_COUNT_IDX].update = settings_update_mode_count;
    settings[SETTING_MODE_COUNT_IDX].complete = settings_complete_mode_count;
    #ifdef SETTING_MODE_COUNT_ENABLE
    settings[SETTING_MODE_COUNT_IDX].enabled = true;
    #endif
}
