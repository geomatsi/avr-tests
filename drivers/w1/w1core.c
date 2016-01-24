/* simple blocking 1-wire implementation */

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include <stdbool.h>
#include <stdio.h>

#include "w1.h"

/* */

static struct w1_pin *pin;

/* set IO registers */

void w1_set_pin(struct w1_pin *p)
{
	pin = p;
}

/* 1wire reset and check presence */

bool w1_init_transaction(void)
{
	uint8_t val;

	cli();

	/* 1wire reset */
	pin->pin_low();
	_delay_us(600);

	/* 1wire relax */
	pin->pin_high();
	_delay_us(80);

	/* 1wire check presence */
	val = pin->pin_value();
	_delay_us(520);

	sei();

	return val ? false : true;
}

/* send 1wire data to device */

void w1_send_byte(uint8_t byte)
{
	uint8_t bit;
	int i;

	cli();

	for(i = 0; i < 8; i++) {

		bit = (byte >> i) & 0x01;
		pin->pin_low();

		if (0 == bit) {
			_delay_us(80);
			pin->pin_high();
		} else {
			_delay_us(10);
			pin->pin_high();
			_delay_us(70);
		}

		/* min here is 1 usec */
		_delay_us(20);
	}

	sei();
}

/* recv 1wire data from device */

uint8_t w1_recv_byte(void)
{
	uint8_t byte;
	uint8_t bit;
	int i;

	byte = 0;

	cli();

	for (i = 0; i < 8; i++) {

		pin->pin_low();
		_delay_us(5);

		pin->pin_high();
		_delay_us(20);

		bit = pin->pin_value();
		_delay_us(55);

		if (1 == bit) {
			byte |= (0x1 << i);
		}

		_delay_us(20);
	}

	sei();

	return byte;
}
