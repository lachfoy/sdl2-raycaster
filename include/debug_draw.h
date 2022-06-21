#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <SDL2/SDL.h>
#include <math.h>

#include "map.h"

#define DEBUG_DRAW_SIZE 42

void debug_draw_map(SDL_Renderer* renderer, Map* map);
void debug_draw_object(SDL_Renderer* renderer, float posX, float posY, float dirX, float dirY);
void debug_draw_ray(SDL_Renderer* renderer, float x, float y, float dirX, float dirY, Map* map);

void debug_draw_map(SDL_Renderer* renderer, Map* map)
{
    for (uint8_t x = 0; x < map->width; x++)
        for (uint8_t y = 0; y < map->height; y++)
        if (map->data[y * map->width + x] > 0)  // ALWAYS [y * w + x] for consistency
        {
            SDL_Rect debug_tile = { x * DEBUG_DRAW_SIZE, y * DEBUG_DRAW_SIZE, DEBUG_DRAW_SIZE, DEBUG_DRAW_SIZE };
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xAA, 0xFF);
            SDL_RenderDrawRect(renderer, &debug_tile);
        }
}

void debug_draw_object(SDL_Renderer* renderer, float x, float y, float dirX, float dirY)
{
    SDL_Rect debug_tile = { (x * DEBUG_DRAW_SIZE) - (DEBUG_DRAW_SIZE / 4), (y * DEBUG_DRAW_SIZE) - (DEBUG_DRAW_SIZE / 4), DEBUG_DRAW_SIZE / 2, DEBUG_DRAW_SIZE / 2};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xAA, 0xFF);
    SDL_RenderDrawRect(renderer, &debug_tile);

    SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xAA, 0xFF);
    SDL_RenderDrawLine(renderer, x * DEBUG_DRAW_SIZE, y * DEBUG_DRAW_SIZE, (x + dirX) * DEBUG_DRAW_SIZE, (y + dirY) * DEBUG_DRAW_SIZE);
}

void debug_draw_ray(SDL_Renderer* renderer, float x, float y, float dirX, float dirY, Map* map)
{
    // drawing a single ray from the player
    // javidx9 - Super Fast Ray Casting in Tiled Worlds using DDA https://youtu.be/NbSee-XM7WA
    float rayStartX = x;
    float rayStartY = y;

    float rayDirX = dirX;
    float rayDirY = dirY;

    // calculate the unit step size
    float rayUnitStepSizeX = sqrtf(1 + (rayDirY / rayDirX) * (rayDirY / rayDirX));
    float rayUnitStepSizeY = sqrtf(1 + (rayDirX / rayDirY) * (rayDirX / rayDirY));

    int mapCheckX = (int)(rayStartX); // truncate position x to integer
    int mapCheckY = (int)(rayStartY); // truncate position y to integer

    float rayLength1DX;
    float rayLength1DY;

    // what direction to step in x or y-direction (either +1 or -1)
    int stepX; 
    int stepY;

    // calculate step direction and starting ray length
    // using this information we can decide how to step through the array - i.e. based on which length is shorter
    if (rayDirX < 0) // if the ray is moving towards the left
    {
        stepX = -1;
        rayLength1DX = (rayStartX - (float)(mapCheckX)) * rayUnitStepSizeX;
    } 
    else
    {
        stepX = 1;
        rayLength1DX = ((float)(mapCheckX + 1) - rayStartX) * rayUnitStepSizeX;
    }
    if (rayDirY < 0) // if the ray is moving towards the top
    {
        stepY = -1;
        rayLength1DY = (rayStartY - (float)(mapCheckY)) * rayUnitStepSizeY;
    } 
    else
    {
        stepY = 1;
        rayLength1DY = ((float)(mapCheckY + 1) - rayStartY) * rayUnitStepSizeY;
    }

    int hit = 0;
    float maxDistance = 10.0f;
    float distance = 0.0f;
    // dda
    while(hit == 0 && (distance < maxDistance))
    {
        if (rayLength1DX < rayLength1DY) // if the length in the x axis is shorter than the length in the y axis, step in the x direction
        {
            mapCheckX += stepX;
            distance = rayLength1DX;
            rayLength1DX += rayUnitStepSizeX;
        }
        else // otherwise step in the x direction
        {
            mapCheckY += stepY;
            distance = rayLength1DY;
            rayLength1DY += rayUnitStepSizeY;
        }

        if (map->data[mapCheckY * map->width + mapCheckX] > 0)  // if we hit a solid tile
        {
            hit = 1;
        }
    }

    float intersectionX = rayStartX + (rayDirX * distance);
    float intersectionY = rayStartY + (rayDirY * distance);

    // finally draw a line between the start position and the intersection position
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawLineF(renderer,
        rayStartX * DEBUG_DRAW_SIZE, rayStartY * DEBUG_DRAW_SIZE,
        intersectionX * DEBUG_DRAW_SIZE, intersectionY * DEBUG_DRAW_SIZE);

}

#endif