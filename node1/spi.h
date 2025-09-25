#include <stdint.h>

enum spiChip {spiIO, spiDisplay, spiCAN}

void spiChipSelect(uint8_t p);

void spiMasterInit(void);

void spiMasterTransmit(char cData);

uint8_t spiMasterReceive();