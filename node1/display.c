#include "display.h"
#include <avr/io.h>
#include "spi.h"

void dataMode(){
    PORTB |= 0b10;
}
void commandMode(){
    PORTB &= ~0b10;
}

void dispInit(){
    // D/C pin
    DDRB |= 0b10;

    commandMode();

    spiChipSelect(spiDisplay);

    spiMasterTransmit(0xae);
    spiMasterTransmit(0xa1);
    spiMasterTransmit(0xda);
    spiMasterTransmit(0x12);
    spiMasterTransmit(0xc8);
    spiMasterTransmit(0xa8);
    spiMasterTransmit(0x3f);
    spiMasterTransmit(0xd5);
    spiMasterTransmit(0x80);
    spiMasterTransmit(0x81);
    spiMasterTransmit(0x50);
    spiMasterTransmit(0xd9);
    spiMasterTransmit(0x21);
    spiMasterTransmit(0x20);
    spiMasterTransmit(0x02);
    spiMasterTransmit(0xdb);
    spiMasterTransmit(0x30);
    spiMasterTransmit(0xad);
    spiMasterTransmit(0x00);
    spiMasterTransmit(0xa4);
    spiMasterTransmit(0xa6);
    spiMasterTransmit(0xaf);

    spiMasterTransmit(0x81);
    spiMasterTransmit(255);
}

void prepareScreenWrite(){
    spiChipSelect(spiDisplay);
    commandMode();
    spiMasterTransmit(0b00100000);
    spiMasterTransmit(0b00000000);
    spiMasterTransmit(0b00100010); //page start/end
    spiMasterTransmit(0b00000000);
    spiMasterTransmit(0b00000111);
    dataMode();
}
void checkerboardFill(){
    prepareScreenWrite();
    uint8_t wrt = 0b11110000;
    for(int i = 0; i<128*8; ++i){
        spiMasterTransmit(wrt);
        wrt = ~wrt;
    }
}
void blackFill(){
    prepareScreenWrite();
    for(int i = 0; i<128*8; ++i){
        spiMasterTransmit(0);
    }
}

void dispIvert(uint8_t inv){
    spiChipSelect(spiDisplay);
    commandMode();
    if(inv) spiMasterTransmit(0b10100111);
    else spiMasterTransmit(0b10100110);
}

void pageModeAt(uint8_t row, uint8_t col){
    spiChipSelect(spiDisplay);
    commandMode();
    spiMasterTransmit(0b00100000); //page mode
    spiMasterTransmit(0b00000010);
    spiMasterTransmit(0b10110000 | row & 0xF); //row select
    spiMasterTransmit(0b00000000 | col & 0xF); //col lower nibble
    spiMasterTransmit(0b00010000 | (col >> 4)); //col higher nibble
    dataMode();
}

void loadImage(char* image){
    prepareScreenWrite();
    for(uint8_t x = 0; x<128; ++x)
    for(uint8_t y = 0; y<8; ++y)
    spiMasterTransmit(*(image++));
}