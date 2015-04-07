#include "radio.h"
#include "spi.h"

/* rf24 pins on devduino2 */

#define DD_MOSI     DDB3
#define DD_MISO     DDB4
#define DD_SCK      DDB5

#define DD_CSN		DDD7
#define DD_CE       DDB0

/* */

void spi_init_pins(void)
{
	/* reset pins */
    DDRB &= ~((1<<DD_MOSI)|(1<<DD_MISO)|(1<<DD_SCK)|(1<<DD_CE));
    DDRD &= ~(1<<DD_CSN);

    /* define the following pins as output */
    DDRB |= ((1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_CE));
    DDRD |= (1<<DD_CSN);
}

/* */

void f_csn(int level)
{
	if (level > 0) {
		PORTD |= (1 << PD7);
	} else {
		PORTD &= ~(1 << PD7);
	}
}

void f_ce(int level)
{
	if (level > 0) {
		PORTB |= (1 << PB0);
	} else {
		PORTB &= ~(1 << PB0);
	}
}

void f_spi_set_speed(int khz)
{
	/* not implemented */
}

uint8_t f_spi_xfer(uint8_t dat)
{
	return spi_fast_shift(dat);
}

struct rf24 nrf = {
	.csn = f_csn,
	.ce = f_ce,
	.spi_set_speed = f_spi_set_speed,
	.spi_xfer = f_spi_xfer,
};

/* */

struct rf24* radio_init(void)
{
	spi_init_pins();
	spi_init();

	rf24_init(&nrf);

	return &nrf;
}
