#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
#include "time.h"
#include "adc.h"
#include "quadrature.h"
#include "motor.h"
#include "solenoide.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile.
 * If you get somewhat cryptic errors referencing functions such as _sbrk,
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
// #include "../path_to/uart.h"

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    CanInit init;

    init.brp = 20;   // 84/(21)
    init.propag = 1; // probably fine
    init.phase1 = 2;
    init.phase2 = 2;
    init.sjw = 1; // hoppebredde, feks er vi to tidskvanta off sync tidspunktet, så juster vi med to tidskvanta. Er det over 3 er de noe gærent.
    init.smp = 0; //

    can_init(init, 1);
    uart_init(84000000, 9600);
    printf("uart initialized\r\n");

    servSigEnable();
    adc_init();
    motorInit();
    // quadratureDecodeInit();
    // printf("status:%u\n\r", REG_TC2_SR0);
    solenoideInit();
    solenoideStopShoot();

    quadratureDecodeInit();
    //quadratureDecodeClockTest();
    //encoder_init_plagiarism();
    //encoder_init_plagiarism_mod();
    // printf("Quadrature status:%u\n\r", REG_TC2_SR0);

    int freq = 4;

    uint16_t goalcount = 0;
    uint8_t solenoideCount = 0;
    int accumError;
    int Inum = 2;
    int Iden = 100*freq;
    int Pnum = 50;
    int Pden = 10;
    int posRef = 0; //intervallet er 5600 langt
    int lastPos = 0;
    int countEqual = 0;

    int Snum = 15;
    int Sden = 20*freq;
    int servStat = Snum * 100*freq;
    
    // servDuty(104);

    uint16_t irActiveTime = 0;
    uint64_t loopStart = time_now();

    while (1)
    {
        //printf("%u\n\r", (uint32_t)(time_now()*100/84000000));
        while(time_now()<loopStart);
        loopStart = time_now()+84000000/50/freq;

        // for(int i = 0; i<100000; ++i);
        // printf("%i\n\r", encoderPosition());
        // printf("t\n\r");

        // Servo
        servStat = servStat-servStat*Snum/Sden + (int)getJoyX()*Snum;
        uint16_t servdt = servStat/Sden;
        if (servdt < 35)
            servdt = 35;
        if (servdt > 212)
            servdt = 212;
        servdt = (servdt - 35) * (98 * 2) / 177;
        //printf("%u\n\r", servStat);
        servDuty(servdt);

        // Motor
        int pos = encoderPosition();
        
        if(abs(getJoyY()-128)>10)
            posRef += (-(getJoyY()-128)*200/100);
        int error = posRef/freq - pos;
        accumError += error;
        int input = error*Pnum/Pden + accumError*Inum/Iden;
        motorSetSpeed(input);

        if(pos == lastPos){countEqual++;}
        else countEqual = 0;
        if(countEqual > 4*freq & abs(input) > 300){
            posRef = pos*freq;
            accumError = 0;
            countEqual = 0;
        }
        lastPos = pos;



        // IR sensor
        auto ir = adc_read();
        //printf("%u\n\r", ir);

        if(ir<200 & irActiveTime == 0) {
            printf("Goal:( #%u\n\r", ++goalcount);
            irActiveTime = 1;
        }
        if(ir > 500) irActiveTime = 0;

        // knapper
        for (uint8_t i = 0; i < 24; ++i)
        {
            //printf("%i", (getButton(i) == 0) ? 0 : 1);
        }

        // solenoide
        // if(stat){
        //     stat = 0;
        //     solenoideActive();
        // }
        // else{
        //     stat = 1;
        //     solenoideStopShoot();
        // }

        if (getButton(buttR5) & !solenoideCount)
        {
            solenoideCount = 1;
            solenoideActive();
        } 
        if(solenoideCount){
            solenoideCount++;
        }
        if(solenoideCount > 4*freq){
            solenoideStopShoot();
        }
        if(solenoideCount > 20*freq){
            solenoideCount = 0;
        }

    }

}    
