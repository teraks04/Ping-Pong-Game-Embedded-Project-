#include "spi.h"

#define TXB0 0
#define TXB1 1
#define TXB2 2
#define TXB_ALL 3

/**
 * @brief ...
 *
 * @param buffer Selects which buffer(s) to request transmission for.
 *               Accepted values: TXB0, TXB1, TXB2, TXB_ALL
 */
void cancontInit();
void reset();
void cancontRequestToSend(uint8_t buffer);
char cancontRead(char address);
void cancontWrite(char data, char address);
char cancontReadStatus();  
void cancontBitModify(char address,char mask,char data);





