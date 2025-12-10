#include "nxamf.h"

#include <stdlib.h>

void nxamf_gamepad_state_delete(NxamfGamepadState *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}