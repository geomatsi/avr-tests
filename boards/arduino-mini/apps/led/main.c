#include <util/delay.h>
#include <avr/io.h>

int main (void){
    DDRB = (1 << DDB5);

    while (1){

        PORTB |= (1 << PB5);
        _delay_ms(500);

        PORTB &= ~(1 << PB5);
        _delay_ms(100);
    }

    return 1;
}
