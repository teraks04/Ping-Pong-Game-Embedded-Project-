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
}