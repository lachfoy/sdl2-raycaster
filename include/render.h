#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "defines.h"

int32_t pitch;
uint32_t *buffer;

SDL_Renderer *renderer;
SDL_Texture *texture;

int renderInit(SDL_Window *window);
void renderClear(uint32_t color);
void renderCleanup();

int renderInit(SDL_Window *window)
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) return 1;

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);
    if (texture == NULL) return 1;

    return 0;
}

void renderClear(uint32_t color)
{
    // The Back Buffer texture may be stored with an extra bit of width (pitch) on the video card in order to properly
    // align it in VRAM should the width not lie on the correct memory boundary (usually four bytes).
    int32_t pitch = 0;

    // This will hold a pointer to the memory position in VRAM where our Back Buffer texture lies
    uint32_t *buffer = NULL;

    // Lock the memory in order to write our Back Buffer image to it
    if (!SDL_LockTexture(texture, NULL, (void **)&buffer, &pitch))
    {
        // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
        // as it will always be a multiple of four
        pitch /= sizeof(uint32_t);

        // Fill texture with randomly colored pixels
        for (uint32_t i = 0; i < RENDER_WIDTH * RENDER_HEIGHT; ++i)
            // buffer[i] = ARGB(FastRand() % 256, FastRand() % 256, FastRand() % 256, 255);
            buffer[i] = color; // fill with nice light blue pixels :)

        // Unlock the texture in VRAM to let the GPU know we are done writing to it
        SDL_UnlockTexture(texture);

        // Copy our texture in VRAM to the display framebuffer in VRAM
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Copy the VRAM framebuffer to the display
        SDL_RenderPresent(renderer);
    }
}

void renderCleanup()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
}

#endif