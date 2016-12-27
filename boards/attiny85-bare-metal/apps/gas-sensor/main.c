#include <util/delay.h>
#include <avr/io.h>

#include "uart.h"
#include "adc.h"

/* */

int main(void)
{
	uint32_t v;

	/* init s/w uart (PB0=RX PB1 = TX): NB TX only for debug purposes */
	uart_init();

	/* re-define PB0 as LED, set it to zero */
	DDRB |= (1 << DDB0);
	PORTB &= ~(1 << PB0);

	/* set PB2 as ALERT OUTPUT, set it to zero */
	DDRB |= (1 << DDB2);
	PORTB &= ~(1 << PB2);

	/* init adc: Vref = Vcc = 5v0, select A2(PB4) input channel */
	adc_scm_init(0, 2);

	/* main loop */
	while (1) {
		/* toggle LED */
		PORTB ^= (1 << PB0);

		/* read gas sensor once a second */
		v = (uint32_t)adc_scm_read();

		v = v * 5000 / 1024;
		printf("gas:%u\n", (unsigned int) v);

		/* set alert if gas level is high enough */
		if (v > 500) {
			PORTB |= (1 << PB2);
		} else {
			PORTB &= ~(1 << PB2);
		}

		_delay_ms(1000);
	}

}
