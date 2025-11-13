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

    //quadratureDecodeInit();
    quadratureDecodeClockTest();
    printf("Quadrature status:%u\n\r", REG_TC2_SR0);

    uint16_t goalcount = 0;
    uint32_t lowp = 50 * 100;
    uint8_t solenoideCount = 0;
    // servDuty(104);

    uint8_t stat = 0;
    uint16_t irActiveTime = 0;
    uint16_t irInactiveTime = 0;
    while (1)
    {

        // for(int i = 0; i<100000; ++i);
        time_spinFor(msecs(10));
        //printf("%u %u %u\n\r", REG_TC2_CV0, REG_TC2_CV1, REG_TC2_CV2);
        // printf("t\n\r");

        // Servo
        lowp = lowp * 75 / 100 + ((uint32_t)getJoyX()) * 25;
        uint16_t servdt = lowp / 100;
        if (servdt < 35)
            servdt = 35;
        if (servdt > 212)
            servdt = 212;
        servdt = (servdt - 35) * (98 * 2) / 177;
        //printf("%u\n\r", servdt);
        servDuty(servdt);

        // Motor
        motorSetSpeed(-(getJoyY()-128)*1024/200);

        // IR sensor
        uint8_t ir = adc_read() < 400 ? 1 : 0;
        if (ir){
            irActiveTime++;
            irInactiveTime = 0;
        }
        else{
            irInactiveTime++;
        }
        if(irInactiveTime > 4) irActiveTime = 0;
        if(irActiveTime == 3){

            printf("Goal:( #%u\n\r", ++goalcount);

            CanMsg ms;
            ms.id = 0;
            ms.length = 1;
            ms.byte[0] = goalcount;
            // can_tx(/* ClockTestcode */ms);
        }

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
        if(solenoideCount > 4){
            solenoideStopShoot();
        }
        if(solenoideCount > 20){
            solenoideCount = 0;
        }

    }

}    
