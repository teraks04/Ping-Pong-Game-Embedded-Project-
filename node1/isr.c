#include <avr/interrupt.h>
#include "adc.h"
#include "spi.h"
#include "can.h"

ISR(TIMER0_COMP_vect) {
    uint8_t selbuf = spiGetChipSelect();
    spiChipSelect(spiOff);
    adcRead();
    canMessage mess;
    mess.id = 1;
    mess.dlc = 2;
    mess.data[0]=joyDirectionX();
    mess.data[1]=joyDirectionY();
    canSend(&mess);
    spiChipSelect(selbuf);
}