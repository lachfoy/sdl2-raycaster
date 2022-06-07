#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "util.h"
#include "map.h"
#include "game.h"
#include "raycast.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RENDER_WIDTH 100
#define RENDER_HEIGHT 75

int main(int argc, char *argv[])
{
    double posX = 22, posY = 12;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

    double time = 0; //time of current frame
    double oldTime = 0; //time of previous frame

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "sdl2-raycaster",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);
    if (window == NULL)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);
    if (texture == NULL)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    // MAIN LOOP
    int quit = false;
    SDL_Event e;
    while(!quit)
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
                // buffer[i] = ARGB(FastRand() % 256, FastRand() % 256, FastRand() % 256, 255);
                buffer[i] = 0x40AAEE; // fill with nice light blue pixels :)


            // raycast???
            for (int x = 0; x < RENDER_WIDTH; x++)
            {
                hitInfo info = raycast(x, RENDER_WIDTH, RENDER_HEIGHT, level_map,
                    posX, posY, dirX, dirY, planeX, planeY);

                // draw a vertical stripe
                for (uint32_t y = info.drawStart; y < info.drawEnd; ++y)
                {
                    // width * row + col
                    buffer[RENDER_WIDTH * y + x] = 0x0000EE;
                } 
            }

            int x = 15, y = 90;
            buffer[RENDER_WIDTH * y + x] = 0x0000EE;

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
                quit = true;
            }
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}