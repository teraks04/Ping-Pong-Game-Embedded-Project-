#include <avr/io.h>
#include "adc.h"

uint8_t adcReads[4] = {0,0,0,0};

void adcInit(){
    DDRD |= 1<<5; 
    TCCR1A = 1<<6 | 0b11; // non inverting mode, fast PWM
    TCCR1B = 1 | 0b11<<3; // prescaler 1, fast PWM
    OCR1A = 0; //reset counter every iteration
}

void adcRead(){
    volatile char *adc = (volatile char *) 0x1000;
    
    adc[0] = 0;
    for(uint16_t i = 0; i < 100; ++i); //wait a bit  30 too little, 70 ok, 100 safe
    
    uint8_t value = 0;
    for(uint8_t channel = 0; channel <= 3; ++channel){
        adcReads[channel] = adc[0];
    }
}

uint8_t adcGet(uint8_t channel){
    return adcReads[channel];
}