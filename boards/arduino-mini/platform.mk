#
# arch: avr
# chip: atmega168
# platform: arduino mini v4
# description: http://arduino.cc/en/Main/arduinoBoardMini
#

## toolchain

CROSS_COMPILE = avr

CC		= $(CROSS_COMPILE)-gcc
LD		= $(CROSS_COMPILE)-ld
OBJCOPY = $(CROSS_COMPILE)-objcopy
OBJDUMP	= $(CROSS_COMPILE)-objdump


## platform compile flags

MCU_TARGET	= atmega168
CLK_FREQ    = 16000000L

PFLAGS = -mmcu=$(MCU_TARGET) -DF_CPU=$(CLK_FREQ)

## platform flash flags

SERIAL_PORT = /dev/ttyUSB0
PROGRAMMER	= arduino
BAUDRATE	= 19200

DUDE_OPTIONS = \
	-p $(MCU_TARGET)	\
	-P $(SERIAL_PORT)	\
	-c $(PROGRAMMER)	\
	-b $(BAUDRATE)		\
	-F

## projects for arduino-mini

TARGETS = "led uart"

ifeq ($(MAKECMDGOALS), led)
include $(PRJ_DIR)/boards/$(PLAT)/apps/led/build.mk
endif

ifeq ($(MAKECMDGOALS), uart)
include $(PRJ_DIR)/boards/$(PLAT)/apps/uart/build.mk
endif

## upload rules

upload:
	avrdude $(DUDE_OPTIONS) -U flash:w:firmware.hex
