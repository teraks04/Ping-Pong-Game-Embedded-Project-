#include "graphics.h"
#include "fonts.h"


uint8_t* BASE_ADDRESS = 0x1400;
uint8_t SCREEN_WIDTH = 128;
uint8_t SCREEN_HEIGHT = 8;

void graphRebase(uint8_t* bas, uint8_t width, uint8_t height){
    BASE_ADDRESS = bas;
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
}

void graphClear(){
    uint8_t* pt = BASE_ADDRESS;
    for(uint8_t x = 0; x<SCREEN_WIDTH; ++x)
    for(uint8_t y = 0; y<SCREEN_HEIGHT; ++y)
        *(pt++) = 0;
}

vec2 FONT_SCALE = {4, 6};
const uint8_t *FONT = font4;

void graphReFont(uint8_t *const ft, vec2 dims){
    FONT = &font8;
    FONT_SCALE = dims;
}

void graphTextPage(vec2 loc, const char * str){
    uint8_t* adr = BASE_ADDRESS + loc.x + loc.y*SCREEN_WIDTH;
    while(*str != '\0'){
        for(uint8_t colm = 0; colm < FONT_SCALE.x; ++colm)
            *(adr++) = pgm_read_byte(FONT +FONT_SCALE.x*(*str - 32)+colm);
        ++str;
    }
}

void graphText(vec2 loc, const char *str){
    uint8_t* adr = BASE_ADDRESS + loc.x + (loc.y>>3)*SCREEN_WIDTH;

    uint8_t offs = loc.y&0b111;
    uint16_t mask = 0x00FF >> 8-FONT_SCALE.y << offs;
    uint8_t lowermask = mask >> 8;
    uint8_t uppermask = mask & 0xFF;
    uint8_t constoffs = 8-FONT_SCALE.y;
    uint8_t upperoffs = offs;
    uint8_t loweroffs = 8-upperoffs;
    

    while(*str != '\0'){
        for(int colm = 0; colm < FONT_SCALE.x; ++colm){
            uint8_t col = pgm_read_byte(FONT +FONT_SCALE.x*(*str - 32)+colm);
            adr[colm] ^= uppermask & ((col >> constoffs) << upperoffs);
            adr[colm + SCREEN_WIDTH] ^= lowermask & ((col >> constoffs) >> loweroffs);
        }
        adr += FONT_SCALE.x;
        ++str;
    }
}

void graphFillOrthoQuad(const vec2 min, const vec2 max){
    uint8_t* pageadr = BASE_ADDRESS + (min.y>>3)*SCREEN_WIDTH;
    uint8_t pages = (max.y>>3) - (min.y>>3) + 1;

    //first page, special case if only page
    uint8_t topMask = 0xff << (min.y&0b111);
    uint8_t bottomMask = 0xff >> (8-(max.y&0b111));
    if(pages == 1){
        topMask &= bottomMask;
        for(uint8_t x = min.x; x < max.x; ++x)
            pageadr[x] |= topMask;
        return;
    }

    for(uint8_t x = min.x; x < max.x; ++x)
        pageadr[x] |= topMask;

    //middle pages
    while(pages > 2){
        --pages;
        pageadr += SCREEN_WIDTH;
        for(uint8_t x = min.x; x < max.x; ++x)
            pageadr[x] = 0xff;
    }

    //bottom page
    pageadr += SCREEN_WIDTH;
    for(uint8_t x = min.x; x < max.x; ++x)
        pageadr[x] |= bottomMask;
    
}

void graphSet(uint8_t x, uint8_t y){
    BASE_ADDRESS[(y>>3)*128+x] |= 1<<(y&0b111);
}
void graphReset(uint8_t x, uint8_t y){
    BASE_ADDRESS[(y>>3)*128+x] &= ~(1<<(y&0b111));
}