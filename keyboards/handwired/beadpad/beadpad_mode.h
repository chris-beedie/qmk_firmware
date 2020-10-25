
#pragma once

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

// extern uint8_t mode_current;

void mode_init(void);

HSV mode_hsv_read(void);
void mode_hsv_write(void);
uint8_t mode_indication_read(void);
void mode_indication_write(void);
uint8_t mode_count_read(void);
void mode_count_write(void);


void mode_set(uint8_t mode);
void mode_refresh(void);
void mode_increment(void);
void mode_decrement(void);

void mode_set_indicator(uint8_t mode);
void mode_refresh_indicator(void);
void mode_increment_indicator(void);
void mode_decrement_indicator(void);


void mode_indication_increment(void);
void mode_indication_decrement(void);

void mode_count_increment(void);
void mode_count_decrement(void);

void mode_count_display(void);
