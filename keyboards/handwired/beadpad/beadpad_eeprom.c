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
#include "beadpad_eeprom.h"
#include "buildinfo.h"

#define BUILD_ADDR              EECONFIG_SIZE
#define MODE_COUNT_ADDR         BUILD_ADDR + 2
#define MODE_INDICATION_ADDR    MODE_COUNT_ADDR + 1
#define HSV_BASE_ADDR           MODE_INDICATION_ADDR + 1

//build_id is used as a volatile to allow code to run post-flash
//build_id is written during build (by rules.mk), taking the 16 least significant bits of the epoch
bool eeprom_is_valid(void) {
    return (eeprom_read_word((uint16_t *)BUILD_ADDR) == BUILD_ID);
}

void eeprom_set_valid(bool valid) {
    if (eeprom_is_valid() != valid) {
        eeprom_update_word((uint16_t *)BUILD_ADDR, valid ? BUILD_ID : 0xFFFF);
    }
}

uint16_t get_hsv_addr(uint8_t mode) {
    return HSV_BASE_ADDR + (2*mode);
}

uint16_t pack(uint8_t value, uint8_t bits, uint8_t offset) {
    //compresses (lossy) a byte to the desired number of bits

    //add on the bit below the mask, means the value will get rounded rather than truncated
    uint8_t _value = value + (1 << (SIZE_OF_BYTE - bits -1));

    //check operation above didn't cause an overflow
    if (_value < value)
        _value = UINT8_MAX;

    //shift it until we are left with the desired number of bits and then reposition for packing
    return (_value >> (SIZE_OF_BYTE - bits)) << offset;
}

uint8_t unpack(uint16_t packed, uint8_t bits, uint8_t offset ) {
    //restores a packed number of bits to a byte representation

    //shift it back to the end, mask it, then restore to 8bit
    return ((packed >> offset) & ((1 << bits) - 1)) << (SIZE_OF_BYTE - bits);
}

uint16_t pack_hsv(uint8_t hue, uint8_t sat, uint8_t val) {

    //compresses/downsamples the hsv values (3x byte) to a word (1x16 bits) using the desired number of bits per value

    return (
        pack(hue, HUE_PACK_BITS, HUE_PACK_OFFSET) |
        pack(sat, SAT_PACK_BITS, SAT_PACK_OFFSET) |
        pack(val, VAL_PACK_BITS, VAL_PACK_OFFSET)
    );

}

HSV unpack_hsv(uint16_t packed_hsv) {

    //restores the word (1x16 bit) to 3x bytes as the hsv struct

    return (HSV){
        .h = unpack(packed_hsv, HUE_PACK_BITS, HUE_PACK_OFFSET),
        .s = unpack(packed_hsv, SAT_PACK_BITS, SAT_PACK_OFFSET),
        .v = unpack(packed_hsv, VAL_PACK_BITS, VAL_PACK_OFFSET)
    };

}

HSV eeprom_read_hsv(uint8_t mode) {
     return unpack_hsv(eeprom_read_word((uint16_t *) get_hsv_addr(mode)));
}

void eeprom_update_hsv(uint8_t mode, uint8_t hue, uint8_t sat, uint8_t val) {
    eeprom_update_word((uint16_t *) get_hsv_addr(mode), pack_hsv(hue, sat, val));
}

uint8_t eeprom_read_mode_indication(void) {
    return eeprom_read_byte((uint8_t *) MODE_INDICATION_ADDR);
}

void eeprom_update_mode_indication(uint8_t mode_indication) {
    eeprom_update_byte((uint8_t *) MODE_INDICATION_ADDR, mode_indication);
}

uint8_t eeprom_read_mode_count(void) {
    return eeprom_read_byte((uint8_t *) MODE_COUNT_ADDR);
}

void eeprom_update_mode_count(uint8_t mode_count) {
    eeprom_update_byte((uint8_t *) MODE_COUNT_ADDR, mode_count);
}

#define DEBUG_ADDR 19

uint8_t eeprom_read_debug(void) {
    return eeprom_read_byte((uint8_t *) DEBUG_ADDR);
}

void eeprom_update_debug(uint8_t val) {
    eeprom_update_byte((uint8_t *) DEBUG_ADDR, val);
}
