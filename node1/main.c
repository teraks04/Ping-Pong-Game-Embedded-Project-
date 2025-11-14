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
#include <string.h>

void sendControls(){
    doIsrCANsend(1);
    uint8_t offs = 0;
    char buf[32];
    resetGoalCount();

    canMessage mess;
    mess.id = 'R';
    mess.dlc = 1;
    canSend(&mess);

    uint16_t lastGoalCount = 0;
    while(1){
        for(uint32_t i = 0; i < 100000; ++i);
        graphClear();
        uint16_t goalCount = getGoalCount();
        if(goalCount != lastGoalCount) graphFillOrthoQuad((vec2){0,0}, (vec2){128,64});
        lastGoalCount = goalCount;
        sprintf(buf, "%u goals", (uint32_t)goalCount);
        graphText((vec2){40, offs++}, buf);
        dispLoadImage(BASE_ADDRESS);
        if(offs > 63) offs = 0;
        if(ioboardGetFlag(buttL7)) break;
    }
    doIsrCANsend(0);
}

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
    menuAppend(&mainMenu, "play game", sendControls);
    menuAppend(&mainMenu, "M tests", &testMenu);
    
    //burningShip();
    sendControls();
    //ioBoardTest();
    while(1)
        menuLayer(&mainMenu);


    return 0;
}
