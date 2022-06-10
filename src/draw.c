#include "draw.h"

void draw_pixel(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color)
{
    if (x >= 0 && x < RENDER_WIDTH && y >= 0 && y < RENDER_HEIGHT)
    {
        buffer[RENDER_WIDTH * y + x] = color;
    }
}

void draw_raycast(uint32_t *buffer, Map* map,
    float posX, float posY, float dirX, float dirY,
    float planeX, float planeY)
{
    for (uint16_t x = 0; x < RENDER_WIDTH; x++) // calculate a ray for each horizontal pixel
    {
        // calculate ray position and direction
        float cameraX = 2 * x / (float)(RENDER_WIDTH) - 1; // x-coordinate in camera space
        float rayDirX = dirX + planeX * cameraX;
        float rayDirY = dirY + planeY * cameraX;

        // which box of the map we're in
        uint8_t mapX = (uint8_t)(posX);
        uint8_t mapY = (uint8_t)(posY);

        // length of ray from current position to next x or y-side
        float sideDistX;
        float sideDistY;

        // length of ray from one x or y-side to next x or y-side
        // float deltaDistX = (rayDirX == 0) ? 1e30 : abs(1 / rayDirX);
        // float deltaDistY = (rayDirY == 0) ? 1e30 : abs(1 / rayDirY);
        //deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
        //deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
        float deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        float deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
        float perpWallDist;

        // what direction to step in x or y-direction (either +1 or -1)
        int8_t stepX;
        int8_t stepY;

        uint8_t hit = 0; // was there a wall hit?
        uint8_t side;    // was a NS or a EW wall hit?

        // calculate step and initial sideDist
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        // perform DDA
        while (hit == 0)
        {
            // jump to next map square, either in x-direction, or in y-direction
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Check if ray has hit a wall
            if (map->data[map->width * mapY + mapX] > 0)
                hit = 1;
        }

        // Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
        if (side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        // Calculate height of line to draw on screen
        int lineHeight = (int)(RENDER_HEIGHT / perpWallDist);

        // calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + RENDER_HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + RENDER_HEIGHT / 2;
        if (drawEnd >= RENDER_HEIGHT)
            drawEnd = RENDER_HEIGHT - 1;

        uint32_t wall_color = (side == 1) ? 0xAA00BB : 0xEE00DD;

        for (int y = drawStart; y < drawEnd+1; ++y) // draw a vertical strip into the pixel buffer
        {
            buffer[RENDER_WIDTH * y + x] = wall_color;
        }
    }
}