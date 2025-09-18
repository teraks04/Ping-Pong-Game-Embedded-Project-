#include <avr/io.h>

void adcInit(){
    DDRD |= 1<<5; 
    TCCR1A = 1<<6 | 0b11; // non inverting mode, fast PWM
    TCCR1B = 1 | 0b11<<3; // prescaler 1, fast PWM
    OCR1A = 0; //reset counter every iteration
}