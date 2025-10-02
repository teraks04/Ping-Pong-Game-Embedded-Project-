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


void dataModel(void){
    PORTB |= 0b10;
}
void commandModel(void){
    PORTB &= ~0b10;
}


int main() {
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();

    dispInit();
    testText();
}
