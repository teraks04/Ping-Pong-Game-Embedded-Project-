
#ifndef SPI_H
#define SPI_H

#include <stdint.h>

enum spiChip {spiIO, spiDisplay, spiCAN, spiOff = 8};

void spiChipSelect(uint8_t p);

void spiMasterInit(void);

void spiMasterTransmit(char cData);

uint8_t spiMasterReceive();

#endif