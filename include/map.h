#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Map
{
    uint8_t* data;
    uint8_t width;
    uint8_t height;

} Map;

// generate a test map
// later this will probably be done by loading a binary or plaintext file
void generate_test_map(Map* test_map);

#endif