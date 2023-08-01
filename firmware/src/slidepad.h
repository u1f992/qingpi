#pragma once

#include "mcp4922.h"

#include "utility.h"

typedef struct
{
    mcp4922_t *dac;
} slidepad_t;

void slidepad_hold(slidepad_t *sp, uint8_t x, uint8_t y)
{
    mcp4922_latch(sp->dac);
    mcp4922_set(sp->dac, MCP4922_DAC_A, MCP4922_BUFFERED, MCP4922_1X, MCP4922_ACTIVE, rescale(x, 0, 255, 0, 4095));
    mcp4922_set(sp->dac, MCP4922_DAC_B, MCP4922_BUFFERED, MCP4922_1X, MCP4922_ACTIVE, rescale(y, 0, 255, 0, 4095));
    mcp4922_unlatch(sp->dac);
    mcp4922_power_on(sp->dac);
}

void slidepad_release(slidepad_t *sp)
{
    mcp4922_shutdown(sp->dac);
}

void slidepad_init(slidepad_t *sp, mcp4922_t *dac)
{
    sp->dac = dac;
    slidepad_release(sp);
}