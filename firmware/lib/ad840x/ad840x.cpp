#include "ad840x.h"

void ad840x_set(AD840X *self, AD840XAddress addr, uint8_t data)
{
    if (self == NULL)
    {
        return;
    }

    uint16_t d = (((uint8_t)addr) << 8) | data;

    digitalWrite(self->cs, LOW);
    self->spi->transfer16(d);
    digitalWrite(self->cs, HIGH);
}

void ad840x_shutdown(AD840X *self)
{
    if (self == NULL)
    {
        return;
    }

    digitalWrite(self->shdn, LOW);
}

void ad840x_power_on(AD840X *self)
{
    if (self == NULL)
    {
        return;
    }

    digitalWrite(self->shdn, HIGH);
}

AD840X *ad840x_new(HardwareSPI *spi, pin_size_t cs, pin_size_t shdn)
{
    if (spi == NULL)
    {
        return NULL;
    }

    AD840X *self = (AD840X *)malloc(sizeof(AD840X));
    if (self == NULL)
    {
        return NULL;
    }

    self->spi = spi;
    self->cs = cs;
    self->shdn = shdn;

    ad840x_power_on(self);
    ad840x_set(self, AD840X_RDAC1, 0);
    ad840x_set(self, AD840X_RDAC2, 0);
    ad840x_set(self, AD840X_RDAC3, 0);
    ad840x_set(self, AD840X_RDAC4, 0);
    ad840x_shutdown(self);

    return self;
}

void ad840x_delete(AD840X *self)
{
    if (self == NULL)
    {
        return;
    }

    ad840x_power_on(self);
    ad840x_set(self, AD840X_RDAC1, 0);
    ad840x_set(self, AD840X_RDAC2, 0);
    ad840x_set(self, AD840X_RDAC3, 0);
    ad840x_set(self, AD840X_RDAC4, 0);
    ad840x_shutdown(self);

    free(self);
}