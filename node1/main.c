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

int main()
{
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();

    dispInit();

    cancontInit();
    canInit();

    uint8_t i = 0;
    // dispLoadImage(BASE_ADDRESS);
    dispBlackFill();
    
    Menu testMenu;
    menuMake(&testMenu, 5);
    menuAppend(&testMenu, "testJoy", joystickTest);
    menuAppend(&testMenu, "IOtest", ioBoardTest);
    menuAppend(&testMenu, "textTest", testText);
    menuAppend(&testMenu, "testCircle", circleTest);
    menuAppend(&testMenu, "testRAM", SRAM_test);

    Menu mainMenu;
    menuMake(&mainMenu, 5);
    menuAppend(&mainMenu, "M tests", &testMenu);
    
    burningShip();
    while(1)
        menuLayer(&mainMenu);


    return 0;
}
