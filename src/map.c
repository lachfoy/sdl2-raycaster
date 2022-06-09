#include "map.h"

void generate_test_map(Map* test_map)
{
    printf("generating test map\n");
    test_map->width = 10;
    test_map->height = 10;

    // create some data to transplant into the map
    uint8_t data[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    // allocate memory and put the data into the map :)
    test_map->data = (uint8_t*)malloc(sizeof(uint8_t) * 10 * 10);
    for (int i = 0; i < 10 * 10; i++)
        test_map->data[i]=data[i];

    printf("test map generated\n");
}