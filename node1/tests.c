//#define __AVR_AT90USB162__
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#include "spi.h"
#include "ioboard.h"
#include "display.h"
#include "xmem.h"
#include "graphics.h"
#include "menu.h"
#include "adc.h"



void flashingLED() {
    DDRA |= 1;

    while(1){
        PORTA |= 1;
        for(uint32_t i = 0; i < 200000; ++i);
        PORTA &= ~1;
        for(uint32_t i = 0; i < 200000; ++i);
    }
}

void HelloWorld(){
    uint8_t q = 0;
    while(1){
        printf("Hello World!!! %i\r\n", q);
        for(uint32_t i = 0; i < 200000; ++i);
        ++q;
    }
}

#include <stdlib.h>
void SRAM_test(void)
{
    volatile char *ext_ram = (char *) 0x1400; // Start address for the SRAM
    uint16_t ext_ram_size = 0xC00;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\n\r");

    // rand() stores some internal state, so calling this function in a loop 
    // will yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();

    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retrieved_value = ext_ram[i];
        if (retrieved_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", 
                   i, retrieved_value, some_value);
            write_errors++;
        }
    }

    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed); 
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retrieved_value = ext_ram[i];
        if (retrieved_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", 
                   i, retrieved_value, some_value);
            retrieval_errors++;
        }
    }

    printf("SRAM test completed with \n\r%4d errors in write phase and \n\r%4d errors in retrieval phase\n\r", 
           write_errors, retrieval_errors);
}

void joystickTest(){
    adcCalibrate();
    while (1)
    {
        adcRead();
        int8_t val1 = joyTrinaryX();
        int val = val1;
        printf("adc value: %u, %u, %i, %i\r\n", joyDirectionX() & 0xff, joyDirectionY() & 0xff, val, joyTrinaryY() & 0xff);
        spiChipSelect(spiIO);
        spiMasterTransmit(0x03);
        IOstandardDelay();
        printf("spi value: %i, %i, %i\r\n", spiMasterReceive(), spiMasterReceive(), spiMasterReceive());
        spiChipSelect(spiOff);
        
        for(uint32_t i = 0; i < 200000; ++i);
        if(ioboardGetFlag(buttL7)) return;
    }
}

void ioBoardTest(){
    uint8_t i = 0;
    while(1){
        //ioboardUpdateJoy();
        //ioboardUpdateButtons();
        for(uint8_t i = 0; i<32; ++i){
            printf("%i", (ioboardGetFlag(i)==0)? 0: 1);
        }
        printf("   ");
        for(uint8_t i = 0; i<32; ++i){
            printf("%i", (ioboardGetButton(i)==0)? 0: 1);
        }
        printf("\n\r");
        for(uint32_t i = 0; i < 200000; ++i);

        if(ioboardGetFlag(buttL7)) return;
    }
}

void smearTest(){
    dispBlackFill();
    pageModeAt(2, 0);
    for(int i = 0 ; i<1; ++i)
        spiMasterTransmit(0xff);
    spiMasterTransmit(0);
    for(uint32_t i = 0; i < 1000000; ++i);
    for(int i = 0 ; i<126; ++i)
        spiMasterTransmit(0xff);
}

void circleTest(){
    dispBlackFill();
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
    dispLoadImage(BASE_ADDRESS);
}

void testText(){
    uint8_t offs = 0;
    while(1){
        for(uint32_t i = 0; i < 100000; ++i);
        graphClear();
        graphText((vec2){40, offs++}, "Hello World!|_#");
        dispLoadImage(BASE_ADDRESS);
        if(offs > 63) offs = 0;
        if(ioboardGetFlag(buttL7)) return;
    }
}

void testMenu(){
    Menu testMenu;
    menuMake(&testMenu, 4);
    menuAppend(&testMenu, "F testJoy", joystickTest);
    menuAppend(&testMenu, "F IOtest", ioBoardTest);
    menuAppend(&testMenu, "F textTest", testText);
    menuAppend(&testMenu, "F testCircle", circleTest);
    uint8_t select = 0;
    printf("%i %i\n\r", testMenu.count, testMenu.maxCount);
    while(1)
        menuLayer(&testMenu);
}

void burningShip(){
    while(1){
        graphClear();
        for(uint8_t x = 0; x<128; ++x)
        for(uint8_t y = 0; y<64; ++y){
            uint16_t zi = y;
            uint16_t zr = x;
            for(uint8_t i = 0; i<0; ++i){
                uint16_t rtemp = ((uint32_t)zr*(uint32_t)zr >>8)-((uint32_t)zi*(uint32_t)zi >>8) + x;
                zr = (2*(uint32_t)zr*(uint32_t)zi >>8) + y;
            }
            if(((uint32_t)zr*(uint32_t)zr >>8)+((uint32_t)zi*(uint32_t)zi >>8)<4*256)
                graphSet(x, y);
        }

        dispLoadImage(BASE_ADDRESS);
        if(ioboardGetFlag(buttL7)) return;
    }
}