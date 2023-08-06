#pragma once

#include "ad840x.h"
#include "g3vm_x1wr.h"

#include "utility.h"

typedef struct
{
    ad840x_t *pots;
    g3vm_x1wr_t *relay;
} touchscreen_t;

/**
 * @brief 
 * 
 * @code {.unparsed}
 * │
 * U1─┐
 * │  │
 * x  │
 *    U3─
 * x  │
 * │  │
 * U2─┘
 * │
 * @endcode
 * 
 * @param pots 
 * @param addr1 
 * @param addr2 
 * @param addr3 
 * @param data 
 */
void _ad840x_triple_set(ad840x_t *pots, ad840x_address_t addr1, ad840x_address_t addr2, ad840x_address_t addr3, uint16_t data)
{
    if (511 <= data)
    {
        return;
    }

    if (data < 256)
    {
        ad840x_set(pots, addr1, 0);
        ad840x_set(pots, addr2, 0);
        ad840x_set(pots, addr3, data);
    }
    else
    {
        ad840x_set(pots, addr1, data - 255);
        ad840x_set(pots, addr2, data - 255);
        ad840x_set(pots, addr3, 255);
    }
}

void touchscreen_hold(touchscreen_t *ts, uint16_t x, uint8_t y)
{
    if (x < 1 || 320 < x || y < 1 || 240 < y)
    {
        return;
    }
    _ad840x_triple_set(ts->pots, AD840X_RDAC1, AD840X_RDAC3, AD840X_RDAC4, rescale(x, 1, 320, 0, 510));
    ad840x_set(ts->pots, AD840X_RDAC2, rescale(y, 1, 240, 0, 255));
    ad840x_power_on(ts->pots);
    g3vm_x1wr_on(ts->relay);
}

void touchscreen_release(touchscreen_t *ts)
{
    g3vm_x1wr_off(ts->relay);
    ad840x_shutdown(ts->pots);
}

void touchscreen_init(touchscreen_t *ts, ad840x_t *pots, g3vm_x1wr_t *relay)
{
    ts->pots = pots;
    ts->relay = relay;
    touchscreen_release(ts);
}