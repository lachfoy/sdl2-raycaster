#ifndef PIXBUFFER_H
#define PIXBUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h> //for some reason needed for memset

// struct holds a width and height for a buffer of 32bpp color pixels
typedef struct PixBuffer
{
    uint16_t width;
    uint16_t height;
    uint32_t* data;
} PixBuffer;

PixBuffer* init_pixbuffer(uint16_t width, uint16_t height);
void delete_pixbuffer(PixBuffer* pixBuffer);
void clear_pixbuffer(PixBuffer* pixBuffer);
uint32_t get_pixel(PixBuffer* pixBuffer, uint16_t x, uint16_t y);
void set_pixel(PixBuffer* pixBuffer, uint16_t x, uint16_t y, uint32_t value);

#endif