#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "leds.h"

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

/*
 * Wait (100 * 160000) cycles = wait 16000000 cycles.
 * Equivalent to 1 second at 16 MHz.
 */
void delay_10ms(int num)
{
	uint16_t i;
	for (i=0; i < num; i++){
		/* wait (40000 x 4) cycles = wait 160000 cycles */
		_delay_loop_2(40000);
	}
}

int main (void)
{
	int delays[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

	unsigned int m = sizeof(delays)/sizeof(int);
	unsigned int p = 0;
	unsigned int i = 0;

	uart_init();
	leds_init();

	stdout = &uart_stream;
	stderr = &uart_stream;

	while (1){

		p = i % m;
		printf("cycle[%u]: delay = %d\n", i++, delays[p]*10);
		led_on(0);
		delay_10ms(delays[p]);

		led_off(0);
		delay_10ms(50);
	}

	return 1;
}
