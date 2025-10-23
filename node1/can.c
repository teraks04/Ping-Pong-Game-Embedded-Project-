#include "can.h"
#include "avr/io.h"

void canInit(){
    //cancontWrite(MCP_CANCTRL, 0b01000000); //loopback mode
    //cancontWrite(MCP_CANCTRL, 0b00001000); //normal mode, one-shot
    //cancontWrite(MCP_CANCTRL, 0b00000100); //normal mode, multi-shot, clock output
    cancontWrite(MCP_CANCTRL, 0b00000000); //normal mode, multi-shot

    //interupt pin
    DDRD &= ~(1 << PD3); 
    PORTD |= (1 << PD3);
    
    MCUCR &= ~(1 << ISC10);
    MCUCR |= (1 << ISC11);

    GICR |= (1 << INT1);
}

void canSend(canMessage* message){
    char bf = 0; //bufferoffset
    //determine a buffer that is free
    char ctrl =  cancontRead(MCP_TXB0CTRL);
    if(!(ctrl & (1 << 4))){ //is TXREQ low?
        bf = 0;
    }
    ctrl =  cancontRead(MCP_TXB1CTRL);
    if(!(ctrl & (1 << 4))){
        bf = 16;
    }
    ctrl =  cancontRead(MCP_TXB2CTRL);
    if(!(ctrl & (1 << 4))){
        bf = 32;
    }

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
    cancontRequestToSend(TXB0+(bf>>4));
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
    uint16_t sidl = (uint8_t)cancontRead(MCP_RXB0SIDL+bf);
    uint16_t sidh = (uint8_t)cancontRead(MCP_RXB0SIDH+bf);
    //sidl &= 0xff;
    //sidh &= 0xff;

    //uint8_t test = (sidl&0xff)>>5;
    //unsigned int test1 = test;

    //printf("%u, %u\n\r", sidh&0xff, (sidl>>5)&0xff);

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

uint8_t canReceived(){
    uint8_t cainintf = cancontRead(MCP_CANINTF);
    return cainintf & 0b11;
}