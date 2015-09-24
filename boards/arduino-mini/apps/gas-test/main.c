#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "leds.h"
#include "gas.h"

/*
 * Wait (100 * 160000) cycles = wait 16000000 cycles.
 * Equivalent to 1 second at 16 MHz.
 */
void delay_10ms(int num){
    uint16_t i;
    for (i=0; i < num; i++){
        /* wait (40000 x 4) cycles = wait 160000 cycles */
        _delay_loop_2(40000);
    }
}

int main (void)
{
    uint32_t gas = 0;

	uart_init();
	leds_init();

	while (1){

        gas = read_gas_mq9(0x0);
		printf("gas[%u]\n", gas);
		led_on(0);
		delay_10ms(100);

        gas = read_gas_mq9(0x0);
		printf("gas[%u]\n", gas);
		led_off(0);
		delay_10ms(100);
	}

	return 1;
}
