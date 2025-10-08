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
#include "menu.h"


int main() {
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();

    dispInit();

    printf("%i\n\r", sizeof(MenuItem));

    testMenu();

    //printf("hi\n\r");

    cancontInit();
    #define TXB0D0 0b00110110
    while(1){
        spiChipSelect(spiCAN);
        spiMasterTransmit(0b00000011); //Ready read
        spiMasterTransmit(TXB0D0);
        char data = spiMasterReceive();
        spiChipSelect(spiOff);
        for (uint16_t i = 0; i < 10000; i++);
    }
    

    //#define TXB0D0 0b00110110
    //cancontInit();
    //cancontWrite(20,TXB0D0);
    //char data = cancontRead(TXB0D0);

    //printf("%i\n\r",data);
}
