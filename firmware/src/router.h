#pragma once
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    BUTTON_OFF,
    BUTTON_ON
} button_bit_t;

typedef enum
{
    HAT_UP,
    HAT_UPRIGHT,
    HAT_RIGHT,
    HAT_DOWNRIGHT,
    HAT_DOWN,
    HAT_DOWNLEFT,
    HAT_LEFT,
    HAT_UPLEFT,
    HAT_NEUTRAL
} hat_direction_t;

typedef struct
{
    void (*button_handlers[14])(button_bit_t);
    void (*hat_handler)(hat_direction_t);
    void (*stick_handler)(uint8_t, uint8_t, uint8_t, uint8_t);
    void (*extra_handler)(uint8_t, uint8_t, uint8_t);
} router_t;

router_t *router_init(
    void (*button_handlers[14])(button_bit_t),
    void (*hat_handler)(hat_direction_t),
    void (*stick_handler)(uint8_t, uint8_t, uint8_t, uint8_t),
    void (*extra_handler)(uint8_t, uint8_t, uint8_t));
void router_run(router_t *r, uint8_t *data);