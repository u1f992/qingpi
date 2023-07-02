#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#include "3ds.h"
#include "router.h"

#define PIN_Y (0)
#define PIN_B (1)
#define PIN_A (2)
#define PIN_X (3)
#define PIN_L (4)
#define PIN_R (5)
#define PIN_START (6)
#define PIN_SELECT (7)
#define PIN_HOME (8)
#define PIN_POWER (9)
#define PIN_UP (10)
#define PIN_RIGHT (11)
#define PIN_DOWN (12)
#define PIN_LEFT (13)

#define BUTTON_(PIN, BIT)              \
    if (BIT == BUTTON_ON)              \
    {                                  \
        button_hold((button_t)PIN);    \
    }                                  \
    else                               \
    {                                  \
        button_release((button_t)PIN); \
    }
void y_handler(button_bit_t b) { BUTTON_(PIN_Y, b); }
void b_handler(button_bit_t b) { BUTTON_(PIN_B, b); }
void a_handler(button_bit_t b) { BUTTON_(PIN_A, b); }
void x_handler(button_bit_t b) { BUTTON_(PIN_X, b); }
void l_handler(button_bit_t b) { BUTTON_(PIN_L, b); }
void r_handler(button_bit_t b) { BUTTON_(PIN_R, b); }
void zl_handler(button_bit_t b) {} // Circle Pad Pro not implemented yet
void zr_handler(button_bit_t b) {} // Circle Pad Pro not implemented yet
void minus_handler(button_bit_t b) { BUTTON_(PIN_SELECT, b); }
void plus_handler(button_bit_t b) { BUTTON_(PIN_START, b); }
void l_click_handler(button_bit_t b) {}
void r_click_handler(button_bit_t b) {}
void home_handler(button_bit_t b) { BUTTON_(PIN_HOME, b); }
void capture_handler(button_bit_t b) { BUTTON_(PIN_POWER, b); }

#define HAT_(UP, RIGHT, DOWN, LEFT)          \
    if (UP)                                  \
    {                                        \
        button_hold((button_t)PIN_UP);       \
    }                                        \
    else                                     \
    {                                        \
        button_release((button_t)PIN_UP);    \
    }                                        \
    if (RIGHT)                               \
    {                                        \
        button_hold((button_t)PIN_RIGHT);    \
    }                                        \
    else                                     \
    {                                        \
        button_release((button_t)PIN_RIGHT); \
    }                                        \
    if (DOWN)                                \
    {                                        \
        button_hold((button_t)PIN_DOWN);     \
    }                                        \
    else                                     \
    {                                        \
        button_release((button_t)PIN_DOWN);  \
    }                                        \
    if (LEFT)                                \
    {                                        \
        button_hold((button_t)PIN_LEFT);     \
    }                                        \
    else                                     \
    {                                        \
        button_release((button_t)PIN_LEFT);  \
    }
void hat_handler(hat_direction_t dir)
{
    switch (dir)
    {
    case HAT_UP:
        HAT_(true, false, false, false);
        break;
    case HAT_UPRIGHT:
        HAT_(true, true, false, false);
        break;
    case HAT_RIGHT:
        HAT_(false, true, false, false);
        break;
    case HAT_DOWNRIGHT:
        HAT_(false, true, true, false);
        break;
    case HAT_DOWN:
        HAT_(false, false, true, false);
        break;
    case HAT_DOWNLEFT:
        HAT_(false, false, true, true);
        break;
    case HAT_LEFT:
        HAT_(false, false, false, true);
        break;
    case HAT_UPLEFT:
        HAT_(true, false, false, true);
        break;
    case HAT_NEUTRAL:
        HAT_(false, false, false, false);
        break;
    }
}

void stick_handler(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry)
{
    if (lx == 0x80 && ly == 0x80)
    {
        slidepad_release();
    }
    else
    {
        slidepad_hold(lx, ly);
    }
    // not implemented for right stick (Circle Pad Pro)
}
void extra_handler(uint8_t ext0, uint8_t ext1, uint8_t ext2)
{
    if ((ext0 >> 7) & 0b1 == 0)
    {
        touchscreen_release();
        return;
    }

    uint16_t x = ((uint16_t)(ext0 & 0b01111111)) << 8 | ext1;
    uint8_t y = ext2;
    if (x <= 0 || 320 < x || y <= 0 || 240 < y)
    {
        return;
    }
    touchscreen_hold(x, y);
}

static inline bool stdin_read_blocking_timeout(uint8_t *dst, size_t len, uint32_t ms)
{
    absolute_time_t start_time = get_absolute_time();
    size_t idx = 0;
    while (idx < len)
    {
        int byte = getchar_timeout_us(0);
        if (byte == PICO_ERROR_TIMEOUT) {
            if (absolute_time_diff_us(start_time, get_absolute_time()) > ms * 1000)
            {
                return false;
            }
        }
        else
        {
            dst[idx] = (uint8_t)byte;
            idx++;
        }
    }
    return true;
}

int main()
{
    bi_decl(bi_program_description("3ds-bss firmware"));

    stdio_init_all();
    touchscreen_init();

    void (*button_handlers[14])(button_bit_t) = {y_handler, b_handler, a_handler, x_handler, l_handler, r_handler, zl_handler, zr_handler, minus_handler, plus_handler, l_click_handler, r_click_handler, home_handler, capture_handler};
    router_t *r = router_init(button_handlers, hat_handler, stick_handler, extra_handler);

    uint8_t buf[10];

    while (true)
    {
        if (!stdin_read_blocking_timeout(buf, 10, 250))
        {
            continue;
        }
        router_run(r, buf);
    }
}
