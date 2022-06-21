#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include <math.h>

#include "defines.h"
#include "map.h"
#include "pixbuffer.h"

void draw_blank_column(PixBuffer *pixBuffer, uint16_t x, uint16_t y1, uint16_t y2, uint32_t color);
void draw_textured_column(PixBuffer *pixBuffer, uint16_t x, uint16_t y, uint32_t* texture); // no definition - to be implemented later
void draw_raycast(PixBuffer *pixBuffer, float posX, float posY, float dirX, float dirY, float planeX, float planeY, Map* map);

#endif