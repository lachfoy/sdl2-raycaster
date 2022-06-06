#include "util.h"

uint32_t ARGB(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha)
{
	return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

uint32_t FastRand(void) 
{   
    uint32_t t;

    s_randX ^= s_randX << 16;
    s_randX ^= s_randX >> 5;
    s_randX ^= s_randX << 1;

    t = s_randX;
    s_randX = s_randY;
    s_randY = s_randZ;
    s_randZ = t ^ s_randX ^ s_randY;

    return s_randZ;
}