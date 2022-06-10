#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "defines.h"
#include "draw.h"
#include "map.h"
#include "debug_draw.h"

int main(int argc, char *argv[])
{
    float posX = 2, posY = 3;  //x and y start position
    float dirX = -1, dirY = 0; //initial direction vector
    float planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

    float time = 0; //time of current frame
    float oldTime = 0; //time of previous frame

    Map test_map;
    generate_test_map(&test_map);
    // for (int i = 0; i < 10 * 10; i++)
    //     printf("%i\n", (&test_map)->data[i]);

    // create an xor bitmap texture for testing
    uint32_t* test_texture;
    test_texture = (uint32_t*)malloc(sizeof(uint32_t) * TEX_WIDTH * TEX_HEIGHT);
    for (uint8_t x = 0; x < TEX_WIDTH; x++)
        for (uint8_t y = 0; y < TEX_HEIGHT; y++)
        {
            uint32_t xorcolor = (x * 256 / TEX_WIDTH) ^ (y * 256 / TEX_HEIGHT);
            test_texture[TEX_WIDTH * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor;;
        }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);
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
        uint32_t *buffer = NULL;

        // Lock the memory in order to write our Back Buffer image to it
        if (!SDL_LockTexture(texture, NULL, (void **)&buffer, &pitch))
        {
            // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
            // as it will always be a multiple of four
            pitch /= sizeof(uint32_t);

            
            for (uint32_t i = 0; i < RENDER_WIDTH * RENDER_HEIGHT; ++i)
                buffer[i] = 0x0000AA;
            draw_raycast(buffer, &test_map, test_texture, posX, posY, dirX, dirY, planeX, planeY);

            // Unlock the texture in VRAM to let the GPU know we are done writing to it
            SDL_UnlockTexture(texture);

            // Copy our texture in VRAM to the display framebuffer in VRAM
            SDL_RenderCopy(renderer, texture, NULL, NULL);
        }

        debug_draw_map(renderer, &test_map);
        debug_draw_object(renderer, posX, posY, dirX, dirY);

        // Copy the VRAM framebuffer to the display
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                
                case SDLK_UP:
                    posY -= 0.2;
                    break;

                case SDLK_DOWN:
                    posY += 0.2;
                    break;
                
                case SDLK_LEFT:
                    posX -= 0.2;
                    break;

                case SDLK_RIGHT:
                    posX += 0.2;
                    break;

                default:
                    break;
                }
            }
        }

        SDL_Delay(33.3);
    }

    free(test_map.data);
    free(test_texture);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}