#ifndef RAYCAST_H
#define RAYCAST_H

#include <stdlib.h>
#include <stdint.h>

#include "defines.h"

typedef struct
{
    int mapX, mapY, drawStart, drawEnd;
} hitInfo;

hitInfo raycast(int x, uint8_t map[10][10],
    double posX, double posY, double dirX, double dirY,
    double planeX, double planeY);

#endif