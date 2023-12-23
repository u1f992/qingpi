#include "qingpi.h"

#include "stdlib.h"

void qpi_hat_hold(QpiHat *self, QpiHatDirection direction)
{
    if (self == NULL)
    {
        return;
    }

    switch (direction)
    {
    case QPI_HAT_UP:
        qpi_button_hold(self->up);
        qpi_button_release(self->right);
        qpi_button_release(self->down);
        qpi_button_release(self->left);
        break;
    case QPI_HAT_UPRIGHT:
        qpi_button_hold(self->up);
        qpi_button_hold(self->right);
        qpi_button_release(self->down);
        qpi_button_release(self->left);
        break;
    case QPI_HAT_RIGHT:
        qpi_button_release(self->up);
        qpi_button_hold(self->right);
        qpi_button_release(self->down);
        qpi_button_release(self->left);
        break;
    case QPI_HAT_DOWNRIGHT:
        qpi_button_release(self->up);
        qpi_button_hold(self->right);
        qpi_button_hold(self->down);
        qpi_button_release(self->left);
        break;
    case QPI_HAT_DOWN:
        qpi_button_release(self->up);
        qpi_button_release(self->right);
        qpi_button_hold(self->down);
        qpi_button_release(self->left);
        break;
    case QPI_HAT_DOWNLEFT:
        qpi_button_release(self->up);
        qpi_button_release(self->right);
        qpi_button_hold(self->down);
        qpi_button_hold(self->left);
        break;
    case QPI_HAT_LEFT:
        qpi_button_release(self->up);
        qpi_button_release(self->right);
        qpi_button_release(self->down);
        qpi_button_hold(self->left);
        break;
    case QPI_HAT_UPLEFT:
        qpi_button_hold(self->up);
        qpi_button_release(self->right);
        qpi_button_release(self->down);
        qpi_button_hold(self->left);
        break;
    case QPI_HAT_NEUTRAL:
        qpi_button_release(self->up);
        qpi_button_release(self->right);
        qpi_button_release(self->down);
        qpi_button_release(self->left);
        break;
    default:
        return;
    }
}

void qpi_hat_release(QpiHat *self)
{
    if (self == NULL)
    {
        return;
    }

    qpi_hat_hold(self, QPI_HAT_NEUTRAL);
}

QpiHat *qpi_hat_new(QpiButton *up, QpiButton *right, QpiButton *down, QpiButton *left)
{
    if (up == NULL || right == NULL || down == NULL || left == NULL)
    {
        return NULL;
    }

    QpiHat *self = (QpiHat *)malloc(sizeof(QpiHat));
    if (self == NULL)
    {
        return NULL;
    }

    self->up = up;
    self->right = right;
    self->down = down;
    self->left = left;

    qpi_hat_release(self);

    return self;
}

void qpi_hat_delete(QpiHat *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}