#
# example: LED blink
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/led
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/leds

## sources

LED_SRCS := \
	main.c \
	leds.c

LED_OBJS := $(LED_SRCS:.c=.o)
LED_OBJS := $(addprefix $(OBJ_DIR)/,$(LED_OBJS))

## deps

LIBS =

## flags

CFLAGS	= -Wall $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/led

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

led: $(OBJ_DIR)/led.hex
	cp $(OBJ_DIR)/led.hex $(OBJ_DIR)/firmware.hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/led.elf: $(LED_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LED_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
