#include "qingpi.h"

#include <stdlib.h>

void qpi_button_hold(QpiButton *self)
{
    if (self == NULL)
    {
        return;
    }

    self->gpio->set_low(self->gpio);
}

void qpi_button_release(QpiButton *self)
{
    if (self == NULL)
    {
        return;
    }

    self->gpio->set_hi_z(self->gpio);
}

QpiButton *qpi_button_new(QpiGeneralPurposeIOInterface *gpio)
{
    if (gpio == NULL)
    {
        return NULL;
    }

    QpiButton *self = (QpiButton *)malloc(sizeof(QpiButton));
    if (self == NULL)
    {
        return NULL;
    }

    self->gpio = gpio;

    qpi_button_release(self);

    return self;
}

void qpi_button_delete(QpiButton *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}