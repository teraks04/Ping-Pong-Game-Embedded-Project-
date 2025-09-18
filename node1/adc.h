#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} pos_t;

void adcInit();
uint8_t adcRead(uint8_t channel);

void posCalibrate();
pos_t posRead(void);