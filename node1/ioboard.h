#include <stdint.h>

enum button {
    buttR0,
    buttR1,
    buttR2,
    buttR3,
    buttR4,
    buttR5,
    RESERVED1,
    RESERVED2,
    buttL0,
    buttL1,
    buttL2,
    buttL3,
    buttL4,
    buttL5,
    buttL6,
    RESERVED3,
    buttNavUp,
    buttNavDown,
    buttNavLeft,
    buttNavRight,
    buttNavButt
};


void ioboardUpdateButtons();
uint8_t ioboardGetButton(uint8_t b);

uint8_t getCheckSum();