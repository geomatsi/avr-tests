
SERIAL_PORT = /dev/ttyUSB0
MCU_TARGET	= atmega328p
CLK_FREQ    = 16000000L

#

DUDE_OPTIONS = -p $(MCU_TARGET) -P $(SERIAL_PORT) -c arduino -b 57600 -F

#

PRG		= main
OBJ		= main.o uart.o led.o

# Toolchain

CC             = avr-gcc
OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

#

LIBS =

# Override is only needed by avr-lib build system.

OPTIMIZE       = -O2
DEFS           = -DF_CPU=$(CLK_FREQ)

override CFLAGS		= -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS    = -Wl,-Map,$(PRG).map

#

all: $(PRG).elf

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

upload: hex
	avrdude  $(DUDE_OPTIONS) -U flash:w:$(PRG).hex

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

clean:
	rm -rf *.o $(PRG).elf
	rm -rf *.map *.hex *.bin *.srec
