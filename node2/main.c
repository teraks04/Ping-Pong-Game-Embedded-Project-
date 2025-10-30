#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
#include "time.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

int main()
{
    SystemInit();
    servSigEnable();
    CanInit init;

    init.brp = 20;// 84/(21)
    init.propag = 1; //probably fine
    init.phase1 = 2;
    init.phase2 = 2;
    init.sjw = 1; //hoppebredde, feks er vi to tidskvanta off sync tidspunktet, så juster vi med to tidskvanta. Er det over 3 er de noe gærent. 
    init.smp = 0; //

    can_init(init, 1);

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(84000000, 9600);
    printf("Hello World\r\n");

    CanMsg mess;
    mess.id = 0b01101100100;
    mess.byte[0]=51;
    mess.byte[1]=0b11110010;
    mess.byte[2]=0b11110010;
    mess.byte[3]=0b11110010;
    mess.byte[4]=0b11110010;
    mess.byte[5]=0b11110010;
    mess.byte[6]=0b11110010;
    mess.byte[7]=0b11110010;
    mess.length = 8;

    
    
    uint32_t lowp = 50*100;
    while(1){
        //for(int i = 0; i<100000; ++i);
        time_spinFor(msecs(10));
        //printf("%i, %i\n\r", getJoyX(), getJoyY());

        lowp = lowp*90/100 + ((uint32_t)getJoyX())*10;
        uint16_t servdt = lowp / 100;
        if(servdt < 35) servdt = 35;
        if(servdt > 212) servdt = 212;
        servdt = (servdt - 35)*(98*2)/177;
        servDuty(servdt);

    }


    while (1)
    {
        /* code */
    }
    
}