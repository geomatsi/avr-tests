#
# example: NODE
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/rf24node
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/clock
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/spi

## sources

NODE_SRCS := \
	main.c	\
	uart.c	\
	led.c	\
	spi.c	\
	clock.c	\
	radio.c

NODE_OBJS := $(NODE_SRCS:.c=.o)
NODE_OBJS := $(addprefix $(OBJ_DIR)/,$(NODE_OBJS))

## deps

LIBS = $(LIBNRF24)

## flags

CFLAGS	= -Wall -O2 $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/rf24node
CFLAGS	+= $(LIBNRF24_INC)

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

rf24node: $(OBJ_DIR)/rf24node.hex
	cp $(OBJ_DIR)/rf24node.hex $(OBJ_DIR)/firmware.hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/rf24node.elf: $(NODE_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(NODE_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
