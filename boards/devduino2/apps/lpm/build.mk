#
# example: LED blink
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/lpm
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/leds
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/adc

## sources

LED_SRCS :=	\
	main.c	\
	adc.c	\
	leds.c

LED_OBJS := $(LED_SRCS:.c=.o)
LED_OBJS := $(addprefix $(OBJ_DIR)/,$(LED_OBJS))

## deps

LIBS =

## flags

CFLAGS	= -Wall -O2 $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/lpm

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

lpm: $(OBJ_DIR)/lpm.hex
	cp $(OBJ_DIR)/lpm.hex $(OBJ_DIR)/firmware.hex
	avr-size --mcu=$(CHIP) --format=avr $(OBJ_DIR)/lpm.elf

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/lpm.elf: $(LED_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LED_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
