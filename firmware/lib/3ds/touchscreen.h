#ifndef _3DS_TOUCHSCREEN_H_
#define _3DS_TOUCHSCREEN_H_

#include "ad840x.h"
#include "g3vm_xwr.h"

#include "common.h"

typedef struct
{
    ad840x_t *pots_;
    g3vm_xwr_t *relay_;
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
inline void ad840x_triple_set_(ad840x_t *pots, ad840x_address_t addr1, ad840x_address_t addr2, ad840x_address_t addr3, uint16_t data)
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

inline void touchscreen_hold(touchscreen_t *ts, uint16_t x, uint8_t y)
{
    if (x < 1 || 320 < x || y < 1 || 240 < y)
    {
        return;
    }
    ad840x_triple_set_(ts->pots_, AD840X_RDAC1, AD840X_RDAC3, AD840X_RDAC4, rescale_(x, 1, 320, 0, 510));
    ad840x_set(ts->pots_, AD840X_RDAC2, rescale_(y, 1, 240, 0, 255));
    ad840x_power_on(ts->pots_);
    g3vm_xwr_on(ts->relay_);
}

inline void touchscreen_release(touchscreen_t *ts)
{
    g3vm_xwr_off(ts->relay_);
    ad840x_shutdown(ts->pots_);
}

inline void touchscreen_init(touchscreen_t *ts, ad840x_t *pots, g3vm_xwr_t *relay)
{
    ts->pots_ = pots;
    ts->relay_ = relay;
    touchscreen_release(ts);
}

#endif