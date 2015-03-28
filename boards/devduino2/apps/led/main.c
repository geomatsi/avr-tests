#include <util/delay.h>
#include <avr/io.h>

int main (void){
    DDRB = (1 << DDB1);

    while (1){

        PORTB |= (1 << PB1);
        _delay_ms(500);

        PORTB &= ~(1 << PB1);
        _delay_ms(500);
    }

    return 1;
}
