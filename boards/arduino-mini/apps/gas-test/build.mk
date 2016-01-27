#
# example: UART
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/gas-test
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/uart
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/leds
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/adc

## sources

UART_SRCS := \
	main.c \
	uart.c \
	leds.c \
	adc.c

UART_OBJS := $(UART_SRCS:.c=.o)
UART_OBJS := $(addprefix $(OBJ_DIR)/,$(UART_OBJS))

## deps

LIBS =

## flags

CFLAGS	= -Wall $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/gas-test

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

gas-test: $(OBJ_DIR)/gas-test.hex
	cp $(OBJ_DIR)/gas-test.hex $(OBJ_DIR)/firmware.hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/gas-test.elf: $(UART_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(UART_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
