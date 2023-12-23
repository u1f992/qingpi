#include "qingpi_adapters.h"

static void gpio_adapter_set_low(QpiGeneralPurposeIOInterface *parent)
{
    GPIOAdapter *self = (GPIOAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    pinMode(self->pin, OUTPUT);
    digitalWrite(self->pin, LOW);
}

static void gpio_adapter_set_hi_z(QpiGeneralPurposeIOInterface *parent)
{
    GPIOAdapter *self = (GPIOAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    pinMode(self->pin, INPUT);
}

GPIOAdapter *gpio_adapter_new(pin_size_t pin)
{
    GPIOAdapter *self = (GPIOAdapter *)malloc(sizeof(GPIOAdapter));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.set_low = gpio_adapter_set_low;
    self->parent.set_hi_z = gpio_adapter_set_hi_z;

    self->pin = pin;

    return self;
}

void gpio_adapter_delete(GPIOAdapter *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}