#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "util.h"
#include "map.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RENDER_WIDTH 100
#define RENDER_HEIGHT 75

int main(int argc, char *argv[])
{
    printf("map:%i", map[2][2]);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "sdl2-cmake",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);

    int quit = 0;
    SDL_Event e;
    while(quit == 0)
    {
        // The Back Buffer texture may be stored with an extra bit of width (pitch) on the video card in order to properly
        // align it in VRAM should the width not lie on the correct memory boundary (usually four bytes).
        int32_t pitch = 0;

        // This will hold a pointer to the memory position in VRAM where our Back Buffer texture lies
        uint32_t* buffer = NULL;

        // Lock the memory in order to write our Back Buffer image to it
        if (!SDL_LockTexture(texture, NULL, (void**)&buffer, &pitch))
        {
            // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
            // as it will always be a multiple of four
            pitch /= sizeof(uint32_t);

            // Fill texture with randomly colored pixels
            for (uint32_t i = 0; i < RENDER_WIDTH * RENDER_HEIGHT; ++i)
                buffer[i] = ARGB(FastRand() % 256, FastRand() % 256, FastRand() % 256, 255);
                // pPixelBuffer[i] =  (255 << 24) | (123 << 16) | (11 << 8) | 240;


            // Unlock the texture in VRAM to let the GPU know we are done writing to it
            SDL_UnlockTexture(texture);

            // Copy our texture in VRAM to the display framebuffer in VRAM
            SDL_RenderCopy(renderer, texture, NULL, NULL);

            // Copy the VRAM framebuffer to the display
            SDL_RenderPresent(renderer);
        }

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:
            case SDL_QUIT:
                quit = 1;
            }
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}