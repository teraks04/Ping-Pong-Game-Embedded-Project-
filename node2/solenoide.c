#include "solenoide.h"
#include "sam.h"

void solenoideInit()
{
    REG_PIOB_PER = 1 << 25; // enable PIO control of PB25
    REG_PIOB_OER = 1 << 25; // configure as output pin
    // D2 -> PB25
    solenoideStopShoot();

}

void solenoideActive()
{
    REG_PIOB_SODR = 1 << 25;
}

void solenoideStopShoot()
{
    REG_PIOB_CODR = 1 << 25;
}
