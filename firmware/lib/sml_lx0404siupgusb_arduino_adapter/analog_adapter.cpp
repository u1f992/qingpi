#include "sml_lx0404siupgusb_arduino_adapter.h"

#include <assert.h>

static void sml_lx0404siupgusb_arduino_analog_adapter_write(SML_LX0404SIUPGUSBAnalogOutputInterface *parent, const uint16_t value)
{
    SML_LX0404SIUPGUSBArduinoAnalogAdapter *self = (SML_LX0404SIUPGUSBArduinoAnalogAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    int val = (int)(((double)value / UINT16_MAX) * self->max_value);
    analogWrite(self->pin, val);
}

SML_LX0404SIUPGUSBArduinoAnalogAdapter *sml_lx0404siupgusb_arduino_analog_adapter_new(const pin_size_t pin, const int resolution)
{
    // Since analogWrite is defined to take int as an argument, the 31-bit limit would be fine.
    // Aside from whether an Arduino board with 31-bit analogWrite exists.
    // https://github.com/arduino/ArduinoCore-API/blob/82a5055a0588976c8df8c1ff3d978f62d68410f3/api/Common.h#L101
    if (resolution < 1 || 31 < resolution)
    {
        return NULL;
    }

    SML_LX0404SIUPGUSBArduinoAnalogAdapter *self = (SML_LX0404SIUPGUSBArduinoAnalogAdapter *)malloc(sizeof(SML_LX0404SIUPGUSBArduinoAnalogAdapter));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.write = sml_lx0404siupgusb_arduino_analog_adapter_write;

    self->pin = pin;
    int max_value = (1UL << resolution) - 1;
    // assert(1 <= max_value && max_value <= INT32_MAX);
    self->max_value = max_value;

    return self;
}

extern inline void sml_lx0404siupgusb_arduino_analog_adapter_delete(SML_LX0404SIUPGUSBArduinoAnalogAdapter *self);
