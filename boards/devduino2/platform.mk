#
# arch: avr
# chip: atmega328p
# platform: devduino v2
# description: http://www.seeedstudio.com/wiki/DevDuino_Sensor_Node_V2.0_(ATmega_328)
#

## toolchain

CROSS_COMPILE = avr

CC		= $(CROSS_COMPILE)-gcc
LD		= $(CROSS_COMPILE)-ld
OBJCOPY = $(CROSS_COMPILE)-objcopy
OBJDUMP	= $(CROSS_COMPILE)-objdump


## platform compile flags

CHIP		= atmega328p
CLK_FREQ    = 16000000L

PFLAGS = -mmcu=$(CHIP) -DF_CPU=$(CLK_FREQ)

## platform flash flags

SERIAL_PORT = /dev/ttyUSB0
PROGRAMMER	= arduino
BAUDRATE	= 57600

DUDE_OPTIONS = \
	-p $(CHIP)	\
	-P $(SERIAL_PORT)	\
	-c $(PROGRAMMER)	\
	-b $(BAUDRATE)		\
	-F

## projects for devduino2

TARGETS = "led uart rf24client rf24node"

ifeq ($(MAKECMDGOALS), led)
include $(PRJ_DIR)/boards/$(PLAT)/apps/led/build.mk
endif

ifeq ($(MAKECMDGOALS), uart)
include $(PRJ_DIR)/boards/$(PLAT)/apps/uart/build.mk
endif

ifeq ($(MAKECMDGOALS), rf24client)
include $(PRJ_DIR)/boards/$(PLAT)/apps/rf24client/build.mk
endif

ifeq ($(MAKECMDGOALS), rf24node)
include $(PRJ_DIR)/boards/$(PLAT)/apps/rf24node/build.mk
endif

## upload rules

upload:
	avrdude $(DUDE_OPTIONS) -U flash:w:out/firmware.hex
