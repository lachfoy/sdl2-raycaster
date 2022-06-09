#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "defines.h"

void draw_pixel(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color);

#endif