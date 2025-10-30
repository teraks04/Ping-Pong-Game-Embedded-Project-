#include "pwm.h"
#include "sam.h"

void servSigEnable(){
    REG_PMC_PCER1 = 1<<4; //enable PID36/PWM clock

    REG_PWM_WPCR = 0x50574D00 | 0b11111100; //disable write protect on all PWM-related registers

    //REG_PWM_CLK = (0b0000<<8) & (1u<<8); //PWM clock at system clock (?) for CLKA. Also set PREA
    REG_PWM_CMR1 = 
        0b1010-1 |//0b1011 for CLKA, 0 for master, 1010 master/1024
        (0b010<<8) |//left aligned (CALG=0), starts at high level
        (0b000<<16); //dead time generator disabled
    REG_PWM_CPRD1 = 1640*2; //PWM period, modify with PWM_CPRDUPD1
    REG_PWM_CDTY1 = 100*2; //duty cycle, modify with PWM_CDTYUPD1
    //REG_WPM_DT0 //dead times
    REG_PWM_SCM = 0; //no syncronous channels
    REG_PWM_CMPV1 = 0; //comparison disabled
    //REG_PWM_CMPV1 //comparison value
    //REG_PWM_ELMR1 //event lines
    //REG_PWM_FMR //fault input polarity...
    //interrupts...
    REG_PWM_ENA = 1<<1; //enable PWM output channel 1

    REG_PWM_WPCR = 0x50574D00 | 0b11111101; //enble write protect (software) on all PWM-related registers

    REG_PIOB_PDR = 1<<13; //no standard IO on b13
    REG_PIOB_ABSR |= 1<<13; //peripheral function B
}

//min 0, max 98
void servDuty(uint8_t dt){
    if(dt > 98*2) dt = 98*2;
    REG_PWM_CDTYUPD1 = dt+74*2;
}