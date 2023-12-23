#include "qingpi_adapters.h"

static void ad840x_adapter_set_wiper_position(QpiDigitalPotentiometerInterface *parent, uint16_t position)
{
    AD840XAdapter *self = (AD840XAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    ad840x_set(self->pots, self->addr, (uint8_t)(((double)position / UINT16_MAX) * UINT8_MAX));
}

static void ad840x_adapter_power_on(QpiDigitalPotentiometerInterface *parent)
{
    AD840XAdapter *self = (AD840XAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    ad840x_power_on(self->pots);
}

static void ad840x_adapter_shutdown(QpiDigitalPotentiometerInterface *parent)
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