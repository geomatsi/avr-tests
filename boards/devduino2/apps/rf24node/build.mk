#
# example: NODE
#

## paths

VPATH += $(TOP_DIR)/nanopb

VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/rf24node
VPATH += $(PRJ_DIR)/boards/$(PLAT)/apps/rf24node/protobuf
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/clock
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/leds
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/spi
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/vcc
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/adc
VPATH += $(PRJ_DIR)/boards/$(PLAT)/bsp/temp

## protobuf

PROTOC ?= protoc
PROTOBUF_SRC = $(PRJ_DIR)/boards/$(PLAT)/apps/rf24node/protobuf

## sources

NODE_SRCS := \
	main.c	\
	uart.c	\
	leds.c	\
	spi.c	\
	clock.c	\
	radio.c \
	vcc.c	\
	adc.c	\
	temp_mcp9700.c \
	pb_common.c \
	pb_encode.c \
	msg.pb.c \

NODE_OBJS := $(NODE_SRCS:.c=.o)
NODE_OBJS := $(addprefix $(OBJ_DIR)/,$(NODE_OBJS))

## deps

LIBS = $(LIBNRF24)

## flags

CFLAGS	= -Wall -Werror $(PFLAGS)

# tweak nanopb to fit into attiny
CFLAGS += -DPB_BUFFER_ONLY -DPB_NO_ERRMSG

CFLAGS	+= $(LIBNRF24_INC) $(NANOPB_INC)
CFLAGS	+= -I$(PRJ_DIR)/include
CFLAGS	+= -I$(OBJ_DIR)/nanopb

LDFLAGS	= -Wl,-Map,$(PRG).map

## rules

rf24node: $(OBJ_DIR)/rf24node.hex $(OBJ_DIR)/rf24node.bin
	cp $(OBJ_DIR)/rf24node.hex $(OBJ_DIR)/firmware.hex
	avr-size --mcu=$(CHIP) --format=avr $(OBJ_DIR)/rf24node.elf

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

$(OBJ_DIR)/rf24node.elf: $(NODE_OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(NODE_OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)/msg.pb.h
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.pb.c $(OBJ_DIR)/%.pb.h: $(PROTOBUF_SRC)/%.proto
	mkdir -p $(OBJ_DIR)/nanopb
	$(PROTOC) --proto_path=$(PROTOBUF_SRC) -o $(OBJ_DIR)/$*.pb $(PROTOBUF_SRC)/$*.proto
	python $(TOP_DIR)/nanopb/generator/nanopb_generator.py $(OBJ_DIR)/$*.pb
	# FIXME: why msg.pb.h is removed unless stored in another directory ?
	mv $(OBJ_DIR)/msg.pb.h $(OBJ_DIR)/nanopb
