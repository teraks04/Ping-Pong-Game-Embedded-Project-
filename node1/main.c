#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"
#include "xmem.h"
#include "adc.h"
#include "spi.h"
#include "display.h"
#include "ioboard.h"


void dataModel(void){
    PORTB |= 0b10;
}
void commandModel(void){
    PORTB &= ~0b10;
}


int main() {
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();

    dispInit();

    /*checkerboardFill();
    for(uint8_t pag = 0; pag < 8; ++pag){
        commandModel();
        spiMasterTransmit(0b00100000);
        spiMasterTransmit(0b00000010);
        spiMasterTransmit(0b10110000+pag);
        dataModel();
        for(char i = 0; i<127; ++i){
            spiMasterTransmit(0x00);
        }
    }*/
    blackFill();
    uint8_t *addr = BASE_ADDRESS;
    for(int16_t x = 0; x<128; ++x)
    for(int16_t y = 0; y<64; ++y){
        if((x-64)*(x-64)+(y-32)*(y-32) < 20*20)
        addr[x+(y>>3)*128] |= 1<<(y&0b111);
        else
        addr[x+(y>>3)*128] &= ~(1<<(y&0b111));
    }

    for(int16_t x = 0; x<128; ++x)
    for(int16_t y = 0; y<64; ++y){
        if((x-64)*(x-64)+(y-32)*(y-32) < 15*15)
        addr[x+(y>>3)*128] &= ~(1<<(y&0b111));
    }
    loadImage(BASE_ADDRESS);


    uint8_t stat = 0;
    while(1){
        for(uint32_t i = 0; i < 1000000; ++i);
        //dispIvert(stat);
        stat = !stat;
        spiChipSelect(spiOff);
    }

}
