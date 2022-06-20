#include "map.h"

Map* generate_test_map()
{
    // generate a test map with some hard coded data
    Map* test_map = (Map*)malloc(sizeof(Map));

    test_map->width = 11;
    test_map->height = 10;

    // create some data to transplant into the map
    uint8_t data[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    // allocate memory and put the data into the map :)
    test_map->data = (uint8_t*)malloc(sizeof(uint8_t) * 11 * 10);
    for (int i = 0; i < 11 * 10; i++)
        test_map->data[i]=data[i];

    return test_map;
}

void delete_map(Map* map)
{
    free(map->data);
    free(map);
}