#
# example: 1-wire test
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/w1-test
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/swuart

VPATH += $(PRJ_DIR)/drivers/w1

## sources

W1_TEST_SRCS := \
	main.c \
	swuart.c \
	w1core.c \
	ds18b20.c \

W1_TEST_OBJS := $(W1_TEST_SRCS:.c=.o)
W1_TEST_OBJS := $(addprefix $(OBJ_DIR)/,$(W1_TEST_OBJS))

## deps

LIBS = $(LIBSOFTUART)

## flags

CFLAGS	= -Wall $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/w1-test
CFLAGS	+= $(LIBSOFTUART_INC)

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

w1-test: $(OBJ_DIR)/w1-test.hex
	cp $(OBJ_DIR)/w1-test.hex $(OBJ_DIR)/firmware.hex
	avr-size --mcu=$(CHIP) --format=avr $(OBJ_DIR)/w1-test.elf

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/w1-test.elf: $(W1_TEST_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(W1_TEST_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
