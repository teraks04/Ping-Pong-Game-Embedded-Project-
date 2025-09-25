#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"
#include "xmem.h"
#include "adc.h"
#include "spi.h"



int main() {
    //flashingLED();
    
    uart_init();
    xmem_init();
    adcInit();
    spiMasterInit();

    spiChipSelect(spiIO);
    while(1){
        spiMasterTransmit(~0b10110001);
        for(uint32_t i = 0; i < 200000; ++i);
    }


}