#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "clock.h"
#include "leds.h"
#include "vcc.h"
#include "temp_mcp9700.h"

#include "radio.h"
#include "uart.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "msg.pb.h"

/* */

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main (void)
{
	struct rf24 *nrf;

	uint8_t addr[] = {'E', 'F', 'C', 'L', 'I'};
	uint8_t buf[20];

	uint8_t rf24_status;
	long vcc, temp;
	int ret;

    pb_ostream_t stream;
	sensor_data message;
    bool pb_status;
    size_t pb_len;

	stdout = &uart_stream;
	stderr = &uart_stream;

	uart_init();

	printf("led_init...\n");
	leds_init();

	printf("radio_init...\n");
	nrf = radio_init();

	delay_ms(500);

	rf24_stop_listening(nrf);
	rf24_enable_dynamic_payloads(nrf);
	rf24_set_retries(nrf, 10 /* retry delay 2500us */, 5 /* retries */);
	rf24_open_writing_pipe(nrf, addr);
	rf24_power_up(nrf);

	while (1){

		vcc = read_vcc();
		printf("... vcc = %ld\n", vcc);

		temp = read_temp_mcp9700();
		printf("... temp = %ld\n", temp);

		memset(buf, 0x0, sizeof(buf));
		stream = pb_ostream_from_buffer(buf, sizeof(buf));

        message.type = 0x1;
        message.data = (uint32_t) vcc;

        pb_status = pb_encode(&stream, sensor_data_fields, &message);
        pb_len = stream.bytes_written;

		if (!pb_status) {
			printf("protobuf encoding failed: %s\n", PB_GET_ERROR(&stream));
		} else {
			printf("protobuf encoded %u bytes\n", pb_len);
		}


		ret = rf24_write(nrf, buf, pb_len);
		if (ret) {
			printf("write error: %d\n", ret);
			rf24_status = rf24_flush_tx(nrf);
		} else {
			printf("written %d bytes\n", pb_len);
        }

		led_toggle(0);
		delay_ms(1000);
	}

	return 1;
}
