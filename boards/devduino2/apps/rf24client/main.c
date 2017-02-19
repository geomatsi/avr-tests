#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "clock.h"
#include "leds.h"
#include "vcc.h"
#include "temp_mcp9700.h"

#include "radio.h"
#include "uart.h"

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main (void)
{
	struct rf24 *nrf;
	enum rf24_tx_status ret;

	uint8_t addr[] = {'E', 'F', 'C', 'L', 'I'};
	uint8_t buf[20];

	uint32_t count = 0;
	long vcc, temp;

	stdout = &uart_stream;
	stderr = &uart_stream;

	uart_init();

	printf("led_init...\n");
	leds_init();

	printf("radio_init...\n");
	nrf = radio_init();

	delay_ms(500);

	rf24_set_payload_size(nrf, sizeof(buf));
	rf24_set_retries(nrf, 0xf /* retry delay 4000us */, 5 /* retries */);

	rf24_set_channel(nrf, 10);
	rf24_set_data_rate(nrf, RF24_RATE_250K);
	rf24_set_crc_mode(nrf, RF24_CRC_16_BITS);
	rf24_set_pa_level(nrf, RF24_PA_MAX);

	rf24_setup_ptx(nrf, addr);
	rf24_start_ptx(nrf);

	while (1){

		vcc = read_vcc();
		printf("... vcc = %ld\n", vcc);

		temp = read_temp_mcp9700();
		printf("... temp = %ld\n", temp);

		memset(buf, 0x0, sizeof(buf));
		snprintf((char *) buf, sizeof(buf) - 1, "p%u:v[%ld]t[%ld]",
			 (unsigned int) count++, vcc, temp);
		printf("xmit buffer: sizeof(%s) = %d\n", buf, sizeof(buf));


		ret = rf24_send(nrf, buf, sizeof(buf));
		if (ret != RF24_TX_OK) {
			printf("send error: %d\n", ret);
			rf24_flush_tx(nrf);
			rf24_flush_rx(nrf);
		}

		led_toggle(0);
		delay_ms(1000);
	}

	return 1;
}
