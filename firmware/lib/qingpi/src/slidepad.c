#include "qingpi.h"

#include <stdlib.h>

void qpi_slidepad_hold(QpiSlidePad *self, uint16_t x, uint16_t y)
{
    if (self == NULL)
    {
        return;
    }

    if (x == QPI_SLIDEPAD_NEUTRAL)
    {
        self->horizontal->source(self->horizontal, 0);
    }
    else if (QPI_SLIDEPAD_NEUTRAL < x)
    {
        // 32768-65535
        // x - QPI_SLIDEPAD_NEUTRAL = 1-32768
        self->horizontal->sink(self->horizontal, (uint16_t)(((double)(x - QPI_SLIDEPAD_NEUTRAL) / 32768) * UINT16_MAX));
    }
    else
    {
        // 0-32766
        // QPI_SLIDEPAD_NEUTRAL - x = 1-32767
        self->horizontal->source(self->horizontal, (uint16_t)(((double)(QPI_SLIDEPAD_NEUTRAL - x) / 32767) * UINT16_MAX));
    }

    if (y == QPI_SLIDEPAD_NEUTRAL)
    {
        self->vertical->source(self->vertical, 0);
    }
    else if (QPI_SLIDEPAD_NEUTRAL < y)
    {
        self->vertical->sink(self->vertical, (uint16_t)(((double)(y - QPI_SLIDEPAD_NEUTRAL) / 32768) * UINT16_MAX));
    }
    else
    {
        self->vertical->source(self->vertical, (uint16_t)(((double)(QPI_SLIDEPAD_NEUTRAL - y) / 32767) * UINT16_MAX));
    }
}

void qpi_slidepad_release(QpiSlidePad *self)
{
    if (self == NULL)
    {
        return;
    }

    qpi_slidepad_hold(self, QPI_SLIDEPAD_NEUTRAL, QPI_SLIDEPAD_NEUTRAL);
}

QpiSlidePad *qpi_slidepad_new(QpiCurrentDAConverterInterface *vertical, QpiCurrentDAConverterInterface *horizontal)
{
    if (vertical == NULL || horizontal == NULL)
    {
        return NULL;
    }

    QpiSlidePad *self = (QpiSlidePad *)malloc(sizeof(QpiSlidePad));
    if (self == NULL)
    {
        return NULL;
    }

    self->vertical = vertical;
    self->horizontal = horizontal;

    qpi_slidepad_release(self);

    return self;
}

void qpi_slidepad_delete(QpiSlidePad *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}