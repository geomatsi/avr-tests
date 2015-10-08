#include <avr/io.h>

#include "uart.h"

/* */

int main(void)
{
    char c;

    /* init s/w uart: NB PB0=RX PB1=TX */

    uart_init();

    /* set PB2 as output and turn on LED */

	DDRB |= (1 << DDB2);
    PORTB |= (1 << PB2);

    /* main loop */

    while(1)
    {
        c = getchar();
        printf("[%c]\n", c);
        PORTB ^= (1 << PB2);
    }

}
