#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include "adc.h"

/* s/w uart */

#include "softuart.h"
#include <stdio.h>

static int swuart_putchar( char c, FILE *stream )
{
	if (c == '\n')
    {
		softuart_putchar('\r');
	}

	softuart_putchar(c);
	return 0;
}

FILE swuart_stream = FDEV_SETUP_STREAM(swuart_putchar, NULL, _FDEV_SETUP_WRITE);

void swuart_init(void)
{
    softuart_init();
	softuart_turn_rx_on();

	sei();

    /* sw: redirect stream to swuart */
    stdout = &swuart_stream;
}

/* */

int main(void)
{
    uint32_t v;

    /* init s/w uart: NB PB0=RX PB1=TX */

    swuart_init();

    /* set PB2 as output and turn on LED */

	DDRB |= (1 << DDB2);
    PORTB |= (1 << PB2);

    /* init adc: Vref = Vcc = 5v0, select A2(PB4) channel */

    adc_scm_init(0, 2);

    /* main loop */

    while(1)
    {
        /* toggle LED */

        PORTB ^= (1 << PB2);

        /* read adc */

        v = adc_scm_read();

        /* translate adc measurement to voltage according to Vref = VCC */

        v = v * 5000 / 1024;

        /* */

        printf("adc[%u]\n", (unsigned int) v);
        _delay_ms(500);
    }

}
