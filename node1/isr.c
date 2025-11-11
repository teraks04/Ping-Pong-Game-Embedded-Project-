#include <avr/interrupt.h>
#include "adc.h"
#include "spi.h"
#include "can.h"
#include "ioboard.h"

ISR(TIMER0_COMP_vect) {
    uint8_t selbuf = spiGetChipSelect();
    spiChipSelect(spiOff);
    adcRead();
    ioboardUpdateJoy();
    ioboardUpdateButtons();
    canMessage mess;
    mess.id = 1;
    mess.dlc = 5;
    mess.data[0]=joyDirectionX();
    mess.data[1]=joyDirectionY();
    uint8_t* buttpt = ioboardGetButtons();
    mess.data[2]=buttpt[0];
    mess.data[3]=buttpt[1];
    mess.data[4]=buttpt[2];
    canSend(&mess);
    spiChipSelect(selbuf);
}