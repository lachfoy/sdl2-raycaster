#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

uint32_t ARGB(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha);

// //Marsaglia's xorshf generator (Fast Random Function)
static uint32_t s_randX = 123456789;
static uint32_t s_randY = 362436069;
static uint32_t s_randZ = 521288629;
uint32_t FastRand(void);

#endif