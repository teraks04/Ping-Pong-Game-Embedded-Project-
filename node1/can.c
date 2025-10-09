#include "can.h"
void canInit(){
    cancontWrite(MCP_CANCTRL, 0b01000000); //loopback mode
    //cancontWrite(MCP_CANCTRL, 0b00001000); //normal mode, one-shot
    //cancontWrite(MCP_CANCTRL, 0b00000100); //normal mode, multi-shot, clock output
}

void canSend(canMessage* message){
    char bf = 0; //bufferoffset
    //determine a buffer that is free
    char ctrl =  cancontRead(MCP_TXB0CTRL);
    if(!(ctrl & (1 << 3))){ //is TXREQ low?
        bf = 0;
    }
    ctrl =  cancontRead(MCP_TXB1CTRL);
    if(!(ctrl & (1 << 3))){
        bf = 16;
    }
    ctrl =  cancontRead(MCP_TXB2CTRL);
    if(!(ctrl & (1 << 3))){
        bf = 32;
    }
    bf = 0;

    //write to this buffer
    //data
    for (int idx = 0; idx < message->dlc; idx++){
        cancontWrite(MCP_TXB0DATA+bf+idx,message->data[idx]); 
    }
    //id
    uint8_t id_high = (message->id >> 3) & 0xFF;
    uint8_t id_low  = (message->id & 0b111);  

    cancontWrite(MCP_TXB0SIDL+bf,id_low << 5);
    cancontWrite(MCP_TXB0SIDH+bf,id_high);
    //datalength
    cancontWrite(MCP_TXB0DLC+bf,message->dlc);  //maybe set RTR
    
    //request to send 
    cancontRequestToSend(TXB0);
    //cancontBitModify(MCP_TXB0CTRL, 0b100, 0b100);
}

canMessage canReceive(){
    canMessage message;
    message.dlc = 8;
    for (uint8_t idx = 0; idx < message.dlc; idx++){
        message.data[idx] = 0;
    }
    message.id = 0;

    //Which buffer has received
    uint8_t bf;
    uint8_t cainintf = cancontRead(MCP_CANINTF);
    if (cainintf & (1 << 0)){
        bf = 0;
    } 
    else if (cainintf & (1 << 1)){
        bf = 16;
    } 
    else{
        //default
        return message;
    }

    // Read ID
    uint16_t sidl = cancontRead(MCP_RXB0SIDL+bf);
    uint16_t sidh = cancontRead(MCP_RXB0SIDH+bf);

    message.id = (sidh << 3) | (sidl >> 5);
    // Read DLC
    message.dlc = 0b00001111 & cancontRead(MCP_RXB0DLC+bf);
    // Read data
    for (uint8_t idx = 0; idx < message.dlc; idx++){
        message.data[idx] = cancontRead(MCP_RXB0DATA + bf + idx);
    }

    // Clear interrupt flag
    cancontBitModify(MCP_CANINTF, (1 << bf/16), 0);

    return message;
}