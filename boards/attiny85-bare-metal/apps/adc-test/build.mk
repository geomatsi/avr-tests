#
# example: adc test
#

## paths

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/adc-test

## sources

ADC_TEST_SRCS := \
	main.c

ADC_TEST_OBJS := $(ADC_TEST_SRCS:.c=.o)
ADC_TEST_OBJS := $(addprefix $(OBJ_DIR)/,$(ADC_TEST_OBJS))

## deps

LIBS = $(LIBSOFTUART)

## flags

CFLAGS	= -Wall -O2 $(PFLAGS)

CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(PRJ_DIR)/boards/$(PLAT)/apps/adc-test
CFLAGS	+= $(LIBSOFTUART_INC)

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

adc-test: $(OBJ_DIR)/adc-test.hex
	cp $(OBJ_DIR)/adc-test.hex $(OBJ_DIR)/firmware.hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

$(OBJ_DIR)/adc-test.elf: $(ADC_TEST_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(ADC_TEST_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^
