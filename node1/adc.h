#include <stdint.h>



typedef struct {
    uint8_t x;
    uint8_t y;
} pos_t;

void adcInit();
void adcRead();
uint8_t adcGet(uint8_t channel);

void posCalibrate();
pos_t posRead(void);