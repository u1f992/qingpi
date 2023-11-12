#include "ncm.h"

#include <stdlib.h>

void ncm_button_hold(NcmButton *self)
{
    if (self == NULL)
    {
        return;
    }

    self->gpio->set_low(self->gpio);
}

void ncm_button_release(NcmButton *self)
{
    if (self == NULL)
    {
        return;
    }

    self->gpio->set_hi_z(self->gpio);
}

NcmButton *ncm_button_new(NcmGeneralPurposeIOInterface *gpio)
{
    if (gpio == NULL)
    {
        return NULL;
    }

    NcmButton *self = (NcmButton *)malloc(sizeof(NcmButton));
    if (self == NULL)
    {
        return NULL;
    }

    self->gpio = gpio;

    ncm_button_release(self);

    return self;
}

void ncm_button_delete(NcmButton *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}