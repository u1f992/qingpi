#include "adg801.h"

void adg801_on(ADG801 *self)
{
    if (self == NULL)
    {
        return;
    }

    digitalWrite(self->in, HIGH);
}

void adg801_off(ADG801 *self)
{
    if (self == NULL)
    {
        return;
    }

    digitalWrite(self->in, LOW);
}

ADG801 *adg801_new(pin_size_t in)
{
    ADG801 *self = (ADG801 *)malloc(sizeof(ADG801));
    if (self == NULL)
    {
        return NULL;
    }

    self->in = in;

    adg801_off(self);

    return self;
}

void adg801_delete(ADG801 *self)
{
    if (self == NULL)
    {
        return;
    }

    adg801_off(self);

    free(self);
}