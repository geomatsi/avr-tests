#include <util/delay.h>
#include <avr/io.h>

#include "uart.h"
#include "adc.h"

/* */

int main(void)
{
    uint32_t v;

    /* init s/w uart: NB PB0=RX PB1=TX */

    uart_init();

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
