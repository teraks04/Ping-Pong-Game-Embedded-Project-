//#define __AVR_ATmega162__
#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"

int main() {
    uart_init();
    while(1){
        uart_transmit(0b10110111);
        for(uint32_t i = 0; i < 400000; ++i);
    }   
}