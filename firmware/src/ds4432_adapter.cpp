#include "adapters.h"

static void ds4432_adapter_sink(NcmCurrentDAConverterInterface *parent, double val)
{
    DS4432Adapter *self = (DS4432Adapter *)parent;
    if (self == NULL)
    {
        return;
    }

    ds4432_set(self->dac, self->addr, DS4432_SINK, (uint8_t)(0b01111111 * val));
}

static void ds4432_adapter_source(NcmCurrentDAConverterInterface *parent, double val)
{
    DS4432Adapter *self = (DS4432Adapter *)parent;
    if (self == NULL)
    {
        return;
    }

    ds4432_set(self->dac, self->addr, DS4432_SOURCE, (uint8_t)(0b01111111 * val));
}

DS4432Adapter *ds4432_adapter_new(DS4432 *dac, DS4432MemoryAddress addr)
{
    if (dac == NULL)
    {
        return NULL;
    }

    DS4432Adapter *self = (DS4432Adapter *)malloc(sizeof(DS4432Adapter));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.sink = ds4432_adapter_sink;
    self->parent.source = ds4432_adapter_source;

    self->dac = dac;
    self->addr = addr;

    return self;
}

void ds4432_adapter_delete(DS4432Adapter *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}