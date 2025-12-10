#include "nxamf.h"

typedef struct MockProtocol
{
    NxamfBytesProtocolInterface parent;
} MockProtocol;

bool mock_protocol_is_acceptable(NxamfBytesProtocolInterface *, const uint8_t, const uint8_t[], const size_t length)
{
    return length < 3;
}

bool mock_protocol_is_ready(NxamfBytesProtocolInterface *, const uint8_t[], const size_t length)
{
    return length == 3;
}

void mock_protocol_convert(NxamfBytesProtocolInterface *, const uint8_t buffer[], const size_t, NxamfGamepadState *state)
{
    state->extension[0] = buffer[0];
    state->extension[1] = buffer[1];
    state->extension[2] = buffer[2];
}

MockProtocol *mock_protocol_new(void)
{
    MockProtocol *self = (MockProtocol *)malloc(sizeof(MockProtocol));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.is_acceptable = mock_protocol_is_acceptable;
    self->parent.is_ready = mock_protocol_is_ready;
    self->parent.convert = mock_protocol_convert;

    return self;
}

void mock_protocol_delete(MockProtocol *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}