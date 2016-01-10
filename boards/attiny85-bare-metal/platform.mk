#
# arch: avr
# chip: atmega168
# platform: bare metal attiny85
# description:
#   http://www.atmel.com/ru/ru/devices/ATTINY85.aspx
#   http://www.engbedded.com/fusecalc
#   http://www.jdunman.com/ww/AmateurRadio/SDR/helix_air_net_au%20%20AVRDUDE%20and%20FTDI%20232H.htm
#
# Schematics of attiny85 programmer based on UM232H is shown on docs/avr-isp-using-um232h.png
#

## toolchain

CROSS_COMPILE = avr

CC		= $(CROSS_COMPILE)-gcc
LD		= $(CROSS_COMPILE)-ld
OBJCOPY = $(CROSS_COMPILE)-objcopy
OBJDUMP	= $(CROSS_COMPILE)-objdump

## dependencies

deps: libnrf24 libsoftuart

## platform compile flags

CHIP		= attiny85
CLK_FREQ    = 1000000L

PFLAGS = -mmcu=$(CHIP) -DF_CPU=$(CLK_FREQ) -Os -DNRF24_LOG_LEVEL=1

## platform flash flags

SERIAL_PORT = /dev/ttyUSB0
PROGRAMMER	= UM232H
SPI_BIT_CLK = 100000

DUDE_OPTIONS = \
	-p $(CHIP)	\
	-P $(SERIAL_PORT)	\
	-c $(PROGRAMMER)	\
	-b $(SPI_BIT_CLK)

## projects for arduino-mini

TARGETS = "led-gpio led-pwm softuart-test adc-test gas-sensor rf24node"

ifeq ($(MAKECMDGOALS), led-gpio)
include $(PRJ_DIR)/boards/$(PLAT)/apps/led-gpio/build.mk
endif

ifeq ($(MAKECMDGOALS), led-pwm)
include $(PRJ_DIR)/boards/$(PLAT)/apps/led-pwm/build.mk
endif

ifeq ($(MAKECMDGOALS), softuart-test)
include $(PRJ_DIR)/boards/$(PLAT)/apps/softuart-test/build.mk
endif

ifeq ($(MAKECMDGOALS), adc-test)
include $(PRJ_DIR)/boards/$(PLAT)/apps/adc-test/build.mk
endif

ifeq ($(MAKECMDGOALS), gas-sensor)
include $(PRJ_DIR)/boards/$(PLAT)/apps/gas-sensor/build.mk
endif

ifeq ($(MAKECMDGOALS), rf24node)
include $(PRJ_DIR)/boards/$(PLAT)/apps/rf24node/build.mk
endif

## upload rules

upload:
	avrdude $(DUDE_OPTIONS) -U flash:w:out/firmware.hex
