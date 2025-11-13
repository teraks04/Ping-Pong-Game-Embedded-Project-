#include "sam.h"
#include "quadrature.h"

void quadratureDecodeInit(){
    //PMC->PMC_WPMR = 0x504D42;
    REG_PMC_PCER0 = 1<<29; //enable clock to counter 2
    REG_PMC_PCER1 = 1<<1 | 1<<2; //enable clocks to channel 6 and 7

    REG_PIOC_PDR = 0b11<<25; //no standard IO on c25/26
    REG_PIOC_ABSR |= 0b11<<25; //peripheral function B

    REG_TC2_WPMR = 0x54494D00 | 0; //disable write protect

    REG_TC2_CCR0 = 0b010; //disable clock to channel 0

    REG_TC2_CMR0 = 4; //use system clock
    REG_TC2_CMR1 = 4; //use system clock
    REG_TC2_CMR2 = 4; //use system clock

    REG_TC2_BMR |=
        0b1<<8 //enable quadrature decode
        | 1<<9 //enable position measure
        | 0<<11 //enable dir detect
        | 1<<12 //detect edges on both PHA and PHB
        | 1<<20; //maxfilt
    
    REG_TC2_CCR0 = 0b101; //enable clock to channel 0

    REG_TC2_WPMR = 0x54494D<<8 | 1; //enable write protect
}

void quadratureDecodeClockTest(){
    printf("clocktest\n\r");
    //PMC->PMC_WPMR = 0x504D42;
    //PMC->PMC_PCER0 = 1<<29;
    REG_PMC_WPMR = 0x504D4300 | 1;

    REG_PMC_PCER0 = 1<<29; //enable clock to counter 2
    REG_TC2_WPMR = 0x54494D00 | 0; //disable write protect

    REG_TC2_CCR0 = 0b010; //disable clock to channel 0

    REG_TC2_CMR0 = (4 //use system clock
                    | 1<<15 // waveform mode
    
    );
    REG_TC2_RA0 = 90;
    printf("%u\n\r%u\n\r", REG_TC2_RA0, REG_TC2_CMR0);
    
    REG_TC2_CCR0 = 0b101; //enable clock to channel 0

    //REG_TC2_WPMR = 0x54494D<<8 | 1; //enable write protect
}

int32_t encoderPosition(){
    return REG_TC2_CV0;
}