#include <util/delay.h>
#include <avr/io.h>

#include "adc.h"

uint32_t read_gas_mq9(uint8_t pin)
{
    uint32_t result;

    /* select Vref = AVcc = Vcc = 5V, select Vin = A0(ADC0) */
    result = analog_read(0x1, pin);

    /* XXX Features of this approach
     *     1. Vref is Vcc
     *     2. FIXME: ADC delay in analog_read
     */

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
