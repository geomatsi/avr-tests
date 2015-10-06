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

void adc_init(void)
{
    /* ADC settings I
     *     ref selection bits = x00 => Vref = VCC (in our case 5v0)
     *     pin mux = 0010 => use ADC2 (PB4) for input
     */

    ADMUX = (0 << ADLAR) | (0 << REFS1) | (0 << REFS0) | (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0);

    /* ADC settings II
     *     prescaler bits = 011 => div = 8 => ADC freq = 125 kHz for F_CPU = 1 MHz
     */

    ADCSRA = (1 << ADEN) | (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(void)
{
    uint16_t r, h, l;

    ADCSRA |= (1 << ADSC);
    while (bit_is_set(ADCSRA, ADSC));

    /* NB: low byte must be read first - it then locks high byte */
    l = ADCL;
    h = ADCH;
    r = (h << 8) | l;

    return r;
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

    /* init adc */

    adc_init();

    /* main loop */

    while(1)
    {
        /* toggle LED */

        PORTB ^= (1 << PB2);

        /* read adc */

        v = adc_read();

        /* translate adc measurement to voltage according to Vref = VCC */

        v = v * 5000 / 1024;

        /* */

        printf("adc[%u]\n", (unsigned int) v);
        _delay_ms(500);
    }

}
