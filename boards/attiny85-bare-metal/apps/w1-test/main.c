#include <util/delay.h>

#include "w1.h"
#include "ds18b20.h"
#include "uart.h"

/* */

int main(void)
{
	uint8_t data[9];
	uint8_t i;
	int temp;
	bool ret;

	/* init s/w uart: NB PB0=RX PB1=TX */

	uart_init();

	if (!ds18b20_set_res(R12BIT)) {
		printf("WARN: couldn't set resolution\n");
	}

	/* main loop */

	while (1) {

		/* reset and check presence */
		ret = w1_init_transaction();
		if (!ret) {
			printf("presence not detected...\n");
			_delay_ms(1000);
			continue;
		}

#if 1

		/* skip ROM: next command can be broadcasted */
		w1_send_byte(SKIP_ROM);

		/* start single temperature conversion */
		w1_send_byte(CONVERT_T);

		/* temperature conversion takes ~1sec */
		_delay_ms(1000);

		/* reset and check presence */
		ret = w1_init_transaction();
		if (!ret) {
			printf("presence after conversion not detected...\n");
			_delay_ms(2000);
			continue;
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
		printf("CRC: %s\n", ds18b20_crc_check(data, 9) ? "OK" : "FAIL");
		_delay_ms(100);

		/* calculate temperature */
		temp = ds18b20_get_temp(data[1], data[0]);

		printf("temperature %c%d\n", (temp >= 0) ? '+' : ' ', temp);

#else
		/* short test command sequence: it should read 1w family code 0x28 */
		w1_send_byte(READ_ROM);
		data[0] = w1_recv_byte();
		printf("ROM[0] = %d\n", data[0]);

#endif

		/* wait before starting next cycle */
		_delay_ms(1000);
	}
}
