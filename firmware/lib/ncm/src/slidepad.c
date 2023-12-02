#include "ncm.h"

#include <stdlib.h>

void ncm_slidepad_hold(NcmSlidePad *self, double x, double y)
{
    if (self == NULL)
    {
        return;
    }
    if (x < 0.0 || 1.0 < x || y < 0.0 || 1.0 < y)
    {
        return;
    }

    if (x == NCM_SLIDEPAD_NEUTRAL)
    {
        self->horizontal->source(self->horizontal, 0.0);
    }
    else if (NCM_SLIDEPAD_NEUTRAL < x)
    {
        // 0.5-1.0
        // => 0.0-1.0
        self->horizontal->sink(self->horizontal, 2.0 * x - 1.0);
    }
    else
    {
        // 0.0-0.5
        // => 1.0-0.0
        self->horizontal->source(self->horizontal, 1.0 - 2.0 * x);
    }

    if (y == NCM_SLIDEPAD_NEUTRAL)
    {
        self->vertical->source(self->vertical, 0.0);
    }
    else if (NCM_SLIDEPAD_NEUTRAL < y)
    {
        self->vertical->sink(self->vertical, 2.0 * y - 1.0);
    }
    else
    {
        self->vertical->source(self->vertical, 1.0 - 2.0 * y);
    }
}

void ncm_slidepad_release(NcmSlidePad *self)
{
    if (self == NULL)
    {
        return;
    }

    ncm_slidepad_hold(self, NCM_SLIDEPAD_NEUTRAL, NCM_SLIDEPAD_NEUTRAL);
}

NcmSlidePad *ncm_slidepad_new(NcmCurrentDAConverterInterface *vertical, NcmCurrentDAConverterInterface *horizontal)
{
    if (vertical == NULL || horizontal == NULL)
    {
        return NULL;
    }

    NcmSlidePad *self = (NcmSlidePad *)malloc(sizeof(NcmSlidePad));
    if (self == NULL)
    {
        return NULL;
    }

    self->vertical = vertical;
    self->horizontal = horizontal;

    ncm_slidepad_release(self);

    return self;
}

void ncm_slidepad_delete(NcmSlidePad *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}