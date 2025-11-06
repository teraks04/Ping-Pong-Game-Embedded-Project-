#include "sam.h"
#include "quadrature.h"

void quadratureDecodeInit(){
    REG_PMC_PCER0 = 1<<29; //enable clock to counter 2

    REG_TC2_WPMR = 0x54494D<<8 | 0; //disable write protect

    REG_TC2_BMR |=
        0b11<<8 //enable quadrature decode with position measure
        | 0<<11 //disable dir detect
        | 1<<12 //detect edges on both signals
        | 10<<20; //maxfilt
    
    REG_PIOC_PDR = 0b11<<25; //no standard IO on c25/26
    REG_PIOC_ABSR |= 0b11<<25; //peripheral function B


    REG_TC2_WPMR = 0x54494D<<8 | 1; //enable write protect
}