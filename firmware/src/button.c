#include "3ds.h"

static inline void write_low(uint pin)
{

}

static inline void power_off(uint pin)
{

}

void button_hold(button_t btn)
{
    write_low((uint)btn);
}
void button_release(button_t btn)
{
    power_off((uint)btn);
}