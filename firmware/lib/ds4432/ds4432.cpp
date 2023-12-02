#include "ds4432.h"

void ds4432_set(DS4432 *self, DS4432MemoryAddress addr, DS4432SignBit sign, uint8_t data)
{
    if (0b0111'1111 < data)
    {
        return;
    }

    uint8_t d = ((uint8_t)sign << 7) | data;

    self->wire->beginTransmission(DS4432_ADDRESS);
    self->wire->write((uint8_t)addr);
    self->wire->write(d);
    self->wire->endTransmission();
}

DS4432 *ds4432_new(HardwareI2C *wire)
{
    if (wire == NULL)
    {
        return NULL;
    }

    DS4432 *self = (DS4432 *)malloc(sizeof(DS4432));
    if (self == NULL)
    {
        return NULL;
    }

    self->wire = wire;

    ds4432_set(self, DS4432_OUT0, DS4432_SOURCE, 0);
    ds4432_set(self, DS4432_OUT1, DS4432_SOURCE, 0);

    return self;
}

void ds4432_delete(DS4432 *self)
{
    if (self == NULL)
    {
        return;
    }

    ds4432_set(self, DS4432_OUT0, DS4432_SOURCE, 0);
    ds4432_set(self, DS4432_OUT1, DS4432_SOURCE, 0);

    free(self);
}