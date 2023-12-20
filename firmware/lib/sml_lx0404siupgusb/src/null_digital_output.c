#include "sml_lx0404siupgusb.h"

#include <stdlib.h>

static void sml_lx0404siupgusb_null_digital_output_write(SML_LX0404SIUPGUSBDigitalOutputInterface *parent, const SML_LX0404SIUPGUSBDigitalOutputState value)
{
    return;
}

SML_LX0404SIUPGUSBNullDigitalOutput *sml_lx0404siupgusb_null_digital_output_new(void)
{
    SML_LX0404SIUPGUSBNullDigitalOutput *self = (SML_LX0404SIUPGUSBNullDigitalOutput *)malloc(sizeof(SML_LX0404SIUPGUSBNullDigitalOutput));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.write = sml_lx0404siupgusb_null_digital_output_write;

    return self;
}

extern inline void sml_lx0404siupgusb_null_digital_output_delete(SML_LX0404SIUPGUSBNullDigitalOutput *self);
