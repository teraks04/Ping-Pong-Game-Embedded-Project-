#include "cancont.h"

void reset(){
    spiChipSelect(spiCAN);
    spiMasterTransmit(0b11000000);
    spiChipSelect(spiOff);
}

void cancontInit(){
    reset();
}

char cancontRead(char address){
    spiChipSelect(spiCAN);
    spiMasterTransmit(0b00000011); //Ready read
    spiMasterTransmit(address);
    char data = spiMasterReceive();
    spiChipSelect(spiOff);

    return data;
}

void cancontWrite(char data, char address){
    spiChipSelect(spiCAN);
    spiMasterTransmit(0b00000010); //Ready write
    spiMasterTransmit(address);
    spiMasterTransmit(data);
    spiChipSelect(spiOff);

    return data;
}

void cancontRequestToSend(uint8_t buffer)
{
    uint8_t cmd = 0x80; // Base RTS opcode (1000 0000)

    switch(buffer) {
        case TXB0:  cmd |= 0x01; break; // 1000 0001
        case TXB1:  cmd |= 0x02; break; // 1000 0010
        case TXB2:  cmd |= 0x04; break; // 1000 0100
        case TXB_ALL: cmd |= 0x07; break; // 1000 0111
        default: return;
    }

    spiChipSelect(spiCAN);
    spiMasterTransmit(cmd);
    spiChipSelect(spiOff);
}

char cancontReadStatus(){
    spiChipSelect(spiCAN);
    spiMasterTransmit(0b10100000);
    char statusData = spiMasterReceive();
    spiChipSelect(spiOff);
    return statusData;
}

void cancontBitModify(char address,char mask,char data){
    spiChipSelect(spiCAN);
    spiMasterTransmit(0b00000101);
    spiMasterTransmit(address);
    spiMasterTransmit(mask);
    spiMasterTransmit(data);
    spiChipSelect(spiOff);
}





