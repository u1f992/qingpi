#pragma once

#include <cstdint>
#include <functional>

typedef struct
{
    std::function<void(double)> write_x;
    std::function<void(double)> write_y;
    std::function<void()> power_off_x;
    std::function<void()> power_off_y;
} slidepad_t;

extern void slidepad_hold(const slidepad_t &, const uint8_t, const uint8_t);
extern void slidepad_release(const slidepad_t &);