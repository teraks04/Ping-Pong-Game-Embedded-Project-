//#define __AVR_ATmega162__
#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"

int main() {
    //flashingLED();
    
    uart_init();
    fdevopen(*uart_transmit, *uart_receive);
    printf("Hei");

    while(1){
        uart_transmit('a');
        for(uint32_t i = 0; i < 400000; ++i);
    }
}