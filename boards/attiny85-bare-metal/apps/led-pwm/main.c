#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

int main(void)
{
    uint8_t v = 0;

    /* set PB1 as output and turn off LED */

    DDRB = (1 << DDB1);
    PORTB = 0x0;

    /* configure Timer1 as PWM */

    TCCR1 = (1 << PWM1A) | (1 << COM1A1) | (1 << COM1A0) | (1 << CS10) | (1 << CS11) | (0 << CS12) | (0 << CS13);
    TCNT1 = 0x0;
    TIMSK = 0x0;

    OCR1C = 100;

    /* */

    while (1) {

        for (v = 0; v < 100; v++) {
            OCR1A = v;
            _delay_ms(20);
        }

        for (v = 100; v > 0; v--) {
            OCR1A = v;
            _delay_ms(20);
        }

    };

    return 0;
}
