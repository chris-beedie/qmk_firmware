

# MCU name
MCU = atmega32u4

# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
ENCODER_ENABLE = yes



THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
THIS_DIR := $(dir $(THIS_MAKEFILE))

EPOCH := $(shell date +"%s")
BUILD_ID := $(shell echo "$$(( $(EPOCH) & 0xFFFF ))" )

$(shell echo '//auto-generated file - changes will be overwritten during build!' > $(THIS_DIR)/buildinfo.h)
$(shell echo '#define BUILD_ID $(BUILD_ID)' >> $(THIS_DIR)/buildinfo.h)

 SRC += color.c \
		ws2812.c
