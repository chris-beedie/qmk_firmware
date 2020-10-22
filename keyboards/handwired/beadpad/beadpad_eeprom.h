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

#include "eeprom.h"
#include "color.h"

#define SIZE_OF_BYTE 8

#define HUE_PACK_BITS 6
#define SAT_PACK_BITS 6
#define VAL_PACK_BITS 4

#define HUE_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS)
#define SAT_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS - SAT_PACK_BITS)
#define VAL_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS - SAT_PACK_BITS - VAL_PACK_BITS)

#if HUE_PACK_BITS + SAT_PACK_BITS + VAL_PACK_BITS > 16
#error HSV too big to pack
#endif

__attribute__((weak)) void beadpad_eeprom_init(bool force);

//build_id is used as a volatile to allow code to run post-flash
//build_id is written during build (by rules.mk), taking the 16 least significant bits of the epoch
bool eeprom_is_valid(void);
void eeprom_set_valid(bool valid);

uint16_t get_hsv_addr(uint8_t mode);

HSV eeprom_read_hsv(uint8_t mode);
void eeprom_update_hsv(uint8_t mode, uint8_t hue, uint8_t sat, uint8_t val);

uint8_t eeprom_read_mode_indication(void);
void eeprom_update_mode_indication(uint8_t mode_indication);

uint8_t eeprom_read_mode_count(void);
void eeprom_update_mode_count(uint8_t mode_count);

