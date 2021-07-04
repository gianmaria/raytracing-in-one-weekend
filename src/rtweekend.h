#pragma once

#include <math.h>
#include <stdlib.h>

const float INF = INFINITY;
const float PI = 3.141592653f;

inline double deg_to_rads(float deg) 
{
    return deg * PI / 180.0f;
}

inline float random_float()
{
    // return random float in range [0, 1)
    return (float)rand() / ((float)RAND_MAX + 1.0f);
}

inline float random_float(float min, float max)
{
    // return random float in range [min, max)
    return min + (max - min) * random_float();
}

inline float clamp(float x, float min, float max) 
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#define ArrayCount(A) (sizeof(A) / sizeof(A[0]))