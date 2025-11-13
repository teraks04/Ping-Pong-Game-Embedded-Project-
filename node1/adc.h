#pragma once
#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} pos_t;

void adcInit();
void adcRead();
uint8_t adcGet(uint8_t channel);

void adcCalibrate();
uint8_t joyDirectionX();
uint8_t joyDirectionY();
int8_t joyTrinaryX();
int8_t joyTrinaryY();