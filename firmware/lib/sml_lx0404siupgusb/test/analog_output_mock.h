#include "sml_lx0404siupgusb.h"

#include <stdlib.h>

typedef struct AnalogOutputMock
{
    SML_LX0404SIUPGUSBAnalogOutputInterface parent;
    uint16_t value;
} AnalogOutputMock;

static void analog_output_mock_write(SML_LX0404SIUPGUSBAnalogOutputInterface *parent, const uint16_t value)
{
    AnalogOutputMock *self = (AnalogOutputMock *)parent;
    if (self == NULL)
    {
        return;
    }

    self->value = value;
}

AnalogOutputMock *analog_output_mock_new(const uint16_t initial_value)
{
    AnalogOutputMock *self = (AnalogOutputMock *)malloc(sizeof(AnalogOutputMock));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.write = analog_output_mock_write;

    self->value = initial_value;

    return self;
}

void analog_output_mock_delete(AnalogOutputMock *self)
{
    free(self);
}
