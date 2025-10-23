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
    
    //canSend(&mess);
    while(0){
        while(!canReceived());

        canMessage messr = canReceive();

        uint8_t sig = messr.data[0];
        int sign = sig;
        printf("%i\n\r", sig);
    }
    
    for(int i = 0; i < 10000; ++i);
    
    /*canMessage messr = canReceive();
    uint8_t sig = messr.data[0];
    int sign = sig;
    printf("%i", sig);*/

    return 0;
}
