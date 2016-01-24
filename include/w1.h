#ifndef _AVR_W1_H_
#define _AVR_W1_H_

#include <stdbool.h>

/* pin controls */

struct w1_pin {
	void (*pin_high)(void);
	void (*pin_low)(void);
	uint8_t (*pin_value)(void);
};

/* 1-wire */

void w1_set_pin(struct w1_pin *);
bool w1_init_transaction(void);
void w1_send_byte(uint8_t);
uint8_t w1_recv_byte(void);

#endif /* _AVR_W1_H_ */
