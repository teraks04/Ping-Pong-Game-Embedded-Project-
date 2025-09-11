#include "uart.h"
#include <avr/io.h>

//baudrate 9600 for 8MHz
uint16_t ubrr = 31;

//From datasheet
void uart_init(){
    // Set baudrate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    //Enable receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    //Set frame format: 8data, 2stop bit
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

    fdevopen(*uart_transmit, *uart_receive);
}

void uart_transmit(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    // Put data into buffer, sends the data
    UDR0 = data;
}

unsigned char uart_receive(void) {
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));
    // Get and return received data from buffer
    return UDR0;
}

uint8_t uart_recieved(void){
    return UCSR0A & (1 << RXC0);
}