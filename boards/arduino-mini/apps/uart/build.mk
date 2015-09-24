#
# example: UART
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/uart
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/uart
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/leds

## sources

UART_SRCS := \
	main.c \
	uart.c \
	leds.c

UART_OBJS := $(UART_SRCS:.c=.o)
UART_OBJS := $(addprefix $(OBJ_DIR)/,$(UART_OBJS))

## deps

LIBS =

## flags

CFLAGS	= -Wall -O2 $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/uart

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

uart: $(OBJ_DIR)/uart.hex
	cp $(OBJ_DIR)/uart.hex $(OBJ_DIR)/firmware.hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/uart.elf: $(UART_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(UART_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
