#include "nxamf.h"

#include <memory.h>
#include <stdlib.h>

NxamfBytesBuffer *nxamf_bytes_buffer_new(NxamfBytesProtocolInterface *protocol)
{
    NxamfBytesBuffer *self = (NxamfBytesBuffer *)malloc(sizeof(NxamfBytesBuffer));
    if (self == NULL)
    {
        return NULL;
    }

    self->protocol = protocol;
    memset(self->buffer, 0, 64);
    self->length = 0;

    return self;
}

void nxamf_bytes_buffer_delete(NxamfBytesBuffer *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}

NxamfGamepadState *nxamf_bytes_buffer_append(NxamfBytesBuffer *self, const uint8_t packet)
{
    if (self == NULL)
    {
        return NULL;
    }

    if (!self->protocol->is_acceptable(self->protocol, packet, self->buffer, self->length))
    {
        // rejected
        nxamf_bytes_buffer_clear(self);
        if (!self->protocol->is_acceptable(self->protocol, packet, self->buffer, self->length))
        {
            return NULL;
        }
    }

    self->buffer[self->length] = packet;
    self->length++;

    if (!self->protocol->is_ready(self->protocol, self->buffer, self->length))
    {
        return NULL;
    }

    NxamfGamepadState *state = (NxamfGamepadState *)calloc(1, sizeof(NxamfGamepadState));
    if (state == NULL)
    {
        return NULL;
    }
    state->hat = NXAMF_HAT_STATE_NEUTRAL;
    state->l_stick.x = NXAMF_STICK_STATE_NEUTRAL;
    state->l_stick.y = NXAMF_STICK_STATE_NEUTRAL;
    state->r_stick.x = NXAMF_STICK_STATE_NEUTRAL;
    state->r_stick.y = NXAMF_STICK_STATE_NEUTRAL;
    self->protocol->convert(self->protocol, self->buffer, self->length, state);
    return state;
}

void nxamf_bytes_buffer_clear(NxamfBytesBuffer *self)
{
    if (self == NULL)
    {
        return;
    }

    self->length = 0;
}