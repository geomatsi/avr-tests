#
# arch: avr
# chip: attiny85
# platform: digispark
# description:
#   http://digistump.com/wiki/digispark
#   http://littlewire.cc
#   https://github.com/micronucleus/micronucleus

## toolchain

CROSS_COMPILE = avr

CC		= $(CROSS_COMPILE)-gcc
LD		= $(CROSS_COMPILE)-ld
OBJCOPY = $(CROSS_COMPILE)-objcopy
OBJDUMP	= $(CROSS_COMPILE)-objdump


## platform compile flags

CHIP		= attiny85
CLK_FREQ    = 16500000L

PFLAGS = -mmcu=$(CHIP) -DF_CPU=$(CLK_FREQ)

## platform flash flags

# FIXME: not avrdude at the moment since Digispark board has micronucleus bootloader on board
# TODO: check if we can use avrdude, there are should be some patches for avrdude floating around
#

## projects for digispark

TARGETS = "led"

ifeq ($(MAKECMDGOALS), led)
include $(PRJ_DIR)/boards/$(PLAT)/apps/led/build.mk
endif

## upload rules

upload:
	micronucleus --type intel-hex --run out/firmware.hex
