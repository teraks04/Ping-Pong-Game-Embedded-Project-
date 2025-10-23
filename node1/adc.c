#include <avr/io.h>
#include "adc.h"
#include <avr/interrupt.h>
#include "can.h"

uint8_t adcReads[4];

uint8_t adcNeutralX, adcNeutralY;

void adcRead(){
    volatile char *adc = (volatile char *) 0x1000;
    
    adc[0] = 0;
    for(uint16_t i = 0; i < 100; ++i); // 30 too little, 70 ok, 100 safe
    
    uint8_t value = 0;
    for(uint8_t channel = 0; channel <= 3; ++channel){
        adcReads[channel] = adc[0];
    }
}

void adcCalibrate(){
    adcRead();
    adcNeutralX = adcReads[1];
    adcNeutralY = adcReads[0];
    printf("calibrated to: %u, %u\r\n", adcNeutralX, adcNeutralY);
}

void adcInit(){
    //ADC clock
    DDRD |= 1<<5; 
    TCCR1A = 1<<6 | 0b11; // non inverting mode, fast PWM
    TCCR1B = 1 | 0b11<<3; // prescaler 1, fast PWM
    OCR1A = 0; //reset counter every iteration
    adcCalibrate();

    TCCR0 = 0b00001101; //CTC, prescaler 1024
    OCR0 = 97; //5Mhz/1024/97 = 50 Hz
    TIMSK |= 0b01; //enable interrupt on overflow
}

uint8_t adcGet(uint8_t channel){
    return adcReads[channel];
}

uint8_t joyDirectionX(){
    uint8_t xRelative = adcNeutralX-adcReads[1]+(uint8_t)127;
    return xRelative;
}

uint8_t joyDirectionY(){
    uint8_t yRelative = adcReads[0]-adcNeutralY+(uint8_t)127;
    return yRelative;
}

int8_t joyTrinaryX(){
    uint8_t xRelative = joyDirectionX();
    if(xRelative > 127+50) return 1;
    if(xRelative < 127-50) return -1;
    return 0;
}

int8_t joyTrinaryY(){
    uint8_t yRelative = joyDirectionY();
    if(yRelative > 127+50) return 1;
    if(yRelative < 127-50) return -1;
    return 0;
}

ISR(TIMER0_COMP_vect) {
    adcRead();
    canMessage mess;
    mess.id = 1;
    mess.dlc = 2;
    mess.data[0]=joyDirectionX();
    mess.data[1]=joyDirectionY();
    canSend(&mess);
}
