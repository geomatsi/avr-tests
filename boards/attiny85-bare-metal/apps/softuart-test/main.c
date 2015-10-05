#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

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
    uint8_t v = 0;

    /* init s/w uart: NB PB0=RX PB1=TX */

    swuart_init();

    /* set PB2 as output and turn on LED */

	DDRB |= (1 << DDB2);
    PORTB |= (1 << PB2);

    /* main loop */

    while(1)
    {
        PORTB ^= (1 << PB2);
        printf("hello[%d]\n", (int) v++);
        _delay_ms(1000);
    }

}
