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
    }
}

void ioBoardTest(){
    spiChipSelect(spiIO);
    spiMasterTransmit(0x05);
    for(uint32_t i = 0; i < 10000; ++i);
    spiMasterTransmit(2);
    spiMasterTransmit(1);
    spiChipSelect(spiOff);

    char butts[30];
    consoleLineSet(1, butts);
    while(1){
        ioboardUpdateButtons();
        for(uint8_t i = 0; i<24; ++i){
            butts[i] = (ioboardGetButton(i)==0)? '0': '1';
        }
        butts[24] = '\0';
        graphClear();
        graphConsole();
        dispLoadImage(BASE_ADDRESS);
        for(uint8_t i = 0; i<24; ++i){
            printf("%i", (ioboardGetButton(i)==0)? 0: 1);
        }
        for(uint32_t i = 0; i < 200000; ++i);
    }

    uint8_t i = 0;
    while(0){
        ioboardUpdateButtons();
        printf("\n\r%d %u\n\r", getCheckSum(), ++i);
        for(uint8_t i = 0; i<24; ++i){
            printf("%i", (ioboardGetButton(i)==0)? 0: 1);
        }
        for(uint32_t i = 0; i < 200000; ++i);
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
    }
}

void testMenu(){
    Menu testMenu;
    menuMake(&testMenu, 20);
    menuAppend(&testMenu, "F testJoy", joystickTest);
    menuAppend(&testMenu, "F option2", testText);
    menuAppend(&testMenu, "F option3", testText);
    menuAppend(&testMenu, "F option4", testText);
    menuAppend(&testMenu, "F option5", testText);
    menuAppend(&testMenu, "F testCircle", circleTest);
    uint8_t select = 0;
    printf("%i %i\n\r", testMenu.count, testMenu.maxCount);
    while(1){
        for(uint32_t i = 0; i < 100000; ++i);
        ioboardUpdateButtons();
        if(ioboardGetButton(buttNavUp)) select--;
        if(ioboardGetButton(buttNavDown)) select++;
        if(select > 5) select = 0;
        if(ioboardGetButton(buttNavButt)) menuEnter(&testMenu, select);
        graphClear();
        menuRender(&testMenu, select);
        dispLoadImage(BASE_ADDRESS);
    }
}