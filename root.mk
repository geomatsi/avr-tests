#
#
#

## platform definitions

ARCH = avr
PLAT ?= devduino2

## project and output directories

OBJ_DIR = $(shell pwd)/out
PRJ_DIR = $(shell pwd)/tests

## default build target: print info

all: info

info:
	@echo "ARCH = $(ARCH)"
	@echo "PLAT = $(PLAT)"
	@echo "BUILD TARGETS = $(TARGETS)"

## dependencies

# libnrf24

LIBNRF24		= $(NRF24_LIB_DIR)/libnrf24_$(ARCH)_$(PLAT).a
NRF24_LIB_DIR	= libnrf24
CFLAGS			+= -I$(NRF24_LIB_DIR)/include

## target specific definitions

include $(PRJ_DIR)/boards/$(PLAT)/platform.mk

## build rules for dependencies

deps: libnrf24

libnrf24:
	make -C libnrf24 CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH) PLAT=$(PLAT) PFLAGS="$(PFLAGS)"

## clean rules

clean:
	rm -rf $(OBJ_DIR)

distclean:
	make -C libnrf24 CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH) PLAT=$(PLAT) PFLAGS="$(PFLAGS)" clean
	rm -rf $(OBJ_DIR)

.PHONY: libnrf24
.PHONY: distclean
.PHONY: clean
