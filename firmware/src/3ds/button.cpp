#include "3ds/button.h"

void button_press(const button_t &btn)
{
    btn.write_digital_low();
}

void button_release(const button_t &btn)
{
    btn.power_off();
}