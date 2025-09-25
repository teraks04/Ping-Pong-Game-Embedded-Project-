#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"
#include "xmem.h"
#include "adc.h"
#include "spi.h"
#include "display.h"
#include "ioboard.h"

void dataMode1(){
    PORTB |= 0b10;
}
void commandMode1(){
    PORTB &= ~0b10;
}

int main() {
    //flashingLED();
    
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();

    spiChipSelect(spiDisplay);
    dispInit();
    //spiMasterTransmit(0b10100101);
    //spiMasterTransmit(0b10100111); //inverse mode

    /*for(uint8_t pag = 0; pag < 8; ++pag){
        commandMode1();
        spiMasterTransmit(0b00100000);
        spiMasterTransmit(0b00000010);
        spiMasterTransmit(0b10110000+pag);
        dataMode1();
        for(char i = 0; i<127; ++i){
            spiMasterTransmit(0x00);
        }
    }*/

    commandMode1();
    spiMasterTransmit(0b00100000);
    spiMasterTransmit(0b00000000);
    dataMode1();
    uint8_t wrt = 0b10101010;
    for(int i = 0; i<128*8; ++i){
        spiMasterTransmit(wrt);
        wrt = ~wrt;
    }

    while(1){
        for(uint32_t i = 0; i < 200000; ++i);
    }

}
