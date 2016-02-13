#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "clock.h"
#include "adc.h"

#include "w1.h"
#include "ds18b20.h"

#include "radio.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "msg.pb.h"

/* */

#define PB_LIST_LEN	3

/* */

uint32_t volt;
uint32_t temp;

/* */

static bool sensor_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    sensor_data sensor = {};

    uint32_t data[PB_LIST_LEN];
    uint32_t idx;

	/* seq number */
    data[0] = (uint32_t)(*arg);

	/* battery voltage */
    data[1] = volt;

	/* temperature */
    data[2] = temp;

    for (idx = 0; idx < PB_LIST_LEN; idx++) {

        sensor.type = idx;
        sensor.data = data[idx];

        /* Encode the header for the field, based on the constant info from pb_field_t */
        if (!pb_encode_tag_for_field(stream, field)) {
            return false;
        }

        /* Encode the data for the field, based on our sensor_data structure */
        if (!pb_encode_submessage(stream, sensor_data_fields, &sensor)) {
            return false;
        }
    };

    return true;
}

/* read ds18B20 temp sensor */

uint32_t get_temp(void)
{
	uint8_t data[9];
	uint8_t i;
	bool ret;
	int temp;

	/* reset and check presence */
	ret = w1_init_transaction();
	if (!ret) {
		return 1001;
	}

	/* skip ROM: next command can be broadcasted */
	w1_send_byte(SKIP_ROM);

	/* start single temperature conversion */
	w1_send_byte(CONVERT_T);

	/* temperature conversion takes ~1sec */
	_delay_ms(1000);

	/* reset and check presence */
	ret = w1_init_transaction();
	if (!ret) {
		return 1002;
	}

	/* skip ROM: careful !!! works only for one device on bus: next command is unicast */
	w1_send_byte(SKIP_ROM);

	/* read scratchpad */
	w1_send_byte(READ_PAD);

	/* get all scratchpad bytes */
	for (i = 0; i < 9; i++) {
		data[i] = w1_recv_byte();
	}

	/* check crc */
	ret = ds18b20_crc_check(data, 9);
	if (!ret) {
		return 1003;
	}

	/* calculate temperature */
	temp = ds18b20_get_temp(data[1], data[0]);

	return (uint32_t)temp;
}

/* read battery */

uint32_t get_battery_voltage(void)
{
	uint32_t v;

	/* re-init adc: Vref = Vcc ?= 3v3, Vbg (1v1) input channel */
	adc_scm_init(0, 12);
	delay_ms(5);

	/* read twice to make sure ADC is settled */
	v = (uint32_t)adc_scm_read();
	delay_ms(5);
	v = (uint32_t)adc_scm_read();

	/* Vcc = 1024 * 1100 / ADC */
	v = 1126400 / v;

	return v;
}

/* main */

int main (void)
{
	struct rf24 *nrf;

	uint8_t addr[] = {'E', 'F', 'C', 'L', 'I'};
	uint8_t buf[32];

	uint8_t rf24_status;
	int ret;

	sensor_data_list message = {};
    pb_ostream_t stream;
    bool pb_status;
    size_t pb_len;

	uint32_t count = 0;

	/* */

	volt = -1;
	temp = -1;

	ds18b20_set_res(R12BIT);

	nrf = radio_init();

	delay_ms(500);

	rf24_stop_listening(nrf);
	rf24_enable_dynamic_payloads(nrf);
	rf24_set_retries(nrf, 10 /* retry delay 2500us */, 5 /* retries */);
	rf24_open_writing_pipe(nrf, addr);
	rf24_power_up(nrf);

	while (1) {

		memset(buf, 0x0, sizeof(buf));
		stream = pb_ostream_from_buffer(buf, sizeof(buf));

        message.sensor.funcs.encode = &sensor_callback;
        message.sensor.arg = (void *)count;
		count++;

        pb_status = pb_encode(&stream, sensor_data_list_fields, &message);
        pb_len = stream.bytes_written;

		if (!pb_status) {
			/* FIXME */
		}

		ret = rf24_write(nrf, buf, pb_len);
		if (ret) {
			rf24_status = rf24_flush_tx(nrf);
        }

		volt = get_battery_voltage();
		temp = get_temp();

		_delay_ms(500);
		/* NB: additional delay - get_temp takes > 1sec */
	}

	return 1;
}
