
// #define SETTING_HSV_IDX 0
// #define SETTING_HSV_KEY KEY1
// #define SETTING_HSV_KEY_SAD KEY2
// #define SETTING_HSV_KEY_SAI KEY3
// #define SETTING_HSV_KEY_VAD KEY4
// #define SETTING_HSV_KEY_VAI KEY5
// #define SETTING_HSV_KEY_HUD ROT_CCW
// #define SETTING_HSV_KEY_HUI ROT_CW

// #define SETTING_MODE_INDICATION_IDX 1
// #define SETTING_MODE_INDICATION_KEY KEY2
// #define SETTING_MODE_INDICATION_KEY_UP ROT_CW
// #define SETTING_MODE_INDICATION_KEY_DOWN ROT_CCW

// #define SETTING_MODE_COUNT_IDX 2
// #define SETTING_MODE_COUNT_KEY KEY3
// #define SETTING_MODE_COUNT_KEY_UP ROT_CW
// #define SETTING_MODE_COUNT_KEY_DOWN ROT_CCW

// #define SETTING_COUNT 3

// struct setting {
//     bool enabled;
//     uint16_t keycode;
//     void (*update)(uint16_t);
//     void (*complete)(void);
// };

// struct setting settings[SETTING_COUNT];


void settings_init(void);

bool settings_try_update(uint16_t keycode);
bool settings_try_complete(uint16_t keycode);
