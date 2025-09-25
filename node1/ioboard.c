#include "ioboard.h"
#include "spi.h"
uint8_t buttons[3];

enum button {
    buttR0,
    buttR1,
    buttR2,
    buttR3,
    buttR4,
    buttR5,
    RESERVED1,
    RESERVED2,
    buttL0,
    buttL1,
    buttL2,
    buttL3,
    buttL4,
    buttL5,
    buttL6,
    RESERVED3,
    buttNavUp,
    buttNavDown,
    buttNavLeft,
    buttNavRight,
    buttNavButt
};

void ioboardUpdateButtons(){
    spiChipSelect(spiIO);
    spiMasterTransmit(0x04);
    buttons[0] = spiMasterReceive(); //right
    buttons[1] = spiMasterReceive(); //left
    buttons[2] = spiMasterReceive(); //nav
}

uint8_t ioboardGetButton(uint8_t b){
    return buttons[b>>3] & 0b1<<(b&0b111);
}
