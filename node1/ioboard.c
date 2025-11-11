#include "ioboard.h"
#include "spi.h"
#include "adc.h"

uint8_t lastbuttons[4] = {0, 0, 0, 0};
uint8_t buttons[4];
uint8_t buttonFlags[4] = {0, 0, 0, 0};
uint8_t joyNeutralX, joyNeutralY;

void IOstandardDelay(){
    for (uint32_t i = 0; i < 33; ++i);
}

void ioboardUpdateButtons(){
    spiChipSelect(spiIO);
    spiMasterTransmit(0x04);
    IOstandardDelay();
    buttons[0] = spiMasterReceive(); // right
    buttons[1] = spiMasterReceive(); // left
    buttons[2] = spiMasterReceive(); // nav
    spiChipSelect(spiDisplay);

    for(uint8_t n = 0; n<4; ++n) buttonFlags[n] |= buttons[n] & (~lastbuttons[n]);

    for(uint8_t n = 0; n<4; ++n) lastbuttons[n] = buttons[n];
}

//this function is a mess. Consider hard-wireing the joystick button
void ioboardUpdateJoy(){    
    spiChipSelect(spiIO);
    spiMasterTransmit(0x03);
    for(int i = 0; i<20; ++i);
    uint8_t joyx = spiMasterReceive()-joyNeutralX+128u;
    uint8_t joyy = spiMasterReceive()-joyNeutralY+128u;
    uint8_t joyb = spiMasterReceive();
    spiChipSelect(spiOff);

    joyx = joyDirectionY();
    joyy = joyDirectionX();

    buttons[3] = joyb&1;
    int joyxx = joyx, joyyy = joyy;
    //printf("%i, %i, %i\n\r", joyxx, joyyy, joyb);
    if (joyx < 78u) buttons[3] |= 1<<3; // left
    if (joyx > 178u) buttons[3] |= 1 << 1; // right
    if (joyy > 178u) buttons[3] |= 1 << 2; // down
    if (joyy < 78u) buttons[3] |= 1 << 4; // up
}
void ioJoyCalibrate(){
    spiChipSelect(spiIO);
    spiMasterTransmit(0x03);
    for(int i = 0; i<25; ++i);
    uint8_t joyx = spiMasterReceive();
    uint8_t joyy = spiMasterReceive();
    uint8_t joyb = spiMasterReceive();
    spiChipSelect(spiOff);
    joyNeutralX = joyx;
    joyNeutralY = joyy;
}

uint8_t ioboardGetButton(uint8_t b){
    return buttons[b >> 3] & 0b1 << (b & 0b111);
}
uint8_t ioboardGetFlag(uint8_t b){
    uint8_t flag = buttonFlags[b >> 3] & 0b1 << (b & 0b111);
    buttonFlags[b >> 3] &= ~(1 << (b & 0b111)); //reset the flag on get;
    return flag;
}

uint8_t getCheckSum(){
    return buttons[0] + buttons[1] + buttons[2];
}

uint8_t *ioboardGetButtons(){
    return buttons;
}