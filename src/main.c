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
#include "pixbuffer.h"

int main(int argc, char *argv[])
{
    float posX = 2, posY = 3;  //x and y start position
    float dirX = -1, dirY = 0; //initial direction vector
    float planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane
    float moveSpeed = 0.2;
    float rotSpeed = 0.1;
    float oldDirX;
    float oldDirY;
    float oldPlaneX;
    float oldPlaneY;

    float time = 0; //time of current frame
    float oldTime = 0; //time of previous frame

    Map* test_map = generate_test_map();

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

    PixBuffer* pixBuffer = init_pixbuffer(RENDER_WIDTH, RENDER_HEIGHT);

    // MAIN LOOP
    int quit = false;
    SDL_Event e;
    while(!quit)
    {
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
                    posX += dirX * moveSpeed;
                    posY += dirY * moveSpeed;
                    break;

                case SDLK_DOWN:
                    posX -= dirX * moveSpeed;
                    posY -= dirY * moveSpeed;
                    break;
                
                case SDLK_LEFT:
                    //both camera direction and camera plane must be rotated
                    oldDirX = dirX;
                    dirX = dirX * cosf(rotSpeed) - dirY * sinf(rotSpeed);
                    dirY = oldDirX * sinf(rotSpeed) + dirY * cosf(rotSpeed);
                    oldPlaneX = planeX;
                    planeX = planeX * cosf(rotSpeed) - planeY * sinf(rotSpeed);
                    planeY = oldPlaneX * sinf(rotSpeed) + planeY * cosf(rotSpeed);
                    break;

                case SDLK_RIGHT:
                    //both camera direction and camera plane must be rotated
                    oldDirX = dirX;
                    dirX = dirX * cosf(-rotSpeed) - dirY * sinf(-rotSpeed);
                    dirY = oldDirX * sinf(-rotSpeed) + dirY * cosf(-rotSpeed);
                    oldPlaneX = planeX;
                    planeX = planeX * cosf(-rotSpeed) - planeY * sinf(-rotSpeed);
                    planeY = oldPlaneX * sinf(-rotSpeed) + planeY * cosf(-rotSpeed);
                    break;

                default:
                    break;
                }
            }
        }
    
        // The Back Buffer texture may be stored with an extra bit of width (pitch) on the video card in order to properly
        // align it in VRAM should the width not lie on the correct memory boundary (usually four bytes).
        int32_t pitch = 0;

        // Lock the memory in order to write our Back Buffer image to it
        if (!SDL_LockTexture(texture, NULL, (void **)&pixBuffer->data, &pitch))
        {
            // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
            // as it will always be a multiple of four
            pitch /= sizeof(uint32_t);

            clear_pixbuffer(pixBuffer);
            //draw_raycast(pixBuffer, posX, posY, dirX, dirY, planeX, planeY, test_map);

            // Unlock the texture in VRAM to let the GPU know we are done writing to it
            SDL_UnlockTexture(texture);

            // Copy our texture in VRAM to the display framebuffer in VRAM
            SDL_RenderCopy(renderer, texture, NULL, NULL);
        }

        // debug drawing functions use SDL drawing funcs instead of the pixel buffer
        debug_draw_map(renderer, test_map);
        debug_draw_object(renderer, posX, posY, dirX, dirY);

        debug_draw_ray(renderer, posX, posY, dirX, dirY, test_map);

        // Copy the VRAM framebuffer to the display
        SDL_RenderPresent(renderer);
    }

    delete_map(test_map);
    delete_pixbuffer(pixBuffer);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}