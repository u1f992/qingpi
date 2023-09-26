#include "xtrbss.h"

XtrBssErrNo xtrbss_touchscreen_hold(XtrBssTouchScreen *ts, double x, double y)
{
    if (ts == NULL)
    {
        return XTRBSS_EINVAL;
    }
    if (x < 0.0 || 1.0 < x || y < 0.0 || 1.0 < y)
    {
        return XTRBSS_EINVAL;
    }

    XtrBssErrNo ret = ts->vertical->set_wiper_position(ts->vertical, y);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }
    ret = ts->vertical->power_on(ts->vertical);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    ret = ts->horizontal->set_wiper_position(ts->horizontal, x);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }
    ret = ts->horizontal->power_on(ts->horizontal);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    ret = ts->relay->energize(ts->relay);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}

XtrBssErrNo xtrbss_touchscreen_release(XtrBssTouchScreen *ts)
{
    if (ts == NULL)
    {
        return XTRBSS_EINVAL;
    }

    XtrBssErrNo ret = ts->relay->de_energize(ts->relay);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    ret = ts->vertical->shutdown(ts->vertical);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }
    ret = ts->horizontal->shutdown(ts->horizontal);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}

XtrBssErrNo xtrbss_touchscreen_new(XtrBssTouchScreen *ts, XtrBssDigitalPotentiometerInterface *vertical, XtrBssDigitalPotentiometerInterface *horizontal, XtrBssRelayInterface *relay)
{
    if (ts == NULL)
    {
        return XTRBSS_EINVAL;
    }
    if (vertical == NULL || horizontal == NULL || relay == NULL)
    {
        return XTRBSS_EINVAL;
    }

    ts->vertical = vertical;
    ts->horizontal = horizontal;
    ts->relay = relay;

    XtrBssErrNo ret = xtrbss_touchscreen_release(ts);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}