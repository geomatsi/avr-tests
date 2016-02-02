#
#
#

## platform definitions

ARCH = avr
PLAT ?= attiny85-bare-metal

## project and output directories

TOP_DIR = $(shell pwd)
OBJ_DIR = $(shell pwd)/out
PRJ_DIR = $(shell pwd)/tests

## default build target: print info

all: info

## dependencies

# libsoftuart

SOFTUART_LIB_DIR	= softuart
LIBSOFTUART_INC		= -I$(SOFTUART_LIB_DIR)
LIBSOFTUART			= $(SOFTUART_LIB_DIR)/libsoftuart_$(CHIP).a

# libnrf24

NRF24_LIB_DIR	= libnrf24
LIBNRF24_INC	= -I$(NRF24_LIB_DIR)/include
LIBNRF24		= $(NRF24_LIB_DIR)/libnrf24_$(CHIP).a

# nanopb

NANOPB_DIR		= nanopb
NANOPB_INC		= -I$(NANOPB_DIR)

## target specific definitions

include $(PRJ_DIR)/boards/$(PLAT)/platform.mk

## information

info:
	@echo "ARCH = $(ARCH)"
	@echo "PLAT = $(PLAT)"
	@echo "FREQ = $(CLK_FREQ)"
	@echo "BUILD TARGETS = $(TARGETS)"

## build rules for dependencies

libsoftuart:
	make -C softuart \
		MCU=$(CHIP) \
		F_CPU=$(subst L,,$(CLK_FREQ)) \
		lib
	mv $(SOFTUART_LIB_DIR)/libmain.a $(SOFTUART_LIB_DIR)/libsoftuart_$(CHIP).a

libnrf24:
	make -C libnrf24 \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		TARGET=$(CHIP) \
		PLT_FLAGS="$(PFLAGS)" \
		CFG_FLAGS="$(NRF24_CFG_FLAGS)"

nanopb:
	make -C nanopb/generator/proto

## clean rules

clean:
	rm -rf $(OBJ_DIR)

distclean:
	make -C softuart clean
	rm -rf $(LIBSOFTUART)
	make -C libnrf24 CROSS_COMPILE=$(CROSS_COMPILE) TARGET=$(CHIP) PLT_FLAGS="$(PFLAGS)" clean
	rm -rf $(OBJ_DIR)

.PHONY: libsoftuart
.PHONY: libnrf24
.PHONY: nanopb
.PHONY: distclean
.PHONY: clean
