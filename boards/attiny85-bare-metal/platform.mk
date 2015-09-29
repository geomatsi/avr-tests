#
# arch: avr
# chip: atmega168
# platform: bare metal attiny85
# description:
#   http://www.atmel.com/ru/ru/devices/ATTINY85.aspx
#   http://www.engbedded.com/fusecalc
#   http://www.jdunman.com/ww/AmateurRadio/SDR/helix_air_net_au%20%20AVRDUDE%20and%20FTDI%20232H.htm

## toolchain

CROSS_COMPILE = avr

CC		= $(CROSS_COMPILE)-gcc
LD		= $(CROSS_COMPILE)-ld
OBJCOPY = $(CROSS_COMPILE)-objcopy
OBJDUMP	= $(CROSS_COMPILE)-objdump


## platform compile flags

CHIP		= attiny85
CLK_FREQ    = 1000000L

PFLAGS = -mmcu=$(CHIP) -DF_CPU=$(CLK_FREQ)

## platform flash flags

SERIAL_PORT = /dev/ttyUSB0
PROGRAMMER	= UM232H
SPI_BIT_CLK = 1000000

DUDE_OPTIONS = \
	-p $(CHIP)	\
	-P $(SERIAL_PORT)	\
	-c $(PROGRAMMER)	\
	-b $(SPI_BIT_CLK)

## projects for arduino-mini

TARGETS = "led"

ifeq ($(MAKECMDGOALS), led)
include $(PRJ_DIR)/boards/$(PLAT)/apps/led/build.mk
endif

## upload rules

upload:
	avrdude $(DUDE_OPTIONS) -U flash:w:out/firmware.hex
