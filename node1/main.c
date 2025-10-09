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

    printf("%i\n\r", sizeof(MenuItem));

    //cancontInit();
    //canInit();

    canMessage mess;
    mess.id = 0b101010101100;
    mess.data[0]=0b11110010;
    mess.dlc = 1;
    while(0){
        canSend(&mess);
        for (uint16_t i = 0; i < 10000; i++);
    }

    
    
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
