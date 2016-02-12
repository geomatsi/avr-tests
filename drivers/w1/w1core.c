/* simple blocking 1-wire implementation */

#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdbool.h>
#include <stdio.h>

/*
 * w1_ops header should provide the following
 * app-specific defines to control pin:
 *	- SET_PIN_HIGH()
 *	- SET_PIN_LOW()
 *	- GET_PIN_VALUE()
 */

#include "w1_ops.h"

/* 1wire reset and check presence */

bool w1_init_transaction(void)
{
	uint8_t val;

	cli();

	/* 1wire reset */
	SET_PIN_LOW();
	_delay_us(600);

	/* 1wire relax */
	SET_PIN_HIGH();
	_delay_us(80);

	/* 1wire check presence */
	val = GET_PIN_VALUE();
	_delay_us(520);

	sei();

	return val ? false : true;
}

/* send 1wire data to device */

void w1_send_byte(uint8_t byte)
{
	volatile uint8_t bit;
	int i;

	cli();

	for(i = 0; i < 8; i++) {

		// bit is declared as volatile:
		// otherwise compiler may reorder code and perform shift after pin is set low,
		// which may significantly increase low pulse on 1MHz devices

		bit = (byte >> i) & 0x01;
		SET_PIN_LOW();

		if (bit) {
			_delay_us(2);
			SET_PIN_HIGH();
			_delay_us(58);
		} else {
			_delay_us(60);
			SET_PIN_HIGH();
		}

		/* min here is 1 usec */
		_delay_us(5);
	}

	sei();
}

/* recv 1wire data from device */

uint8_t w1_recv_byte(void)
{
	volatile uint8_t byte;
	volatile uint8_t bit;
	int i;

	byte = 0;

	cli();

	for (i = 0; i < 8; i++) {

		SET_PIN_LOW();
		_delay_us(3);
		SET_PIN_HIGH();
		_delay_us(8);

		bit = GET_PIN_VALUE();
		_delay_us(40);

		if (bit) {
			byte |= (0x1 << i);
		}

		_delay_us(5);
	}

	sei();

	return byte;
}
