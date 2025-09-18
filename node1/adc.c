#include <avr/io.h>
#include "adc.h"

void adcInit(){
    DDRD |= 1<<5; 
    TCCR1A = 1<<6 | 0b11; // non inverting mode, fast PWM
    TCCR1B = 1 | 0b11<<3; // prescaler 1, fast PWM
    OCR1A = 0; //reset counter every iteration
}

uint8_t adcRead(uint8_t channel){
    char *adc = (char *) 0x1000;
    
    adc[0] = 0;
    //pause
    
    uint8_t value;
    while(channel >= 0 && channel <= 3){
        value = adc[0];
        channel--;
    }

    return value;
}