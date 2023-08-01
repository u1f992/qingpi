#pragma once

#include "ad840x.h"
#include "adg752.h"
#include "g3vm_x1wr.h"

#include "utility.h"

typedef struct
{
    ad840x_t *pots;
    adg752_t *sw;
    g3vm_x1wr_t *relay;
} touchscreen_t;

/**
 * @brief Treat two potentiometers in series as a Rab=2kΩ potentiometer with 512 steps.
 *
 * @code {.unparsed}
 * |
 * A1
 * ├─W1─┐
 * B1   └─S1─┐
 * |         ├─D─
 * A2   ┌─S2─┘
 * ├─W2─┘
 * B2
 * |
 * @endcode
 *
 * @param pots
 * @param sw
 * @param addr1 A1-W1-B1
 * @param addr2 A2-W2-B2
 * @param data 0-512
 */
void _ad840x_dual_set(ad840x_t *pots, adg752_t *sw, ad840x_address_t addr1, ad840x_address_t addr2, uint16_t data)
{
    if (512 <= data)
    {
        return;
    }

    // 1基でみると0～255でワイパーは下から上に動く（Vw出力が0～Vaになる）
    // 2基の場合、前半の値は下、後半の値は上のポテンショメータから出力する
    if (data < 256)
    {
        adg752_s2(sw);
        ad840x_set(pots, addr2, data);
    }
    else
    {
        adg752_s1(sw);
        ad840x_set(pots, addr1, data - 256);
    }
}

void touchscreen_hold(touchscreen_t *ts, uint16_t x, uint8_t y)
{
    if (x < 1 || 340 < x || y < 1 || 240 < y)
    {
        return;
    }
    _ad840x_dual_set(ts->pots, ts->sw, AD840X_RDAC1, AD840X_RDAC3, rescale(x, 1, 340, 0, 511));
    ad840x_set(ts->pots, AD840X_RDAC2, rescale(y, 1, 240, 0, 255));
    ad840x_power_on(ts->pots);
    g3vm_x1wr_on(ts->relay);
}

void touchscreen_release(touchscreen_t *ts)
{
    g3vm_x1wr_off(ts->relay);
    ad840x_shutdown(ts->pots);
}

void touchscreen_init(touchscreen_t *ts, ad840x_t *pots, adg752_t *sw, g3vm_x1wr_t *relay)
{
    ts->pots = pots;
    ts->sw = sw;
    ts->relay = relay;
    touchscreen_release(ts);
}