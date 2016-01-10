#
# example: gas sensor
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/gas-sensor
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/swuart
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/adc

## sources

GAS_SENSOR_SRCS := \
	main.c \
	swuart.c \
	adc.c

GAS_SENSOR_OBJS := $(GAS_SENSOR_SRCS:.c=.o)
GAS_SENSOR_OBJS := $(addprefix $(OBJ_DIR)/,$(GAS_SENSOR_OBJS))

## deps

LIBS = $(LIBSOFTUART)

## flags

CFLAGS	= -Wall -O2 $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/gas-sensor
CFLAGS	+= $(LIBSOFTUART_INC)

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

gas-sensor: $(OBJ_DIR)/gas-sensor.hex
	cp $(OBJ_DIR)/gas-sensor.hex $(OBJ_DIR)/firmware.hex
	avr-size --mcu=$(CHIP) --format=avr $(OBJ_DIR)/gas-sensor.elf

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/gas-sensor.elf: $(GAS_SENSOR_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(GAS_SENSOR_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
