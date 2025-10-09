#include "cancont.h"
typedef struct {
    uint16_t id; 
    uint8_t dlc; //data length
    uint8_t data[8];
} canMessage;

void canInit();