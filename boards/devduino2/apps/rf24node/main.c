#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "clock.h"
#include "vcc.h"

#include "radio.h"
#include "uart.h"
#include "led.h"

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main (void)
{
	struct rf24 *nrf;

	uint8_t addr[] = {'E', 'F', 'C', 'L', 'I'};
	uint8_t buf[20];

	uint32_t count = 0;
	uint8_t status;
	long vcc;
	int ret;

	stdout = &uart_stream;
	stderr = &uart_stream;

	uart_init();

	printf("led_init...\n");
	led_init();

	printf("radio_init...\n");
	nrf = radio_init();

	delay_ms(500);

	rf24_stop_listening(nrf);
	rf24_set_payload_size(nrf, sizeof(buf));
	rf24_set_retries(nrf, 10 /* retry delay 2500us */, 5 /* retries */);
	rf24_open_writing_pipe(nrf, addr);
	rf24_power_up(nrf);

	while (1){

		memset(buf, 0x0, sizeof(buf));
		sprintf((char *) buf, "node 0x%08x", (unsigned int) count++);
		printf("xmit buffer: sizeof(%s) = %d\n", buf, sizeof(buf));

		ret = rf24_write(nrf, buf, sizeof(buf));
		if (ret) {
			printf("write error: %d\n", ret);
			status = rf24_flush_tx(nrf);
		}

		led_toggle();
		delay_ms(1000);

		vcc = read_vcc();
		printf("... vcc = %ld\n", vcc);
	}

	return 1;
}
