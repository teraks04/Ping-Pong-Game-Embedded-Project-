#include "xmem.h"
#include <avr/io.h>

const uint16_t BASE_ADDRESS = 0x1000;

xmem_init(){
    //Enable external memory interface
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << 5); //release pins 7-4
    //SFIOR |= (1 << 6); //disable bus keeper
    //SFIOR &= ~(1 << 6); //enable bus keeper
}

void xmem_write ( uint8_t data , uint16_t addr ) {
    volatile char * ext_mem = ( char *) BASE_ADDRESS ;
    ext_mem[addr] = data ;
}

uint8_t xmem_read ( uint16_t addr ) {
    volatile char * ext_mem = ( char *) BASE_ADDRESS ;
    uint8_t ret_val = ext_mem [ addr ];
    return ret_val ;
}