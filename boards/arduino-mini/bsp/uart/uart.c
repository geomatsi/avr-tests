#include <avr/interrupt.h>
#include <avr/io.h>

#include "uart.h"

#define UART_BAUD  115200

FILE uart_io = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

void uart_init(void)
{
    /* hw: init uart 115200 8N1 */

    UCSR0A = _BV(U2X0);
    UBRR0L = (F_CPU / (8UL * UART_BAUD)) - 1;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);

    /* sw: redirect streams to/from uart */

    stdout = &uart_io;
    stderr = &uart_io;
    stdin  = &uart_io;
}

int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;

    return 0;
}

int uart_getchar(FILE *stream)
{
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}
