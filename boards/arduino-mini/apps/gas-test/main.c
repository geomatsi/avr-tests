#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "leds.h"
#include "adc.h"

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

uint32_t read_gas_mq(void)
{
    uint32_t result;

    result = adc_scm_read();

    /*
     * Sanity check:
     * ADC = Vin*1024/Vref = (Vin/Vref)*1024
     * Vref = 5.0v, Vin <= 5.0v =>
     * Vin/Vref <= 1 =>
     * ADC = (Vin/Vref)*1024 < 1024 =>
     * ADC fits to 10bit resolution
     *
     * Calculate Vcc in mV:
     * ADC = (Vin/Vref)*1024 = Vin*1024/5000mV
     * Vin = 5000mV * (ADC/1024)
     */

    result = result * 5000 / 1024;

    return result;
}
int main (void)
{
    uint32_t gas = 0;

	uart_init();
	leds_init();


    /* Vref = AVcc = Vcc = 5V, input channel Vin = A0(ADC0) */
    adc_scm_init(0x1, 0x0);

	while (1){

        gas = read_gas_mq();
		printf("gas[%u]\n", gas);
		led_on(0);
		delay_10ms(100);

        gas = read_gas_mq();
		printf("gas[%u]\n", gas);
		led_off(0);
		delay_10ms(100);
	}

	return 1;
}
