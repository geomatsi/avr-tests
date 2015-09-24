#ifndef _AVR_UART_H_
#define _AVR_UART_H_

#include <avr/io.h>

/* */

void uart_init(void);

int	uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

#endif /* _AVR_UART_H_ */
