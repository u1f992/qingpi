#include "3ds.h"

static inline void write_x(const double val)
{

}
static inline void write_y(const double val)
{

}
static inline void power_off_x()
{

}
static inline void power_off_y()
{

}

void slidepad_hold(uint8_t x, uint8_t y)
{
    write_x(((double)x) / 255 * _3DS_LOGIC_LEVEL);
    write_y(((double)y) / 255 * _3DS_LOGIC_LEVEL);
}
void slidepad_release()
{
    power_off_x();
    power_off_y();
}