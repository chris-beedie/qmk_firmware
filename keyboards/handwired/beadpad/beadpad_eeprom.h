

#define SIZE_OF_BYTE 8

#define HUE_PACK_BITS 7
#define SAT_PACK_BITS 6
#define VAL_PACK_BITS 3

#define HUE_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS)
#define SAT_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS - SAT_PACK_BITS)
#define VAL_PACK_OFFSET (2*SIZE_OF_BYTE - HUE_PACK_BITS - SAT_PACK_BITS - VAL_PACK_BITS)

#if HUE_PACK_BITS + SAT_PACK_BITS + VAL_PACK_BITS > 16
#error HSV too big to pack
#endif


//build_id is used as a volatile to allow code to run post-flash
//build_id is written during build (by rules.mk), taking the 16 least significant bits of the epoch
bool eeprom_is_valid(void);
void eeprom_set_valid(bool valid);
uint8_t eeprom_read_mode_count(void);
void eeprom_update_mode_count(uint8_t mode_count);
uint16_t get_hsv_addr(uint8_t mode);

HSV eeprom_read_hsv(uint8_t mode);
void eeprom_update_hsv(uint8_t mode, uint8_t hue, uint8_t sat, uint8_t val);

__attribute__((weak)) void eeprom_init_hsv(bool force);
