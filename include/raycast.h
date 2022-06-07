#ifndef RAYCAST_H
#define RAYCAST_H

#include <stdlib.h>
#include <stdint.h>

typedef struct hitInfo
{
    int mapX, mapY, drawStart, drawEnd;
} hitInfo;

hitInfo raycast(int x, int renderW, int renderH, uint8_t map[10][10],
    double posX, double posY, double dirX, double dirY,
    double planeX, double planeY);

#endif