#
# example: 1-wire tests
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/w1
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/clock
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/uart
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/leds

VPATH += $(PRJ_DIR)/drivers/w1

## sources

W1_SRCS := \
	main.c \
	clock.c \
	uart.c \
	w1core.c \
	ds18b20.c

W1_OBJS := $(W1_SRCS:.c=.o)
W1_OBJS := $(addprefix $(OBJ_DIR)/,$(W1_OBJS))

## deps

LIBS =

## flags

CFLAGS	= -Wall $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/w1

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

w1: $(OBJ_DIR)/w1.hex
	cp $(OBJ_DIR)/w1.hex $(OBJ_DIR)/firmware.hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/w1.elf: $(W1_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(W1_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
