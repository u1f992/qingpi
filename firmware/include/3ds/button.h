#pragma once

#include <functional>

typedef struct
{
    std::function<void()> write_digital_low;
    std::function<void()> power_off;
} button_t;

extern void button_press(const button_t &);
extern void button_release(const button_t &);