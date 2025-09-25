#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"
#include "xmem.h"
#include "adc.h"
#include "spi.h"
#include "display.h"
#include "ioboard.h"



int main() {
    //flashingLED();
    
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();

    ioBoardTest();

}
