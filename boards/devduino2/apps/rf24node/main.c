#include <avr/io.h>
#include <stdio.h>

#include "clock.h"

#include "radio.h"
#include "uart.h"
#include "led.h"

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main (void)
{
	struct rf24 *nrf;
	uint32_t i = 0;

	stdout = &uart_stream;
	stderr = &uart_stream;

	uart_init();

	printf("led_init...\n");
	led_init();

	printf("radio_init...\n");
	nrf = radio_init();

	delay_ms(1000);

	printf("Data Rate	= %d\n", (int) rf24_get_data_rate(nrf));
	printf("Model		= %d\n", (int) rf24_is_p_variant(nrf));
	printf("CRC Length	= %d\n", (int) rf24_get_crc_length(nrf));
	printf("PA Power	= %d\n", (int) rf24_get_pa_level(nrf));

	while (1){

		printf("cycle[%u]\n", (unsigned int) (i++));

		printf("Model = %d\n", (int) rf24_is_p_variant(nrf));

		led_on();
		delay_ms(500);
		led_off();
		delay_ms(1000);
	}

	return 1;
}
