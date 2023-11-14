#include <Arduino.h>

#include "ncm.h"

typedef struct GPIOAdapter
{
    NcmGeneralPurposeIOInterface parent;
    pin_size_t pin;
} GPIOAdapter;

static void gpio_adapter_set_low(NcmGeneralPurposeIOInterface *parent)
{
    GPIOAdapter *self = (GPIOAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    pinMode(self->pin, OUTPUT);
    digitalWrite(self->pin, LOW);
}

static void gpio_adapter_set_high(NcmGeneralPurposeIOInterface *parent)
{
    GPIOAdapter *self = (GPIOAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    pinMode(self->pin, OUTPUT);
    digitalWrite(self->pin, HIGH);
}

static void gpio_adapter_set_hi_z(NcmGeneralPurposeIOInterface *parent)
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
    self->parent.set_high = gpio_adapter_set_high;
    self->parent.set_hi_z = gpio_adapter_set_hi_z;

    self->pin = pin;

    gpio_adapter_set_hi_z((NcmGeneralPurposeIOInterface *)self);

    return self;
}

void gpio_adapter_delete(GPIOAdapter *self)
{
    if (self == NULL)
    {
        return;
    }

    gpio_adapter_set_hi_z((NcmGeneralPurposeIOInterface *)self);

    free(self);
}