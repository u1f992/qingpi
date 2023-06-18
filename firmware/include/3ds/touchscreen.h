#pragma once

#include <cstdint>
#include <functional>
#include <queue>

typedef struct
{
    uint16_t x;
    uint8_t y;
} touchscreen_point_t;

typedef struct
{
    std::function<void(double)> write_x;
    std::function<void(double)> write_y;
    std::function<double()> read_x;
    std::function<void()> power_off_x;
    std::function<void()> power_off_y;
    std::function<void(std::function<void()>, int)> set_timeout_us;

    bool pressed_ = false;
    std::queue<touchscreen_point_t> q_;
} touchscreen_t;

extern void touchscreen_start(touchscreen_t &);
extern void touchscreen_press(touchscreen_t &, uint16_t, uint8_t);
extern void touchscreen_release(touchscreen_t &);