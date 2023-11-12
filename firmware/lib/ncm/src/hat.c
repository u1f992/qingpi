#include "ncm.h"

#include "stdlib.h"

void ncm_hat_hold(NcmHat *self, NcmHatDirection direction)
{
    if (self == NULL)
    {
        return;
    }

    switch (direction)
    {
    case NCM_HAT_UP:
        ncm_button_hold(self->up);
        ncm_button_release(self->right);
        ncm_button_release(self->down);
        ncm_button_release(self->left);
        break;
    case NCM_HAT_UPRIGHT:
        ncm_button_hold(self->up);
        ncm_button_hold(self->right);
        ncm_button_release(self->down);
        ncm_button_release(self->left);
        break;
    case NCM_HAT_RIGHT:
        ncm_button_release(self->up);
        ncm_button_hold(self->right);
        ncm_button_release(self->down);
        ncm_button_release(self->left);
        break;
    case NCM_HAT_DOWNRIGHT:
        ncm_button_release(self->up);
        ncm_button_hold(self->right);
        ncm_button_hold(self->down);
        ncm_button_release(self->left);
        break;
    case NCM_HAT_DOWN:
        ncm_button_release(self->up);
        ncm_button_release(self->right);
        ncm_button_hold(self->down);
        ncm_button_release(self->left);
        break;
    case NCM_HAT_DOWNLEFT:
        ncm_button_release(self->up);
        ncm_button_release(self->right);
        ncm_button_hold(self->down);
        ncm_button_hold(self->left);
        break;
    case NCM_HAT_LEFT:
        ncm_button_release(self->up);
        ncm_button_release(self->right);
        ncm_button_release(self->down);
        ncm_button_hold(self->left);
        break;
    case NCM_HAT_UPLEFT:
        ncm_button_hold(self->up);
        ncm_button_release(self->right);
        ncm_button_release(self->down);
        ncm_button_hold(self->left);
        break;
    case NCM_HAT_NEUTRAL:
        ncm_button_release(self->up);
        ncm_button_release(self->right);
        ncm_button_release(self->down);
        ncm_button_release(self->left);
        break;
    default:
        return;
    }
}

void ncm_hat_release(NcmHat *self)
{
    if (self == NULL)
    {
        return;
    }

    ncm_hat_hold(self, NCM_HAT_NEUTRAL);
}

NcmHat *ncm_hat_new(NcmButton *up, NcmButton *right, NcmButton *down, NcmButton *left)
{
    if (up == NULL || right == NULL || down == NULL || left == NULL)
    {
        return NULL;
    }

    NcmHat *self = (NcmHat *)malloc(sizeof(NcmHat));
    if (self == NULL)
    {
        return NULL;
    }

    self->up = up;
    self->right = right;
    self->down = down;
    self->left = left;

    ncm_hat_release(self);

    return self;
}

void ncm_hat_delete(NcmHat *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}