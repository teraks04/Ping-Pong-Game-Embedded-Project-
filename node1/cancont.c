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

void cancontRequestToSend(char data, char address){
    spiChipSelect(spiCAN);
    spiMasterTransmit(0b00000010); //Ready write
    spiMasterTransmit(address);
    spiMasterTransmit(data);
    spiChipSelect(spiOff);

    return data;
}




