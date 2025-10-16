#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"

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
    CanInit init;
    init.brp = 5; //clock from 48 to 8 MHz  48/(5+1), 125 ns pr clock =: 1 tidskvanta
    init.propag = 4; //probably fine
    init.phase1 = 5;
    init.phase2 = 5;
    init.sjw = 3; //hoppebredde, feks er vi to tidskvanta off sync tidspunktet, så juster vi med to tidskvanta. Er det over 3 er de noe gærent. 
    init.smp = 0; //

    can_init(init, 0);

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(84000000, 9600);
    printf("Hello World\r\n");



    Pio *piob = PIOB;
    piob->PIO_PER = 1<<13;  //servo sig enable
    piob->PIO_OER = 1<<13; //output enable
    while(1){
        piob->PIO_SODR = 1<<13; //set
        piob->PIO_CODR = 1<<13; //clear
    }


    while (1)
    {
        /* code */
    }
    
}