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

## dependencies

NRF24_CFG_FLAGS = -DNRF24_LOG_LEVEL=2

deps: libnrf24 nanopb

## platform compile flags

CHIP		= atmega168
CLK_FREQ    = 16000000L

PFLAGS = -mmcu=$(CHIP) -DF_CPU=$(CLK_FREQ) \
	-Os -fshort-enums -ffunction-sections -Wl,--gc-sections

## platform flash flags

SERIAL_PORT = /dev/ttyUSB0
PROGRAMMER	= arduino
BAUDRATE	= 19200

DUDE_OPTIONS = \
	-p $(CHIP)	\
	-P $(SERIAL_PORT)	\
	-c $(PROGRAMMER)	\
	-b $(BAUDRATE)		\
	-F

## projects for arduino-mini

TARGETS = "led uart gas-test w1"

ifeq ($(MAKECMDGOALS), led)
include $(PRJ_DIR)/boards/$(PLAT)/apps/led/build.mk
endif

ifeq ($(MAKECMDGOALS), uart)
include $(PRJ_DIR)/boards/$(PLAT)/apps/uart/build.mk
endif

ifeq ($(MAKECMDGOALS), gas-test)
include $(PRJ_DIR)/boards/$(PLAT)/apps/gas-test/build.mk
endif

ifeq ($(MAKECMDGOALS), w1)
include $(PRJ_DIR)/boards/$(PLAT)/apps/w1/build.mk
endif

## upload rules

upload:
	avrdude $(DUDE_OPTIONS) -U flash:w:out/firmware.hex
