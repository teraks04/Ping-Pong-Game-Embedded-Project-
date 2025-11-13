#pragma once
#include <stdint.h>

void dispInit();

void dispCheckerboardFill();
void dispBlackFill();
void dispIvert(uint8_t inv);

void pageModeAt(uint8_t row, uint8_t col);