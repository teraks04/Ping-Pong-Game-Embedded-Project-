#include "can.h"


void canInit(){
    cancontWrite(0b01000000,0x0F); //Write to CANCTRL register,loopback mode
}

void canSend(canMessage* message){
    //determine a buffer that is free
    

    //sent to this buffer

}
