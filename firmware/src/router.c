#include "router.h"

router_t *router_init(
    void (*button_handlers[14])(button_bit_t),
    void (*hat_handler)(hat_direction_t),
    void (*stick_handler)(uint8_t, uint8_t, uint8_t, uint8_t),
    void (*extra_handler)(uint8_t, uint8_t, uint8_t))
{
    router_t *r = (router_t *)malloc(sizeof(router_t));
    for (int i = 0; i < 14; i++)
    {
        r->button_handlers[i] = button_handlers[i];
    }
    r->hat_handler = hat_handler;
    r->stick_handler = stick_handler;
    r->extra_handler = extra_handler;
    return r;
}

void router_run(router_t *r, uint8_t *data)
{
    assert(data[0] == 0xAB);
    assert((((uint16_t)data[2]) << 8 | data[1]) <= 0x3FFF);
    assert(data[3] <= 8);

    uint16_t btn = ((uint16_t)data[2]) << 8 | data[1];
    for (int i = 13; i >= 0; i--)
    {
        r->button_handlers[i]((button_bit_t)((btn >> i) & 0b1));
    }
    r->hat_handler((hat_direction_t)data[3]);
    r->stick_handler(data[4], data[5], data[6], data[7]);
    r->extra_handler(data[8], data[9], data[10]);
}