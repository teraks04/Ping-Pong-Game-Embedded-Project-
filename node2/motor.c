#include "motor.h"

void motorInit(){
    REG_PMC_PCER1 = 1<<4; //enable PID36/PWM clock
    REG_PWM_WPCR = 0x50574D00 | 0b11111100; //disable write protect on all PWM-related registers

    REG_PWM_ENA = 1; //enable PWM output channel 0, kanal 0 er kobla til PB12
    REG_PWM_CPRD0 = 1024; //PWM period, 1024*1/84*10^6 -> ca. 10khZ
    REG_PWM_CMR0 = 
        0b0        |//0 for master, 1010 for master/1024
        (0b010<<8) |//left aligned (CALG=0), starts at high level
        (0b000<<16); //dead time generator disabled

    REG_PWM_WPCR = 0x50574D00 | 0b11111101; //enble write protect (software) on all PWM-related registers 
    
    REG_PIOB_ABSR |= 1<<13; //peripheral function B on PIOB
    REG_PIOB_PDR = 1<<12; //no standard IO on PB12

    //direction pin
    REG_PIOC_PER = 1 << 23; //enable PIO control of PB23
    REG_PIOC_OER = 1 << 23; //configure as output pin
}

void motorSetSpeed(int speed){
    REG_PWM_CDTY0 = abs(speed); //duty cycle, mellom 0 og CPRD

    if(speed > 0){
        //driv direction pin høy
        REG_PIOC_SODR = 1 << 23;
    } else{
        //driv direction pin lav
        REG_PIOC_CODR = 1 << 23;
    }
}