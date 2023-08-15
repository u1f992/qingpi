#ifndef _3DS_COMMON_H_
#define _3DS_COMMON_H_

#include <stdint.h>

inline uint16_t rescale_(uint16_t value, double in_min, double in_max, double out_min, double out_max)
{
    return (uint16_t)(((value - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min);
}

#endif