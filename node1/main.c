#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"
#include "xmem.h"
#include "adc.h"
#include "spi.h"
#include "display.h"
#include "ioboard.h"
#include "graphics.h"
#include "cancont.h"
#include "can.h"
#include "menu.h"


int main() {
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();

    dispInit();

    cancontInit();
    canInit();

    canMessage mess;
    mess.id = 0b00000000100;
    mess.data[0]=51;
    mess.data[1]=0b11110010;
    mess.data[2]=0b11110010;
    mess.data[3]=0b11110010;
    mess.data[4]=0b11110010;
    mess.data[5]=0b11110010;
    mess.data[6]=0b11110010;
    mess.data[7]=0b11110010;
    mess.dlc = 8;
    
    uint8_t i = 0;
    dispLoadImage(BASE_ADDRESS);

    while(1){

        for(uint16_t i = 1; i>0; ++i);
        dispBlackFill();
    }
    
    while(1);

    return 0;
}
