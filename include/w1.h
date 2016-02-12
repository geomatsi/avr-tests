#ifndef _AVR_W1_H_
#define _AVR_W1_H_

#include <stdbool.h>

/* 1-wire */
bool w1_init_transaction(void);
void w1_send_byte(uint8_t);
uint8_t w1_recv_byte(void);

#endif /* _AVR_W1_H_ */
