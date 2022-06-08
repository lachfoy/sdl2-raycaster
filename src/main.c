#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "util.h"
#include "map.h"
#include "raycast.h"
#include "defines.h"
#include "render.h"

int main(int argc, char *argv[])
{
    double posX = 22, posY = 12;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

    double time = 0; //time of current frame
    double oldTime = 0; //time of previous frame

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


    if (renderInit(window) != 0)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    // MAIN LOOP
    int quit = false;
    SDL_Event e;
    while(!quit)
    {
        renderClear(0x00FF00);

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

    renderCleanup();
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}