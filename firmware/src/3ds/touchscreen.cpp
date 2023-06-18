#include "3ds/touchscreen.h"

#include <cassert>

enum class touchscreen_state : uint8_t
{
    wait_to_be_pressed,
    wait_for_x_to_be_accepted,
    write_y,
    write_x_y,
    continue_pressing
};

void loop(touchscreen_t &ts, const touchscreen_state state, const bool powered, const double x_volt, const double y_volt)
{
    int next_us;
    auto next_state = state;
    auto next_powered = powered;
    auto next_x_volt = x_volt;
    auto next_y_volt = y_volt;

    switch (state)
    {
    case touchscreen_state::wait_to_be_pressed:
        if (!ts.pressed_)
        {
            if (powered)
            {
                ts.power_off_x();
                ts.power_off_y();
                next_powered = false;
            }
            next_us = 1;
            break;
        }

        if (!powered && ts.q_.empty())
        {
            next_us = 1;
            break;
        }

        if (!ts.q_.empty())
        {
            auto p = ts.q_.front();
            ts.q_.pop();

            next_x_volt = static_cast<double>(p.x) / 320 * 1.8;
            next_y_volt = static_cast<double>(p.y) / 240 * 1.8;
            ts.write_x(next_x_volt);
            ts.write_y(0);
            next_powered = true;
        }
        next_state = touchscreen_state::wait_for_x_to_be_accepted;
        next_us = 1;
        break;

    case touchscreen_state::wait_for_x_to_be_accepted:
        if (ts.read_x() < x_volt * 0.9)
        {
            next_us = 1;
            break;
        }
        next_state = touchscreen_state::write_y;
        next_us = 500;
        break;

    case touchscreen_state::write_y:
        ts.write_y(y_volt);
        next_state = touchscreen_state::write_x_y;
        next_us = 200;
        break;

    case touchscreen_state::write_x_y:
        ts.write_x(1.8);
        ts.write_y(1.8);
        next_state = touchscreen_state::continue_pressing;
        next_us = 200;
        break;

    case touchscreen_state::continue_pressing:
        if (!ts.q_.empty())
        {
            auto p = ts.q_.front();
            ts.q_.pop();

            next_x_volt = static_cast<double>(p.x) / 320 * 1.8;
            next_y_volt = static_cast<double>(p.y) / 240 * 1.8;
            ts.write_x(next_x_volt);
        }
        else
        {
            ts.write_x(x_volt);
        }
        ts.write_y(0);
        next_state = touchscreen_state::wait_to_be_pressed;
        next_us = 1;
        break;
    }

    ts.set_timeout_us([&ts, next_state, next_powered, next_x_volt, next_y_volt]()
                      { loop(ts, next_state, next_powered, next_x_volt, next_y_volt); },
                      next_us);
}

void touchscreen_start(touchscreen_t &ts)
{
    ts.set_timeout_us([&ts]()
                      { loop(ts, touchscreen_state::wait_to_be_pressed, true, 0, 0); },
                      1);
}

void touchscreen_press(touchscreen_t &ts, uint16_t x, uint8_t y)
{
    assert(1 <= x && x <= 320);
    assert(1 <= y && y <= 240);

    touchscreen_point_t p;
    p.x = x;
    p.y = y;
    ts.q_.push(p);
    ts.pressed_ = true;
}

void touchscreen_release(touchscreen_t &ts)
{
    ts.pressed_ = false;
}