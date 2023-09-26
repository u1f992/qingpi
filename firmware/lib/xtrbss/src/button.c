#include "xtrbss.h"

XtrBssErrNo xtrbss_button_hold(XtrBssButton *btn)
{
    if (btn == NULL)
    {
        return XTRBSS_EINVAL;
    }

    XtrBssErrNo ret = btn->gpio->set_low(btn->gpio);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}

XtrBssErrNo xtrbss_button_release(XtrBssButton *btn)
{
    if (btn == NULL)
    {
        return XTRBSS_EINVAL;
    }

    XtrBssErrNo ret = btn->gpio->set_hi_z(btn->gpio);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}

XtrBssErrNo xtrbss_button_new(XtrBssButton *btn, XtrBssGeneralPurposeIOInterface *gpio)
{
    if (btn == NULL)
    {
        return XTRBSS_EINVAL;
    }
    if (gpio == NULL)
    {
        return XTRBSS_EINVAL;
    }

    btn->gpio = gpio;

    XtrBssErrNo ret = xtrbss_button_release(btn);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}