#pragma once

#include "button.h"

typedef struct
{
    button_t *up;
    button_t *right;
    button_t *down;
    button_t *left;
} hat_t;

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

void hat_hold(hat_t *hat, hat_direction_t dir)
{
    switch (dir)
    {
    case HAT_UP:
        button_hold(hat->up);
        button_release(hat->right);
        button_release(hat->down);
        button_release(hat->left);
        break;
    case HAT_UPRIGHT:
        button_hold(hat->up);
        button_hold(hat->right);
        button_release(hat->down);
        button_release(hat->left);
        break;
    case HAT_RIGHT:
        button_release(hat->up);
        button_hold(hat->right);
        button_release(hat->down);
        button_release(hat->left);
        break;
    case HAT_DOWNRIGHT:
        button_release(hat->up);
        button_hold(hat->right);
        button_hold(hat->down);
        button_release(hat->left);
        break;
    case HAT_DOWN:
        button_release(hat->up);
        button_release(hat->right);
        button_hold(hat->down);
        button_release(hat->left);
        break;
    case HAT_DOWNLEFT:
        button_release(hat->up);
        button_release(hat->right);
        button_hold(hat->down);
        button_hold(hat->left);
        break;
    case HAT_LEFT:
        button_release(hat->up);
        button_release(hat->right);
        button_release(hat->down);
        button_hold(hat->left);
        break;
    case HAT_UPLEFT:
        button_hold(hat->up);
        button_release(hat->right);
        button_release(hat->down);
        button_hold(hat->left);
        break;
    case HAT_NEUTRAL:
    default:
        button_release(hat->up);
        button_release(hat->right);
        button_release(hat->down);
        button_release(hat->left);
        break;
    }
}

void hat_release(hat_t *hat)
{
    hat_hold(hat, HAT_NEUTRAL);
}

void hat_init(hat_t *hat, button_t *up, button_t *right, button_t *down, button_t *left)
{
    hat->up = up;
    hat->right = right;
    hat->down = down;
    hat->left = left;
    hat_release(hat);
}