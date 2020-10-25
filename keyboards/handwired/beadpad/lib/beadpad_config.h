

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xBEAD
#define PRODUCT_ID      0xCDBD
#define DEVICE_VER      0x0001
#define MANUFACTURER    bead
#define PRODUCT         beadpad
#define DESCRIPTION     macropad

/* key matrix size */
#define MATRIX_ROWS 1
#define MATRIX_COLS KEY_COUNT + 1 //TODO - remove rot from this


#define MATRIX_ROW_PINS { KEY_ROW_PIN }
#define MATRIX_COL_PINS KEY_COL_PINS
#ifdef ENCODER_ENABLE
#define ENCODERS_PAD_A { ENCODER_PAD_A }
#define ENCODERS_PAD_B { ENCODER_PAD_B }
#define ENCODER_RESOLUTION 4
#endif
#define UNUSED_PINS

#define DIODE_DIRECTION COL2ROW

#define RGB_DI_PIN LED_PIN
#define RGBLED_NUM KEY_COUNT
// #define RGBLIGHT_SLEEP

#define DEBOUNCE 5

#define TAP_CODE_DELAY 0

// disable unused features
#define NO_ACTION_LAYER
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
//#define NO_DEBUG
//#define NO_PRINT
