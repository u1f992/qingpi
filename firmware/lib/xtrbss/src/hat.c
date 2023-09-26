#include "xtrbss.h"

XtrBssErrNo xtrbss_hat_hold(XtrBssHat *hat, XtrBssHatDirection direction)
{
    if (hat == NULL)
    {
        return XTRBSS_EINVAL;
    }

    switch (direction)
    {
    case XTRBSS_HAT_UP:
        xtrbss_button_hold(hat->up);
        xtrbss_button_release(hat->right);
        xtrbss_button_release(hat->down);
        xtrbss_button_release(hat->left);
        break;
    case XTRBSS_HAT_UPRIGHT:
        xtrbss_button_hold(hat->up);
        xtrbss_button_hold(hat->right);
        xtrbss_button_release(hat->down);
        xtrbss_button_release(hat->left);
        break;
    case XTRBSS_HAT_RIGHT:
        xtrbss_button_release(hat->up);
        xtrbss_button_hold(hat->right);
        xtrbss_button_release(hat->down);
        xtrbss_button_release(hat->left);
        break;
    case XTRBSS_HAT_DOWNRIGHT:
        xtrbss_button_release(hat->up);
        xtrbss_button_hold(hat->right);
        xtrbss_button_hold(hat->down);
        xtrbss_button_release(hat->left);
        break;
    case XTRBSS_HAT_DOWN:
        xtrbss_button_release(hat->up);
        xtrbss_button_release(hat->right);
        xtrbss_button_hold(hat->down);
        xtrbss_button_release(hat->left);
        break;
    case XTRBSS_HAT_DOWNLEFT:
        xtrbss_button_release(hat->up);
        xtrbss_button_release(hat->right);
        xtrbss_button_hold(hat->down);
        xtrbss_button_hold(hat->left);
        break;
    case XTRBSS_HAT_LEFT:
        xtrbss_button_release(hat->up);
        xtrbss_button_release(hat->right);
        xtrbss_button_release(hat->down);
        xtrbss_button_hold(hat->left);
        break;
    case XTRBSS_HAT_UPLEFT:
        xtrbss_button_hold(hat->up);
        xtrbss_button_release(hat->right);
        xtrbss_button_release(hat->down);
        xtrbss_button_hold(hat->left);
        break;
    case XTRBSS_HAT_NEUTRAL:
        xtrbss_button_release(hat->up);
        xtrbss_button_release(hat->right);
        xtrbss_button_release(hat->down);
        xtrbss_button_release(hat->left);
        break;
    default:
        return XTRBSS_EINVAL;
    }

    return XTRBSS_OK;
}

XtrBssErrNo xtrbss_hat_release(XtrBssHat *hat)
{
    if (hat == NULL)
    {
        return XTRBSS_EINVAL;
    }

    XtrBssErrNo ret = xtrbss_hat_hold(hat, XTRBSS_HAT_NEUTRAL);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}

XtrBssErrNo xtrbss_hat_new(XtrBssHat *hat, XtrBssButton *up, XtrBssButton *right, XtrBssButton *down, XtrBssButton *left)
{
    if (hat == NULL)
    {
        return XTRBSS_EINVAL;
    }
    if (up == NULL || right == NULL || down == NULL || left == NULL)
    {
        return XTRBSS_EINVAL;
    }

    hat->up = up;
    hat->right = right;
    hat->down = down;
    hat->left = left;

    XtrBssErrNo ret = xtrbss_hat_release(hat);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}