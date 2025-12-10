#include "nxamf.h"

#include <stdlib.h>

static bool nxamf_protocol_multiplexer_is_acceptable(NxamfBytesProtocolInterface *parent, const uint8_t packet, const uint8_t buffer[], const size_t length)
{
    NxamfProtocolMultiplexer *self = (NxamfProtocolMultiplexer *)parent;

    for (int i = 0; i < self->protocols_length; i++)
    {
        NxamfBytesProtocolInterface *p = self->protocols[i];
        if (p == NULL)
        {
            continue;
        }

        if (p->is_acceptable(p, packet, buffer, length))
        {
            return true;
        }
    }
    return false;
}

static bool nxamf_protocol_multiplexer_is_ready(NxamfBytesProtocolInterface *parent, const uint8_t buffer[], const size_t length)
{
    NxamfProtocolMultiplexer *self = (NxamfProtocolMultiplexer *)parent;

    for (int i = 0; i < self->protocols_length; i++)
    {
        NxamfBytesProtocolInterface *p = self->protocols[i];
        if (p == NULL)
        {
            continue;
        }

        if (p->is_ready(p, buffer, length))
        {
            self->ready_index = i;
            return true;
        }
    }
    return false;
}

static void nxamf_protocol_multiplexer_convert(NxamfBytesProtocolInterface *parent, const uint8_t buffer[], const size_t length, NxamfGamepadState *state)
{
    NxamfProtocolMultiplexer *self = (NxamfProtocolMultiplexer *)parent;

    if (self->protocols_length == 0)
    {
        return;
    }

    NxamfBytesProtocolInterface *p = self->protocols[self->ready_index];
    if (p == NULL)
    {
        return;
    }

    p->convert(p, buffer, length, state);
}

NxamfProtocolMultiplexer *nxamf_protocol_multiplexer_new(NxamfBytesProtocolInterface *protocols[], size_t length)
{
    NxamfProtocolMultiplexer *self = (NxamfProtocolMultiplexer *)malloc(sizeof(NxamfProtocolMultiplexer));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.is_acceptable = nxamf_protocol_multiplexer_is_acceptable;
    self->parent.is_ready = nxamf_protocol_multiplexer_is_ready;
    self->parent.convert = nxamf_protocol_multiplexer_convert;

    self->protocols = protocols;
    self->protocols_length = length;
    self->ready_index = 0;

    return self;
}

void nxamf_protocol_multiplexer_delete(NxamfProtocolMultiplexer *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}