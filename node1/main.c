#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"
#include "xmem.h"
#include "adc.h"

#define anToJoy(x) ((int8_t)(x-160))

int main() {
    //flashingLED();
    
    uart_init();
    xmem_init();
    adcInit();

    while (1)
    {
        adcRead();
        printf("joystick value: %d, %d\r\n", anToJoy(adcGet(1)), anToJoy(adcGet(0)));
        for(uint32_t i = 0; i < 200000; ++i);
    }
    
    
}