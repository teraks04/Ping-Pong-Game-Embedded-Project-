//#define __AVR_AT90USB162__
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

void flashingLED() {
    DDRA |= 1;
    
    while(1){
        PORTA |= 1;
        for(uint32_t i = 0; i < 200000; ++i);
        PORTA &= ~1;
        for(uint32_t i = 0; i < 200000; ++i);
    }
}

void HelloWorld(){
    uint8_t q = 0;
    while(1){
        printf("Hello World!!! %i\r\n", q);
        for(uint32_t i = 0; i < 200000; ++i);
        ++q;
    }
}