#include "adc.h"
#include "sam.h"

void adc_init(){

    REG_ADC_CR = 1; //software reset
    REG_ADC_WPMR = 0x414443 << 8 | 0b0; //disable write protect 
    REG_ADC_CHER = 0b1; //enable channel 0
    REG_ADC_CHDR = ~0 << 1; //disable all other channels
    REG_ADC_MR = 0b0 | // only software triggers
                 0b1 << 4 | //lowres 10 bit resolution
                 0b0 << 5 | //adc er alltid på, ikke i sleep mode
                 0b00000001 << 8 | //prescaler = 1, adclock = 84MHz/4 (1-22Mhz lovlig) 
                 8 << 16 | //startup time, 512 sykler > 168 som er safe
                 0b00 << 20 | //settling time, uviktig
                 0b0011 << 24 | //track time, 180ns er safe, 3+1 sykel tracking time
                 0b00 << 28; //transfer time, 3 sykler
                 //1000 sykler = 0.5 muS
    REG_ADC_WPMR = 0x414443 << 8 | 0b1; //enable write protect 

    REG_PMC_PCER1 = 1<<5; // enable clock to adc
    REG_PIOA_PDR = 1<<2; //no standard IO on A2

    //REG_PIOA_PUDR = 1<<2;//disable pull up
    //REG_PMC_PCER1 = 
    
}

uint16_t adc_read(){
    //reference voltage pin ADVREF???????????
    REG_ADC_CR = 0b10; //start conversion
    while(!(REG_ADC_ISR & 0b1)); //checks end of conversion flag for channel 0
    return REG_ADC_CDR; //reads data in channel 0
}