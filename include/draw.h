#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include <math.h>

#include "defines.h"
#include "map.h"

void draw_pixel(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color);
void draw_raycast(uint32_t *buffer, Map* map, uint32_t *texture,
    float posX, float posY, float dirX, float dirY,
    float planeX, float planeY);

#endif