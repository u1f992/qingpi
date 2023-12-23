#include "qingpi.h"

#include <stdlib.h>

void qpi_touchscreen_hold(QpiTouchScreen *self, uint16_t x, uint16_t y)
{
    if (self == NULL)
    {
        return;
    }

    self->vertical->set_wiper_position(self->vertical, y);
    self->horizontal->set_wiper_position(self->horizontal, x);

    self->vertical->power_on(self->vertical);
    self->horizontal->power_on(self->horizontal);
    self->sw->on(self->sw);
}

void qpi_touchscreen_release(QpiTouchScreen *self)
{
    if (self == NULL)
    {
        return;
    }

    self->sw->off(self->sw);
    self->vertical->shutdown(self->vertical);
    self->horizontal->shutdown(self->horizontal);
}

QpiTouchScreen *qpi_touchscreen_new(QpiDigitalPotentiometerInterface *vertical, QpiDigitalPotentiometerInterface *horizontal, QpiSPSTSwitchInterface *sw)
{
    if (vertical == NULL || horizontal == NULL || sw == NULL)
    {
        return NULL;
    }

    QpiTouchScreen *self = (QpiTouchScreen *)malloc(sizeof(QpiTouchScreen));
    if (self == NULL)
    {
        return NULL;
    }

    self->vertical = vertical;
    self->horizontal = horizontal;
    self->sw = sw;

    qpi_touchscreen_release(self);

    return self;
}

void qpi_touchscreen_delete(QpiTouchScreen *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}