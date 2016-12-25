#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "leds.h"

#define blink(led, count, msec)		\
do {					\
	int c = count;			\
	for (; c > 0; c--) {		\
		led_on(led);		\
		_delay_ms(msec);	\
		led_off(led);		\
		if (c)			\
			_delay_ms(msec);\
	}				\
} while(0);


ISR(WDT_vect)
{
	wdt_disable();
}

int main (void)
{

	leds_init();

	blink(0, 5, 100);

	while (1){

		/* disable ADC */
		ADCSRA &= ~_BV(ADEN);

		/* disable all peripherals */
		power_all_disable();

		/* setup watchdog timer and its interrupt */
		wdt_enable(WDTO_8S);
		WDTCSR |= _BV(WDIE);

		/* power down mode with BOD off */
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		cli();
		sleep_enable();
		sleep_bod_disable();
		sei();
		sleep_cpu();
		sleep_disable();
		sei();

		blink(0, 3, 500);
	}

	return 1;
}
