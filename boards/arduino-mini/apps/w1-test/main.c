#include <stdbool.h>
#include <stdio.h>

#include "clock.h"
#include "uart.h"

#include "w1.h"
#include "ds18b20.h"

/* */

int main (void)
{
	uint8_t data[9];
	uint8_t i;
	int temp;
	bool ret;

    uart_init();

	if (!ds18b20_set_res(R12BIT)) {
		printf("WARN: couldn't set resolution\n");
	}

	while (1) {

		printf("start 1-wire cycle\n");

		/* reset and check presence */
		ret = w1_init_transaction();
		if (!ret) {
			printf("presence not detected: wait for 2 sec...\n");
			delay_ms(2000);
			continue;
		}

		delay_ms(1);

		/* skip ROM: next command can be broadcasted */
		w1_send_byte(SKIP_ROM);

		/* start single temperature conversion */
		w1_send_byte(CONVERT_T);

		/* temperature conversion takes ~1sec */
		delay_ms(1000);

		/* reset and check presence */
		ret = w1_init_transaction();
		if (!ret) {
			printf("presence not detected: wait for 2 sec...\n");
			delay_ms(2000);
			continue;
		}

		delay_ms(1);

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

		/* calculate temperature */
		temp = ds18b20_get_temp(data[1], data[0]);

		printf("temperature %c%d\n", (temp >= 0) ? '+' : ' ', temp);

		/* wait before starting next cycle */
		delay_ms(2000);
	}

    return 0;
}
