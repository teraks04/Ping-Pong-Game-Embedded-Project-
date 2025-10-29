
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "ioboard.h"

void ioInterruptInit(){
    TCCR0 = 0b00001101; //CTC, prescaler 1024
    OCR0 = 97; //5Mhz/1024/97 = 50 Hz
    TIMSK |= 0b01; //enable interrupt on overflow
}

ISR(TIMER0_COMP_vect) {
    for(uint8_t i = 0; i<64; ++i);
    printf("i\n\r");
    uint8_t selbuf = spiGetChipSelect();
    spiChipSelect(spiOff);
    adcRead();
    canMessage mess;
    mess.id = 1;
    mess.dlc = 2;
    mess.data[0]=joyDirectionX();
    mess.data[1]=joyDirectionY();
    canSend(&mess);

    ioboardUpdateButtons();

    spiChipSelect(selbuf);
    for(uint8_t i = 0; i<64; ++i);
}
