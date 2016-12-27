#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "clock.h"
#include "uart.h"

#include "radio.h"

/* */

void led_init(void)
{
	/* define PB3 as LED, set it to zero */
	DDRB |= (1 << DDB3);
	PORTB &= ~(1 << PB3);
}

void led_toggle()
{
	PORTB ^= (1 << PB3);
}

void led_on()
{
	PORTB |= (1 << PB3);
}

void led_off()
{
	PORTB &= ~(1 << PB3);
}

void led_blink(uint16_t count, uint16_t delay)
{
	uint16_t i;

	for(i = 0; i < count; i++) {
		led_on();
		delay_ms(delay);
		led_off();
		delay_ms(delay);
	}
}

/* */

int main (void)
{
	struct rf24 *nrf;

	uint8_t addr[] = {'E', 'F', 'C', 'L', 'I'};
	uint8_t buf[20];

	uint8_t rf24_status;
	int ret;

	uint32_t count = 0;

	/* */

	led_init();
	led_blink(3, 100);

	nrf = radio_init();

	delay_ms(500);

	rf24_stop_listening(nrf);
	rf24_set_payload_size(nrf, sizeof(buf));
	rf24_set_retries(nrf, 10 /* retry delay 2500us */, 5 /* retries */);
	rf24_open_writing_pipe(nrf, addr);
	rf24_power_up(nrf);

	led_blink(3, 100);

	while (1) {

		memset(buf, 0x0, sizeof(buf));
		snprintf((char *) buf, sizeof(buf) - 1, "p:%u", (unsigned int)count++);

		ret = rf24_write(nrf, buf, sizeof(buf));
		if (ret) {
			rf24_status = rf24_flush_tx(nrf);
		}

		led_toggle();
		delay_ms(1000);
	}

	return 1;
}
