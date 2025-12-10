#include "nxamf/nxmc2.h"

#include <stdlib.h>

static bool nxmc2_protocol_is_acceptable(NxamfBytesProtocolInterface *self, const uint8_t packet, const uint8_t buffer[], const size_t length)
{
    return !(11 <= length ||
             (length == 0 && packet != 0xAB) ||
             (length == 3 && 8 < packet));
}

static bool nxmc2_protocol_is_ready(NxamfBytesProtocolInterface *self, const uint8_t buffer[], const size_t length)
{
    return length == 11 &&
           buffer[0] == 0xAB &&
           buffer[3] <= 8;
}

static void nxmc2_protocol_convert(NxamfBytesProtocolInterface *self, const uint8_t buffer[], const size_t length, NxamfGamepadState *state)
{
    state->y = (buffer[1] & 0b00000001);
    state->b = (buffer[1] & 0b00000010) >> 1;
    state->a = (buffer[1] & 0b00000100) >> 2;
    state->x = (buffer[1] & 0b00001000) >> 3;
    state->l = (buffer[1] & 0b00010000) >> 4;
    state->r = (buffer[1] & 0b00100000) >> 5;
    state->zl = (buffer[1] & 0b01000000) >> 6;
    state->zr = (buffer[1] & 0b10000000) >> 7;

    state->minus = (buffer[2] & 0b00000001);
    state->plus = (buffer[2] & 0b00000010) >> 1;
    state->l_click = (buffer[2] & 0b00000100) >> 2;
    state->r_click = (buffer[2] & 0b00001000) >> 3;
    state->home = (buffer[2] & 0b00010000) >> 4;
    state->capture = (buffer[2] & 0b00100000) >> 5;
    // state->_ = (buffer[2] & 0b11000000) >> 6;

    state->hat = buffer[3];

    state->l_stick.x = buffer[4];
    state->l_stick.y = buffer[5];
    state->r_stick.x = buffer[6];
    state->r_stick.y = buffer[7];

    state->extension[0] = buffer[8];
    state->extension[1] = buffer[9];
    state->extension[2] = buffer[10];
}

Nxmc2Protocol *nxmc2_protocol_new(void)
{
    Nxmc2Protocol *self = (Nxmc2Protocol *)malloc(sizeof(Nxmc2Protocol));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.is_acceptable = nxmc2_protocol_is_acceptable;
    self->parent.is_ready = nxmc2_protocol_is_ready;
    self->parent.convert = nxmc2_protocol_convert;

    return self;
}

void nxmc2_protocol_delete(Nxmc2Protocol *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}