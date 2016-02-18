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

#define PB_LIST_LEN 3

/* */

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

/* */

bool sensor_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
	uint32_t *dptr = (uint32_t *)(*arg);
    sensor_data sensor = {};

    uint32_t data[PB_LIST_LEN];
    uint32_t idx;

	data[0] = (uint32_t)(*dptr);
    data[1] = (uint32_t)read_vcc();
    data[2] = (uint32_t)read_temp_mcp9700();

    for (idx = 0; idx < PB_LIST_LEN; idx++) {

        printf("protobuf encoding: (%lu, %lu)\n", idx, data[idx]);

        sensor.type = idx;
        sensor.data = data[idx];

        /* Encode the header for the field, based on the constant info from pb_field_t */
        if (!pb_encode_tag_for_field(stream, field)) {
			printf("protobuf tag encoding failed: %s\n", PB_GET_ERROR(stream));
            return false;
        }

        /* Encode the data for the field, based on our sensor_data structure */
        if (!pb_encode_submessage(stream, sensor_data_fields, &sensor)) {
			printf("protobuf submessage encoding failed: %s\n", PB_GET_ERROR(stream));
            return false;
        }
    };

    return true;
}

/* */

int main (void)
{
	struct rf24 *nrf;

	uint8_t addr[] = {'E', 'F', 'C', 'L', 'I'};
	uint8_t buf[20];

	uint32_t count = 0;
	uint8_t rf24_status;
	int ret;

	sensor_data_list message = {};
    pb_ostream_t stream;
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

		memset(buf, 0x0, sizeof(buf));
		stream = pb_ostream_from_buffer(buf, sizeof(buf));

        message.sensor.funcs.encode = &sensor_callback;
        message.sensor.arg = (void *)&count;
		count++;

        pb_status = pb_encode(&stream, sensor_data_list_fields, &message);
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
