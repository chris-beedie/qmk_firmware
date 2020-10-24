/* Copyright 2020 Chris Beedie
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "beadpad.h"

#include "beadpad_mode.h"


__attribute__((weak)) void beadpad_init(void) {  }
__attribute__((weak)) void handle_key(uint16_t keycode, bool pressed) {  }
//qmk overrides ==================================================================================================================



void eeconfig_init_kb() {
   beadpad_eeprom_init(true);
}

void keyboard_post_init_kb(void) {

    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    } else {
        beadpad_eeprom_init(false);
    }

    beadpad_init();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    handle_key(keycode, record->event.pressed);
    return false;
}

void encoder_update_kb(uint8_t index, bool clockwise) {
   handle_key(clockwise ? ROT_CW : ROT_CCW, false);
}

void suspend_power_down_kb(void) {
    hsv_enable(false);
}

void suspend_wakeup_init_kb(void) {
    mode_refresh();
}





