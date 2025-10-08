#include <stdint.h>

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct vec2{
    uint8_t x, y;
} vec2;

//int acc[] = {1, 2, 3};

//int vecadd(int a, int b){return a+b;}

void graphClear();
void graphRebase(uint8_t* bas, uint8_t width, uint8_t height);
void graphTextPage(vec2 loc, const char* str);
void graphText(vec2 loc, const char *str);
void graphReFont(uint8_t *const ft, vec2 dims);
void graphFillOrthoQuad(const vec2 min, const vec2 max);


#endif /* GRAPHICS_H_ */