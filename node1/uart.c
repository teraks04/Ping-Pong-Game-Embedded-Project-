#include "uart.h"
#include <avr/io.h>
#include "stdint.h"

uint16_t ubrr = 31;

//From datasheet
void uart_init(){
    // Set baudrate
    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char)ubrr;

    //Enable receiver and transmitter
    UCSRB = (1<<RXEN)|(1<<TXEN);

    //Set frame format: 8data, 2stop bit
    UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);

    //fdevopen(uart_transmit, uart_receive);
}

void uart_transmit(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSRA & (1 << UDRE)));
    // Put data into buffer, sends the data
    UDR = data;
}

unsigned char uart_receive(void) {
    // Wait for data to be received
    while (!(UCSRA & (1 << RXC)));
    // Get and return received data from buffer
    return UDR;
}

bool uart_recieved(void){
    return UCSRA & (1 << RXC);
}