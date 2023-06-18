#pragma once

#include <cstdint>
#include <functional>
#include <queue>

enum class touchscreen_state : uint8_t
{
    wait_to_be_pressed,
    start_pressing,
    wait_for_x_to_be_accepted,
    write_y,
    write_x_y
};

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

    touchscreen_state state_ = touchscreen_state::wait_to_be_pressed;
    bool pressed_ = false;
    std::queue<touchscreen_point_t> q_;
    double x_volt_;
    double y_volt_;

} touchscreen_t;

extern void touchscreen_start(touchscreen_t &);
void touchscreen_press(touchscreen_t &, uint16_t, uint8_t);
void touchscreen_release(touchscreen_t &);