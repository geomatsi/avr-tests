#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "clock.h"
#include "adc.h"

#include "radio.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "msg.pb.h"

/* */

#define PB_LIST_LEN	3

/* */

uint32_t get_battery_voltage(void);
uint32_t get_temp(void);

/* */

static bool sensor_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    sensor_data sensor = {};

    uint32_t data[PB_LIST_LEN];
    uint32_t idx;

	/* seq number */
    data[0] = (uint32_t)(*arg);

	/* battery voltage */
    data[1] = get_battery_voltage();

	/* temperature */
    data[2] = get_temp();

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

/* leds */

static void led_init(void)
{
    /* define PB3 as LED, set it to zero */
	DDRB |= (1 << DDB3);
    PORTB &= ~(1 << PB3);
}

static void led_toggle()
{
	PORTB ^= (1 << PB3);
}

static void led_on()
{
	PORTB |= (1 << PB3);
}

static void led_off()
{
	PORTB &= ~(1 << PB3);
}

static void led_blink(uint16_t count, uint16_t delay)
{
	uint16_t i;

	for(i = 0; i < count; i++) {
		led_on();
		delay_ms(delay);
		led_off();
		delay_ms(delay);
	}
}

/* read ds18B20 temp sensor */

uint32_t get_temp(void)
{
	return (uint32_t)42;
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
	uint8_t buf[20];

	uint8_t rf24_status;
	int ret;

	sensor_data_list message = {};
    pb_ostream_t stream;
    bool pb_status;
    size_t pb_len;

	uint32_t count = 0;

	/* */

	led_init();
	led_blink(3, 100);

	nrf = radio_init();

	delay_ms(500);

	rf24_stop_listening(nrf);
	rf24_enable_dynamic_payloads(nrf);
	rf24_set_retries(nrf, 10 /* retry delay 2500us */, 5 /* retries */);
	rf24_open_writing_pipe(nrf, addr);
	rf24_power_up(nrf);

	led_blink(3, 100);

	while (1) {

		memset(buf, 0x0, sizeof(buf));
		stream = pb_ostream_from_buffer(buf, sizeof(buf));

        message.sensor.funcs.encode = &sensor_callback;
        message.sensor.arg = (void *)count;
		count++;

        pb_status = pb_encode(&stream, sensor_data_list_fields, &message);
        pb_len = stream.bytes_written;

		if (!pb_status) {
			led_blink(5, 50);
		}


		ret = rf24_write(nrf, buf, pb_len);
		if (ret) {
			rf24_status = rf24_flush_tx(nrf);
			led_blink(3, 100);
        }

		led_toggle();
		delay_ms(1000);
	}

	return 1;
}
