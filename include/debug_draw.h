#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <SDL2/SDL.h>

#include "map.h"

#define DEBUG_DRAW_SIZE 32

void debug_draw_map(SDL_Renderer* renderer, Map* map);
void debug_draw_object(SDL_Renderer* renderer, float posX, float posY, float dirX, float dirY);

void debug_draw_map(SDL_Renderer* renderer, Map* map)
{
    for (uint8_t x = 0; x < map->width; x++)
        for (uint8_t y = 0; y < map->height; y++)
        if (map->data[map->width * y + x] > 0)
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

#endif