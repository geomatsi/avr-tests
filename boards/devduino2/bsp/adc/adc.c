#include <util/delay.h>
#include <avr/io.h>

#include "clock.h"
#include "adc.h"

uint16_t analog_read(uint8_t ref, uint8_t pin)
{
	uint16_t result;
	uint16_t high, low;

	/* select Vref = AVcc = Vcc, select Vin = internal 1v1 */
	ADMUX = ((ref & 0x3) << REFS0) | (pin & 0xf);

	/* set prescaler and enable adc */
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADEN);

	/* FIXME: do we have to wait for Vref to settle ??? */
	/* delay_ms(50); */

	/* start conversion */
	ADCSRA |= _BV(ADSC);

	/* measuring */
	while (bit_is_set(ADCSRA, ADSC));

	/* must read ADCL first: it then locks ADCH */
	low  = ADCL;

	/* reading ADCH unlocks both ADCH and ADCL */
	high = ADCH;

	result = (high << 8) | low;

	/* disable ADC */
	ADCSRA &= ~(_BV(ADEN));

	return result;
}
