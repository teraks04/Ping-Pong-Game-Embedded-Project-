#include "ioboard.h"
#include "spi.h"
uint8_t buttons[3];

void IOstandardDelay(){
    for(uint32_t i = 0; i < 33; ++i);
}

void ioboardUpdateButtons(){
    spiChipSelect(spiIO);
    spiMasterTransmit(0x04);
    IOstandardDelay();
    buttons[0] = spiMasterReceive(); //right
    buttons[1] = spiMasterReceive(); //left
    buttons[2] = spiMasterReceive(); //nav
    spiChipSelect(spiDisplay);
}

uint8_t ioboardGetButton(uint8_t b){
    return buttons[b>>3] & 0b1<<(b&0b111);
}

uint8_t getCheckSum(){
    return buttons[0]+buttons[1]+buttons[2];
}