#include "3ds/slidepad.h"

void slidepad_hold(const slidepad_t &sp, const uint8_t x, const uint8_t y)
{
    sp.write_x(1.8 * static_cast<double>(x) / 255);
    sp.write_y(1.8 * static_cast<double>(y) / 255);
}

void slidepad_release(const slidepad_t &sp)
{
    sp.power_off_x();
    sp.power_off_y();
}