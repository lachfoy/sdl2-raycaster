#include "pixbuffer.h"

PixBuffer* init_pixbuffer(uint16_t width, uint16_t height)
{
    PixBuffer* pixBuffer = (PixBuffer*)malloc(sizeof(PixBuffer));

    pixBuffer->width = width;
    pixBuffer->height = height;

    pixBuffer->data = (uint32_t*)malloc(sizeof(uint32_t) * pixBuffer->width * pixBuffer->height);

    return pixBuffer;
}

void delete_pixbuffer(PixBuffer* pixBuffer)
{
    //free(pixBuffer->data); // somehow this data gets freed earlier?
    free(pixBuffer);
}

void clear_pixbuffer(PixBuffer* pixBuffer)
{
    memset(pixBuffer->data, 0x000000, sizeof(uint32_t) * pixBuffer->width * pixBuffer->height);
}

uint32_t get_pixel(PixBuffer* pixBuffer, uint16_t x, uint16_t y)
{
    return pixBuffer->data[y * pixBuffer->width + x]; // ALWAYS [y * w + x] for consistency
}

void set_pixel(PixBuffer* pixBuffer, uint16_t x, uint16_t y, uint32_t value)
{
    if (x < pixBuffer->width && y < pixBuffer->height)
    {
        pixBuffer->data[y * pixBuffer->width + x] = value;
    }
}