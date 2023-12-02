#include "adapters.h"

static void ad840x_adapter_set_wiper_position(NcmDigitalPotentiometerInterface *parent, double position)
{
    AD840XAdapter *self = (AD840XAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    ad840x_set(self->pots, self->addr, (uint8_t)(255 * position));
}

static void ad840x_adapter_power_on(NcmDigitalPotentiometerInterface *parent)
{
    AD840XAdapter *self = (AD840XAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    ad840x_power_on(self->pots);
}

static void ad840x_adapter_shutdown(NcmDigitalPotentiometerInterface *parent)
{
    AD840XAdapter *self = (AD840XAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    ad840x_shutdown(self->pots);
}

AD840XAdapter *ad840x_adapter_new(AD840X *pots, AD840XAddress addr)
{
    if (pots == NULL)
    {
        return NULL;
    }

    AD840XAdapter *self = (AD840XAdapter *)malloc(sizeof(AD840XAdapter));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.set_wiper_position = ad840x_adapter_set_wiper_position;
    self->parent.power_on = ad840x_adapter_power_on;
    self->parent.shutdown = ad840x_adapter_shutdown;

    self->pots = pots;
    self->addr = addr;

    return self;
}

void ad840x_adapter_delete(AD840XAdapter *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}