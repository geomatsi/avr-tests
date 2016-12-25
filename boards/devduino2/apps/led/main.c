#include <util/delay.h>
#include <avr/io.h>

#include "leds.h"

int main (void)
{

	leds_init();
	led_on(0);

	while (1) {
		_delay_ms(500);
		led_toggle(0);
	}

	return 1;
}
