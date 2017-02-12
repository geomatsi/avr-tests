#include "radio.h"
#include "spi.h"

/* */

void spi_pin_init(void)
{
	/* NRF24 CSN: PB4 GPIO */
	DDRB |= (1 << DDB4);
	PORTB &= ~(1 << PB4);

	/* NRF24 CE: wired to VCC in hardware */
}

/* */

void f_csn(int level)
{
	if (level > 0) {
		PORTB |= (1 << PB4);
	} else {
		PORTB &= ~(1 << PB4);
	}
}

void f_ce(int level)
{
	/* don't spend pin: CE is wired to VCC */
}

uint8_t f_spi_xfer(uint8_t dat)
{
	return spi_fast_shift(dat);
}

struct rf24 nrf = {
	.csn = f_csn,
	.ce = f_ce,
	.spi_xfer = f_spi_xfer,
};

/* */

struct rf24* radio_init(void)
{
	spi_pin_init();
	spi_init();
	spi_set_mode(SPI_MODE_0);

	rf24_init(&nrf);

	return &nrf;
}
