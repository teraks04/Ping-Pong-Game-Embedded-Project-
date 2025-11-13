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
    
    uint8_t cs = getIsrCANsend();
        canMessage mess;
        mess.id = 1;
        mess.dlc = 5;
        mess.data[0]=joyDirectionX()*cs+128*(1-cs);
        mess.data[1]=joyDirectionY()*cs+128*(1-cs);
        uint8_t* buttpt = ioboardGetButtons();
        mess.data[2]=buttpt[0]*cs;
        mess.data[3]=buttpt[1]*cs;
        mess.data[4]=buttpt[2]*cs;
        canSend(&mess);

    spiChipSelect(selbuf);
}