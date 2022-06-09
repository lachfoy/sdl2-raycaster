#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <SDL2/SDL.h>

#include "map.h"

#define DEBUG_DRAW_SIZE 32

void debug_draw_map(SDL_Renderer* renderer, Map* map);
void debug_draw_object(SDL_Renderer* renderer, double posX, double posY);

void debug_draw_map(SDL_Renderer* renderer, Map* map)
{
    for (int x = 0; x < map->width; x++)
        for (int y = 0; y < map->height; y++)
        if (map->data[map->width * y + x] > 0)
        {
            SDL_Rect debug_tile = { x * DEBUG_DRAW_SIZE, y * DEBUG_DRAW_SIZE, DEBUG_DRAW_SIZE, DEBUG_DRAW_SIZE };
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xAA, 0xFF);
            SDL_RenderDrawRect(renderer, &debug_tile);
        }
}

void debug_draw_object(SDL_Renderer* renderer, double x, double y)
{
    SDL_Rect debug_tile = { (x * DEBUG_DRAW_SIZE) + (DEBUG_DRAW_SIZE / 4), (y * DEBUG_DRAW_SIZE) + (DEBUG_DRAW_SIZE / 4), DEBUG_DRAW_SIZE / 2, DEBUG_DRAW_SIZE / 2};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xAA, 0xFF);
    SDL_RenderDrawRect(renderer, &debug_tile);
}

#endif