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

    spiChipSelect(spiDisplay);
    commandMode();

    spiMasterTransmit(0xae); //disp off
    spiMasterTransmit(0x81); //contrast control
    spiMasterTransmit(255);
    spiMasterTransmit(0xa4); //disp from ram
    spiMasterTransmit(0xa6); //normal display

    spiMasterTransmit(0xa1); //zero at zero
    spiMasterTransmit(0xc8); //com scan direction left-right
    spiMasterTransmit(0xa8); //multiplex ratio 63
    spiMasterTransmit(63);
    spiMasterTransmit(0xaf); //disp on
}

void prepareScreenWrite(){
    spiChipSelect(spiDisplay);
    commandMode();
    spiMasterTransmit(0b00100000); //horizontal adressal mode
    spiMasterTransmit(0b00000000);
    spiMasterTransmit(0b00100010); //page start/end
    spiMasterTransmit(0);
    spiMasterTransmit(7);
    spiMasterTransmit(0b00100001); //column start/end
    spiMasterTransmit(0);
    spiMasterTransmit(127);
    dataMode();
}
void dispCheckerboardFill(){
    prepareScreenWrite();
    uint8_t wrt = 0b11110000;
    for(int i = 0; i<128*8; ++i){
        spiMasterTransmit(wrt);
        wrt = ~wrt;
    }
}
void dispBlackFill(){
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

void dispLoadImage(char* image){
    prepareScreenWrite();
    //for(uint8_t x = 0; x<128; ++x)
    //for(uint8_t y = 0; y<8; ++y)
    for(uint16_t prog = 0; prog<128*8; ++prog)
    spiMasterTransmit(*(image++));
}