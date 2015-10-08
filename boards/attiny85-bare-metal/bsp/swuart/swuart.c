#include <avr/interrupt.h>

#include "softuart.h"

#include "uart.h"

FILE sw_uart_io = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

void uart_init(void)
{
    /* init s/w uart: PB0=RX PB1=TX */

    softuart_init();
	softuart_turn_rx_on();
	sei();

    /* sw: redirect streams to/from soft uart */

    stdout = &sw_uart_io;
    stderr = &sw_uart_io;
    stdin  = &sw_uart_io;
}

int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        softuart_putchar('\r');

    softuart_putchar(c);
    return 0;
}

int uart_getchar(FILE *stream)
{
    char c;

    while(!softuart_kbhit());
    c = softuart_getchar();

    return c;
}
