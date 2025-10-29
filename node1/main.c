#include <avr/io.h>
#include <stdint.h>
#include <string.h>
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
#include "ioInterrupt.h"


int main() {
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();
    dispInit();
    cancontInit();
    canInit();
    ioInterruptInit();

    
    //testText();
    //joystickTest();
    //ioBoardTest();

    char consl1[20];

    consoleLineSet(1, consl1);
    uint8_t ind = 0;

    while(!ioboardGetButton(buttL7)){
        sprintf(consl1, "Hello World%u", ind++);
        graphClear();
        graphConsole();
        dispLoadImage(BASE_ADDRESS);

        for(uint16_t i = 1; i>0; ++i);
    }

    return 0;
}
