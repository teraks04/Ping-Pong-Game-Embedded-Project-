
#include <stdint.h>

struct Menu;

typedef struct{
    char name[14];
    union{
        struct Menu *destMenu;
        void (*destFunction)(void);
        void* destAddress;
    };
}MenuItem;

typedef struct Menu{
    uint8_t count;
    uint8_t maxCount;
    MenuItem* items;
}Menu;

void menuMake(Menu* dest, uint8_t maxSize);
void menuAppend(Menu* dest, const char* name, void* itemAddress);
void menuRender(Menu* men, uint8_t select);
Menu* menuEnter(Menu* men, uint8_t select);
void menuLayer(Menu* men);