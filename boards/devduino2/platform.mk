#
# arch: avr
# chip: atmega328p
# platform: devduino v2
# description: http://www.seeedstudio.com/wiki/DevDuino_Sensor_Node_V2.0_(ATmega_328)
#

## toolchain

CROSS_COMPILE = avr

CC	= $(CROSS_COMPILE)-gcc
LD	= $(CROSS_COMPILE)-ld
OBJCOPY	= $(CROSS_COMPILE)-objcopy
OBJDUMP	= $(CROSS_COMPILE)-objdump

## dependencies

NRF24_CFG_FLAGS = -DNRF24_LOG_LEVEL=2 -DCONFIG_LIB_RF24_SIZEOPT -DSPI_SINGLE_BYTE

deps: libnrf24 nanopb

## platform compile flags

CHIP		= atmega328p
CLK_FREQ	= 16000000L

PFLAGS = -mmcu=$(CHIP) -DF_CPU=$(CLK_FREQ) \
	-Os -fshort-enums -ffunction-sections -Wl,--gc-sections

## platform programming flags

# programming using arduino bootloader

SERIAL_PORT	= /dev/ttyUSB0
PROGRAMMER	= arduino
BAUDRATE	= 57600

DUDE_OPTIONS_SERIAL =		\
	-p $(CHIP)		\
	-P $(SERIAL_PORT)	\
	-c $(PROGRAMMER)	\
	-b $(BAUDRATE)		\
	-F

# programming using ft232rl based ISP programmer

FTDI_PORT = ft0
PROGRAMMER  = arduino-ft232r
FTDI_BBSYNC_RATE = 2400

DUDE_OPTIONS_FT232RL =		\
	-p $(CHIP)		\
	-P $(FTDI_PORT)		\
	-c $(PROGRAMMER)	\
	-B $(FTDI_BBSYNC_RATE)

# programming using ft232rl based ISP programmer

PROGRAMMER  = usbasp

DUDE_OPTIONS_USBASP =		\
	-p $(CHIP)		\
	-c $(PROGRAMMER)

## projects for devduino2

TARGETS = "led uart rf24client rf24node lpm"

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

ifeq ($(MAKECMDGOALS), lpm)
include $(PRJ_DIR)/boards/$(PLAT)/apps/lpm/build.mk
endif

## upload rules

upload-serial:
	avrdude $(DUDE_OPTIONS_SERIAL) -U flash:w:out/firmware.hex

upload-ft232rl:
	avrdude $(DUDE_OPTIONS_FT232RL) -U flash:w:out/firmware.hex

upload-usbasp:
	avrdude $(DUDE_OPTIONS_USBASP) -U flash:w:out/firmware.hex

upload: upload-usbasp
