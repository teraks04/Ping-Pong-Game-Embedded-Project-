#include "menu.h"
#include "xmem.h"
#include "graphics.h"
#include "string.h"


//a menu is an array of menu-items
//a menu-item is a 14 character string, and a 16-bit pointer to either a menu, or a void(void) function
//the first char is a M for menu, or F for function

const char* MENUS_BASE_ADDRESS = BASE_ADDRESS + 2048;

void menuMake(Menu* dest, uint8_t maxSize){
    dest->count = 0;
    dest->maxCount = maxSize;
    dest->items = MENUS_BASE_ADDRESS;
    MENUS_BASE_ADDRESS += sizeof(MenuItem)*maxSize;
}

void menuAppend(Menu* dest, const char* name, void* itemAddress){
    strcpy(dest->items[dest->count].name, name);
    dest->items[dest->count].destAddress = itemAddress;
    dest->count += 1;
}

void menuRender(Menu* men, uint8_t select){
    uint8_t topLine = select>3? select-3 : 0;
    uint8_t lastLine = men->count;


    graphFillOrthoQuad((vec2){0, (select-topLine)*6}, (vec2){127, (select-topLine)*6+6});

    if(lastLine-topLine > 10) lastLine = topLine + 10;

    /*if(topLine = 0){
        graphText((vec2){4, 0}, "return");
        topLine++;
    }*/

    for(uint8_t line = topLine; line < lastLine; ++line)
        graphText((vec2){4, (line-topLine)*6}, men->items[line].name);
}

Menu* menuEnter(Menu* men, uint8_t select){
    if(men->items[select].name[0]=='M')
        return men->items[select].destAddress;
    void (*func)(void) = men->items[select].destAddress;
    func();
}

void menuLayer(Menu* men){
    return;
}