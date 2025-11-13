#include "solenoide.h"
void solenoideInit()
{
    REG_PIOC_PER = 1 << 25; // enable PIO control of PB25
    REG_PIOC_OER = 1 << 25; // configure as output pin
    // D2 -> PB25
}

void solenoideActive()
{
    REG_PIOC_SODR = 1 << 23;
}

void solenoideStopShoot()
{
    REG_PIOC_CODR = 1 << 23;
}
