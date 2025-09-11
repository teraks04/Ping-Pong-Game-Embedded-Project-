#include "stdint.h"

void uart_init();
void uart_transmit(unsigned char data);
unsigned char uart_receive(void);
uint8_t uart_recieved(void);