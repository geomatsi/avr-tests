#include <util/delay.h>
#include <avr/io.h>

#include "leds.h"

int main (void)
{

	leds_init();

	while (1) {
		led_toggle(0);
		_delay_ms(500);
	}

	return 1;
}
