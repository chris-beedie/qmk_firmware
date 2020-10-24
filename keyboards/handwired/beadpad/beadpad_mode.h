
#include "beadpad_led.c"

enum mode_indication_t {
    MI_NONE,
    MI_MODE_UP,
    MI_MODE_DOWN,
    MI_MODE_UP_DOWN,
    MI_ALL,
    MI_BINARY,

} mode_indication;

#define MI_LAST MI_BINARY

extern uint8_t mode_current;

void mode_init(void);


void mode_set(uint8_t mode);
void mode_increment(void);
void mode_decrement(void);
void mode_refresh(void);

void mode_set_indicator(uint8_t mode);
void mode_increment_indicator(void);
void mode_decrement_indicator(void);


