#include "ncm.h"

#include <stdlib.h>

void ncm_touchscreen_hold(NcmTouchScreen *self, double x, double y)
{
    if (self == NULL)
    {
        return;
    }
    if (x < 0.0 || 1.0 < x || y < 0.0 || 1.0 < y)
    {
        return;
    }

    self->vertical->set_wiper_position(self->vertical, y);
    self->horizontal->set_wiper_position(self->horizontal, x);

    self->vertical->power_on(self->vertical);
    self->horizontal->power_on(self->horizontal);
    self->sw->on(self->sw);
}

void ncm_touchscreen_release(NcmTouchScreen *self)
{
    if (self == NULL)
    {
        return;
    }

    self->sw->off(self->sw);
    self->vertical->shutdown(self->vertical);
    self->horizontal->shutdown(self->horizontal);
}

NcmTouchScreen *ncm_touchscreen_new(NcmDigitalPotentiometerInterface *vertical, NcmDigitalPotentiometerInterface *horizontal, NcmSwitchInterface *sw)
{
    if (vertical == NULL || horizontal == NULL || sw == NULL)
    {
        return NULL;
    }

    NcmTouchScreen *self = (NcmTouchScreen *)malloc(sizeof(NcmTouchScreen));
    if (self == NULL)
    {
        return NULL;
    }

    self->vertical = vertical;
    self->horizontal = horizontal;
    self->sw = sw;

    ncm_touchscreen_release(self);

    return self;
}

void ncm_touchscreen_delete(NcmTouchScreen *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}