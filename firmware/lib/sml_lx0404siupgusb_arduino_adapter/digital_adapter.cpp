#include "sml_lx0404siupgusb_arduino_adapter.h"

static void sml_lx0404siupgusb_arduino_digital_adapter_write(SML_LX0404SIUPGUSBDigitalOutputInterface *parent, const SML_LX0404SIUPGUSBDigitalOutputState value)
{
    SML_LX0404SIUPGUSBArduinoDigitalAdapter *self = (SML_LX0404SIUPGUSBArduinoDigitalAdapter *)parent;
    if (self == NULL)
    {
        return;
    }

    digitalWrite(self->pin, value == SML_LX0404SIUPGUSB_LOW ? LOW : HIGH);
}

SML_LX0404SIUPGUSBArduinoDigitalAdapter *sml_lx0404siupgusb_arduino_digital_adapter_new(const pin_size_t pin)
{
    SML_LX0404SIUPGUSBArduinoDigitalAdapter *self = (SML_LX0404SIUPGUSBArduinoDigitalAdapter *)malloc(sizeof(SML_LX0404SIUPGUSBArduinoDigitalAdapter));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.write = sml_lx0404siupgusb_arduino_digital_adapter_write;

    self->pin = pin;

    return self;
}

extern inline void sml_lx0404siupgusb_arduino_digital_adapter_delete(SML_LX0404SIUPGUSBArduinoDigitalAdapter *self);
