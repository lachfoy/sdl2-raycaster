#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

#include "defines.h"
#include "map.h"

void draw_pixel(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color);
void draw_raycast(uint32_t *buffer, Map* map,
    double posX, double posY, double dirX, double dirY,
    double planeX, double planeY);

#endif