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
    //...

    return message;
}