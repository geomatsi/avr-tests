#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "leds.h"

int main (void)
{
	char ch;

	leds_init();
	uart_init();

	while(1) {

		led_toggle(0);

		puts("enter char: ");
		ch = getchar();
		printf("You wrote %c\n", ch);
	}

	return 1;
}
