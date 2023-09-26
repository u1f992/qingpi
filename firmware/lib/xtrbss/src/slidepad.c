#include "xtrbss.h"

XtrBssErrNo xtrbss_slidepad_hold(XtrBssSlidePad *sp, double x, double y)
{
    if (sp == NULL)
    {
        return XTRBSS_EINVAL;
    }
    if (x < 0.0 || 1.0 < x || y < 0.0 || 1.0 < y)
    {
        return XTRBSS_EINVAL;
    }

    XtrBssErrNo ret;
    if (x == XTRBSS_SLIDEPAD_NEUTRAL)
    {
        ret = sp->horizontal->source(sp->horizontal, 0.0);
    }
    else if (XTRBSS_SLIDEPAD_NEUTRAL < x)
    {
        // 0.5-1.0
        // => 0.0-1.0
        ret = sp->horizontal->source(sp->horizontal, 2.0 * x - 1.0);
    }
    else
    {
        // 0.0-0.5
        // => 1.0-0.0
        ret = sp->horizontal->sink(sp->horizontal, 1.0 - 2.0 * x);
    }
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    if (y == XTRBSS_SLIDEPAD_NEUTRAL)
    {
        ret = sp->vertical->source(sp->vertical, 0.0);
    }
    else if (XTRBSS_SLIDEPAD_NEUTRAL < y)
    {
        ret = sp->vertical->source(sp->vertical, 2.0 * y - 1.0);
    }
    else
    {
        ret = sp->vertical->sink(sp->vertical, 1.0 - 2.0 * y);
    }
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}

XtrBssErrNo xtrbss_slidepad_release(XtrBssSlidePad *sp)
{
    if (sp == NULL)
    {
        return XTRBSS_EINVAL;
    }

    XtrBssErrNo ret = xtrbss_slidepad_hold(sp, XTRBSS_SLIDEPAD_NEUTRAL, XTRBSS_SLIDEPAD_NEUTRAL);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}

XtrBssErrNo xtrbss_slidepad_new(XtrBssSlidePad *sp, XtrBssCurrentDAConverterInterface *vertical, XtrBssCurrentDAConverterInterface *horizontal)
{
    if (sp == NULL)
    {
        return XTRBSS_EINVAL;
    }
    if (vertical == NULL || horizontal == NULL)
    {
        return XTRBSS_EINVAL;
    }

    sp->vertical = vertical;
    sp->horizontal = horizontal;

    XtrBssErrNo ret = xtrbss_slidepad_release(sp);
    if (ret != XTRBSS_OK)
    {
        return ret;
    }

    return XTRBSS_OK;
}