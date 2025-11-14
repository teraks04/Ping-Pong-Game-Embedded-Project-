#pragma once
#include "cancont.h"
typedef struct {
    uint16_t id; 
    uint8_t dlc; //data length
    uint8_t data[8];
} canMessage;

void canInit();
void canSend(canMessage* message);
canMessage canReceive();
uint8_t canReceived();

void doIsrCANsend(uint8_t state);
uint8_t getIsrCANsend();

uint16_t getGoalCount();
void resetGoalCount();