#
# example: SOFTUART test
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/softuart-test
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/swuart

## sources

SOFTUART_TEST_SRCS := \
	main.c \
	swuart.c

SOFTUART_TEST_OBJS := $(SOFTUART_TEST_SRCS:.c=.o)
SOFTUART_TEST_OBJS := $(addprefix $(OBJ_DIR)/,$(SOFTUART_TEST_OBJS))

## deps

LIBS = $(LIBSOFTUART)

## flags

CFLAGS	= -Wall $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/softuart-test
CFLAGS	+= $(LIBSOFTUART_INC)

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

softuart-test: $(OBJ_DIR)/softuart-test.hex
	cp $(OBJ_DIR)/softuart-test.hex $(OBJ_DIR)/firmware.hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/softuart-test.elf: $(SOFTUART_TEST_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SOFTUART_TEST_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
