#pragma once

#include "ds4432.h"

#include "utility.h"

typedef struct
{
    ds4432_t *dac;
} slidepad_t;

#define SLIDEPAD_NEUTRAL 128

void slidepad_hold(slidepad_t *sp, uint8_t x, uint8_t y)
{
    if (x == SLIDEPAD_NEUTRAL)
    {
        ds4432_set(sp->dac, DS4432_OUT0, DS4432_SOURCE, 0);
    }
    else if(SLIDEPAD_NEUTRAL < x)
    {
        // 129, 130, ..., 255
        ds4432_set(sp->dac, DS4432_OUT0, DS4432_SINK, rescale(x, 129, 255, 1, 127));
    }
    else
    {
        // 127, 126, ..., 0
        ds4432_set(sp->dac, DS4432_OUT0, DS4432_SOURCE, rescale(127 - x, 0, 127, 1, 127));
    }

    if (y == SLIDEPAD_NEUTRAL)
    {
        ds4432_set(sp->dac, DS4432_OUT1, DS4432_SOURCE, 0);
    }
    else if(SLIDEPAD_NEUTRAL < y)
    {
        // 129, 130, ..., 255
        ds4432_set(sp->dac, DS4432_OUT1, DS4432_SINK, rescale(y, 129, 255, 1, 127));
    }
    else
    {
        // 127, 126, ..., 0
        ds4432_set(sp->dac, DS4432_OUT1, DS4432_SOURCE, rescale(127 - y, 0, 127, 1, 127));
    }
}

void slidepad_release(slidepad_t *sp)
{
    slidepad_hold(sp, SLIDEPAD_NEUTRAL, SLIDEPAD_NEUTRAL);
}

void slidepad_init(slidepad_t *sp, ds4432_t *dac)
{
    sp->dac = dac;
    slidepad_release(sp);
}