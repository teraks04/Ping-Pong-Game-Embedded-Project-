#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "tests.h"
#include "xmem.h"

int main() {
    //flashingLED();
    
    uart_init();
    xmem_init();
    SRAM_test();
    
}