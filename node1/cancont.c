#include "cancont.h"

void reset(){
    spiChipSelect(spiCAN);
    spiMasterTransmit(0b11000000);
    spiChipSelect(spiOff);
}

void cancontInit(){
    reset();
    //set in config mode
    // init.brp = 5; //clock from 48 to 8 MHz  48/(5+1), 125 ns pr clock =: 1 tidskvanta
    // init.propag = 4; //probably fine
    // init.phase1 = 5;
    // init.phase2 = 5;
    // init.sjw = 3; //hoppebredde, feks er vi to tidskvanta off sync tidspunktet, så juster vi med to tidskvanta. Er det over 3 er de noe gærent. 
    // init.smp = 0; //
    cancontWrite(MCP_CANCTRL, 0b10000000); //config mode
    //0; //TQ = 2 x (BRP + 1)/FOSC
    uint8_t brp = 1; //(brp+1)/16MHz = 0.25 µs. *(3+5+5)=3.25µs
    uint8_t sjw = 1 << 6;
    cancontWrite(MCP_CNF1,brp | sjw);
    uint8_t propag = 1;
    uint8_t phase1 = 2 << 3;
    uint8_t smp = 0 << 6;
    uint8_t btlmode = 1 << 7;
    cancontWrite(MCP_CNF2,propag | phase1 | btlmode | smp);
    uint8_t phase2 = 2;
    cancontWrite(MCP_CNF3, phase2);
}

char cancontRead(char address){
    spiChipSelect(spiCAN);
    spiMasterTransmit(0b00000011); //Ready read
    spiMasterTransmit(address);
    char data = spiMasterReceive();
    spiChipSelect(spiOff);

    return data;
}

void cancontWrite(char address,char data){
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





