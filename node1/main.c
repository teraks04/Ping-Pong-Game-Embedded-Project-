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
    
    uint8_t val = 0;

    for(uint8_t id = 0; id < 11; ++id){
        mess.id = 0b1<<id;
        canSend(&mess);
        canMessage messrec = canReceive();
        uint16_t buf1 = messrec.id;
        unsigned int buf2 = buf1;
        printf("%u\n\r", buf2);
    }
    return;
    while(1){
        //cancontWrite(MCP_TXB0DATA, 0b10110010);
        //cancontBitModify(MCP_TXB0DATA,0b111,0b111);
        //val = ~val;
        //cancontBitModify(MCP_TXB0CTRL,0b1,val);
        //uint8_t buf = cancontRead(MCP_TXB0CTRL);
        //unsigned int buf2 = buf;
        //printf("%u\n\r", buf);
        canSend(&mess);
        for (uint32_t i = 0; i < 100000; i++);
        //printf("%u\n\r", cancontRead(MCP_TXB0CTRL)&0b1000); //is pending transmission?
        
        canMessage messrec = canReceive();

        printf("%u, %u, %u\n\r", messrec.id&0xFFFF, messrec.data[0] &0xff, messrec.dlc &0xff);
        return;
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
