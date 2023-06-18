#include "3ds/touchscreen.h"

void loop(touchscreen_t &ts)
{
    int next_us;

    switch (ts.state_)
    {
    case touchscreen_state::wait_to_be_pressed:
        if (!ts.pressed_)
        {
            ts.power_off_x();
            ts.power_off_y();
            next_us = 1;
            break;
        }
        ts.state_ = touchscreen_state::start_pressing;
        next_us = 1;
        break;

    case touchscreen_state::start_pressing:
        if (!ts.q_.empty())
        {
            auto p = ts.q_.front();
            ts.q_.pop();

            ts.x_volt_ = static_cast<double>(p.x) / 320 * 1.8;
            ts.y_volt_ = static_cast<double>(p.y) / 240 * 1.8;
        }
        ts.write_x(ts.x_volt_);
        ts.write_y(0);
        ts.state_ = touchscreen_state::wait_for_x_to_be_accepted;
        next_us = 1;
        break;

    case touchscreen_state::wait_for_x_to_be_accepted:
        if (ts.read_x() < ts.x_volt_ * 0.9)
        {
            next_us = 1;
            break;
        }
        ts.state_ = touchscreen_state::write_y;
        next_us = 500;
        break;

    case touchscreen_state::write_y:
        ts.write_y(ts.y_volt_);
        ts.state_ = touchscreen_state::write_x_y;
        next_us = 200;
        break;

    case touchscreen_state::write_x_y:
        ts.write_x(1.8);
        ts.write_y(1.8);
        ts.state_ = touchscreen_state::wait_to_be_pressed;
        next_us = 200;
        break;
    }

    ts.set_timeout_us([&ts]()
                      { loop(ts); },
                      next_us);
}

void touchscreen_start(touchscreen_t &ts)
{
    ts.set_timeout_us([&ts]()
                      { loop(ts); },
                      1);
}

void touchscreen_press(touchscreen_t &ts, uint16_t x, uint8_t y)
{
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