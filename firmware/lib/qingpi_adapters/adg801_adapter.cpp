#include "qingpi_adapters.h"

static void adg801_adapter_on(QpiSPSTSwitchInterface *parent)
{
    ADG801Adapter *self = (ADG801Adapter *)parent;
    if (self == NULL)
    {
        return;
    }

    adg801_on(self->sw);
}

static void adg801_adapter_off(QpiSPSTSwitchInterface *parent)
{
    ADG801Adapter *self = (ADG801Adapter *)parent;
    if (self == NULL)
    {
        return;
    }

    adg801_off(self->sw);
}

ADG801Adapter *adg801_adapter_new(ADG801 *sw)
{
    if (sw == NULL)
    {
        return NULL;
    }

    ADG801Adapter *self = (ADG801Adapter *)malloc(sizeof(ADG801Adapter));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.on = adg801_adapter_on;
    self->parent.off = adg801_adapter_off;

    self->sw = sw;

    return self;
}

void adg801_adapter_delete(ADG801Adapter *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}