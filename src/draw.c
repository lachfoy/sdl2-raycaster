#include "draw.h"

void draw_blank_column(PixBuffer *pixBuffer, uint16_t x, uint16_t y1, uint16_t y2, uint32_t color)
{
    for (int y = y1; y < y2+1; ++y) // draw a vertical strip into the pixel buffer
        {
            set_pixel(pixBuffer, x, y, color);
        }
}

void draw_raycast(PixBuffer *pixBuffer, float posX, float posY, float dirX, float dirY, float planeX, float planeY, Map* map)
{
    // calculate a ray for each horizontal pixel
    for (uint16_t x = 0; x < RENDER_WIDTH; x++)
    {
        // calculate ray position and direction
        float cameraX = 2 * x / (float)(RENDER_WIDTH) - 1; // x-coordinate in camera space
        float rayDirX = dirX + planeX * cameraX;
        float rayDirY = dirY + planeY * cameraX;

        // which box of the map we're in
        uint8_t mapX = (uint8_t)(posX);
        uint8_t mapY = (uint8_t)(posY);

        //length of ray from current position to next x or y-side
        float sideDistX;
        float sideDistY;

        //length of ray from one x or y-side to next x or y-side
        float deltaDistX = (rayDirX == 0) ? 1e30 : fabsf(1 / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30 : fabsf(1 / rayDirY);
        float perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        uint8_t stepX;
        uint8_t stepY;

        uint8_t hit = 0; //was there a wall hit?
        uint8_t side; //was a NS or a EW wall hit?

        //calculate step and initial sideDist
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        //perform DDA
        while (hit == 0)
        {
            //jump to next map square, either in x-direction, or in y-direction
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
            //Check if ray has hit a wall
            if (map->data[mapY * map->width + mapX] > 0) hit = 1; // ALWAYS [y * w + x] for consistency
        }

        //Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
        if(side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);

        //Calculate height of line to draw on screen
        uint16_t lineHeight = (uint16_t)(RENDER_HEIGHT / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        uint32_t drawStart = -lineHeight / 2 + RENDER_HEIGHT / 2;
        if (drawStart < 0) drawStart = 1;
        uint32_t drawEnd = lineHeight / 2 + RENDER_HEIGHT / 2;
        if (drawEnd >= RENDER_HEIGHT) drawEnd = RENDER_HEIGHT - 1;

        uint32_t color = 0x00FFFF;
        //give x and y sides different brightness
        if (side == 1) { color = color / 2; }

        //draw the pixels of the stripe as a vertical line
        draw_blank_column(pixBuffer, x, drawStart, drawEnd, color);
    }
}
