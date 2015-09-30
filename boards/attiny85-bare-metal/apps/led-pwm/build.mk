#
# example: LED blink
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/led-pwm

## sources

LED_SRCS := \
	main.c

LED_OBJS := $(LED_SRCS:.c=.o)
LED_OBJS := $(addprefix $(OBJ_DIR)/,$(LED_OBJS))

## deps

LIBS =

## flags

CFLAGS	= -Wall -O2 $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/led-pwm

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

led-pwm: $(OBJ_DIR)/led-pwm.hex
	cp $(OBJ_DIR)/led-pwm.hex $(OBJ_DIR)/firmware.hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/led-pwm.elf: $(LED_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LED_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
