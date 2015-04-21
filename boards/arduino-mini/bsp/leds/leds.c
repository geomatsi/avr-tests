#include <avr/io.h>

#include "leds.h"

void leds_init (void)
{
    DDRB = (1 << DDB5);
}

void led_on(uint32_t led)
{
	PORTB |= (1 << PB5);
}

void led_off(uint32_t led)
{
	PORTB &= ~(1 << PB5);
}

void led_toggle(uint32_t led)
{
	PORTB ^= (1 << PB5);
}
