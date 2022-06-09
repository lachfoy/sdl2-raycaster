#include "draw.h"

void draw_pixel(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color)
{
    if (x >= 0 && x < RENDER_WIDTH && y >= 0 && y < RENDER_HEIGHT)
    {
        buffer[RENDER_WIDTH * y + x] = color;
    }
}