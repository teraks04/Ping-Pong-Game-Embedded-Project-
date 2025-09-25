#include "spi.h"
#include <avr/io.h>

//DD_SCK er makro for pinnumeret til pinnnen som skal brukes til klokka

void spiChipSelect(uint8_t p){
    uint8_t portmask = 0b11100;
    uint8_t activePort = 0b100<<p;
    PORTB |= portmask^activePort;
    PORTB &= ~(activePort);
}


void spiMasterInit(void){
    /* Set MOSI and SCK output, all others input */
    DDRB = (1<<5)|(1<<6); 

    //Set PB2-PB4 as output we can use for chip select
    DDRB |= (1<<3)|(1<<4)|(1<<5); 

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void spiMasterTransmit(char cData){
    /* Start transmission */
    SPDR = cData;

    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    //SPIF = 7 = end of transmission flag
    //SPSR = status register (bit 7 of this register, is SPIF)
}

uint8_t spiMasterReceive(){
    /* Start transmission */
    SPDR = 0;

    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    uint8_t data = SPDR;

    return data;
}