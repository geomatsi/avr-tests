#include <avr/io.h>

#include "led.h"

void led_init (void)
{
	DDRB = (1 << DDB1);
}

void led_on(void)
{
	PORTB |= (1 << PB1);
}

void led_off(void)
{
	PORTB &= ~(1 << PB1);
}

void led_toggle(void)
{
	PORTB ^= (1 << PB1);
}
