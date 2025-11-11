#include <stdint.h>

enum button
{
    buttR1,
    buttR2,
    buttR3,
    buttR4,
    buttR5,
    buttR6,
    buttL1 = 8,
    buttL2,
    buttL3,
    buttL4,
    buttL5,
    buttL6,
    buttL7,
    buttNavButt = 16,
    buttNavRight,
    buttNavDown,
    buttNavLeft,
    buttNavUp,
    buttJoyButt = 24,
    buttJoyRight,
    buttJoyDown,
    buttJoyLeft,
    buttJoyUp,
};

void ioboardUpdateButtons();
uint8_t ioboardGetButton(uint8_t b);
uint8_t ioboardGetFlag(uint8_t b);
uint8_t* ioboardGetButtons();

uint8_t getCheckSum();
void IOstandardDelay();

void ioBoardUpdateJoy();
void ioJoyCalibrate();