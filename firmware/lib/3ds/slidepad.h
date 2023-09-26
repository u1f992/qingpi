#ifndef _3DS_SLIDEPAD_H_
#define _3DS_SLIDEPAD_H_

#include "ds4432.h"

#include "common.h"

typedef struct
{
    DS4432 *dac_;
} slidepad_t;

const uint8_t SLIDEPAD_NEUTRAL = 128;

inline void slidepad_hold(slidepad_t *sp, uint8_t x, uint8_t y)
{
    if (x == SLIDEPAD_NEUTRAL)
    {
        ds4432_set(sp->dac_, DS4432_OUT1, DS4432_SOURCE, 0);
    }
    else if(SLIDEPAD_NEUTRAL < x)
    {
        // 129, 130, ..., 255
        ds4432_set(sp->dac_, DS4432_OUT1, DS4432_SINK, rescale_(x, 129, 255, 1, 127));
    }
    else
    {
        // 127, 126, ..., 0
        ds4432_set(sp->dac_, DS4432_OUT1, DS4432_SOURCE, rescale_(127 - x, 0, 127, 1, 127));
    }

    if (y == SLIDEPAD_NEUTRAL)
    {
        ds4432_set(sp->dac_, DS4432_OUT0, DS4432_SOURCE, 0);
    }
    else if(SLIDEPAD_NEUTRAL < y)
    {
        // 129, 130, ..., 255
        ds4432_set(sp->dac_, DS4432_OUT0, DS4432_SINK, rescale_(y, 129, 255, 1, 127));
    }
    else
    {
        // 127, 126, ..., 0
        ds4432_set(sp->dac_, DS4432_OUT0, DS4432_SOURCE, rescale_(127 - y, 0, 127, 1, 127));
    }
}

inline void slidepad_release(slidepad_t *sp)
{
    slidepad_hold(sp, SLIDEPAD_NEUTRAL, SLIDEPAD_NEUTRAL);
}

inline void slidepad_init(slidepad_t *sp, DS4432 *dac)
{
    sp->dac_ = dac;
    slidepad_release(sp);
}

#endif