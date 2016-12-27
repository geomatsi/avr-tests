#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "leds.h"
#include "adc.h"

/* */

#define	lpm_bod_off(mode)			\
	do {					\
		set_sleep_mode(mode);		\
		cli();				\
		sleep_enable();			\
		sleep_bod_disable();		\
		sei();				\
		sleep_cpu();			\
		sleep_disable();		\
		sei();				\
	} while (0);

#define wdt_setup(period)		\
	do {				\
		wdt_enable(period);	\
		WDTCSR |= _BV(WDIE);	\
	} while (0);



ISR(WDT_vect)
{
	wdt_disable();
}

/* */

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

/* */

int main (void)
{

	leds_init();

	/* disable ADC and other peripherals */
	adc_disable();
	power_all_disable();

	blink(0, 5, 100);

	while (1){
		/* setup watchdog timer and its interrupt */
		wdt_setup(WDTO_8S);

		/* power down mode with BOD off */
		lpm_bod_off(SLEEP_MODE_PWR_DOWN);

		/* blink on wake up */
		blink(0, 3, 500);
	}

	return 1;
}
