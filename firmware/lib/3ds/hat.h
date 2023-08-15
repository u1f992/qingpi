#ifndef _3DS_HAT_H_
#define _3DS_HAT_H_

#include "button.h"

typedef struct
{
    button_t *up_;
    button_t *right_;
    button_t *down_;
    button_t *left_;
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

inline void hat_hold(hat_t *hat, hat_direction_t dir)
{
    switch (dir)
    {
    case HAT_UP:
        button_hold(hat->up_);
        button_release(hat->right_);
        button_release(hat->down_);
        button_release(hat->left_);
        break;
    case HAT_UPRIGHT:
        button_hold(hat->up_);
        button_hold(hat->right_);
        button_release(hat->down_);
        button_release(hat->left_);
        break;
    case HAT_RIGHT:
        button_release(hat->up_);
        button_hold(hat->right_);
        button_release(hat->down_);
        button_release(hat->left_);
        break;
    case HAT_DOWNRIGHT:
        button_release(hat->up_);
        button_hold(hat->right_);
        button_hold(hat->down_);
        button_release(hat->left_);
        break;
    case HAT_DOWN:
        button_release(hat->up_);
        button_release(hat->right_);
        button_hold(hat->down_);
        button_release(hat->left_);
        break;
    case HAT_DOWNLEFT:
        button_release(hat->up_);
        button_release(hat->right_);
        button_hold(hat->down_);
        button_hold(hat->left_);
        break;
    case HAT_LEFT:
        button_release(hat->up_);
        button_release(hat->right_);
        button_release(hat->down_);
        button_hold(hat->left_);
        break;
    case HAT_UPLEFT:
        button_hold(hat->up_);
        button_release(hat->right_);
        button_release(hat->down_);
        button_hold(hat->left_);
        break;
    case HAT_NEUTRAL:
    default:
        button_release(hat->up_);
        button_release(hat->right_);
        button_release(hat->down_);
        button_release(hat->left_);
        break;
    }
}

inline void hat_release(hat_t *hat)
{
    hat_hold(hat, HAT_NEUTRAL);
}

inline void hat_init(hat_t *hat, button_t *up, button_t *right, button_t *down, button_t *left)
{
    hat->up_ = up;
    hat->right_ = right;
    hat->down_ = down;
    hat->left_ = left;
    hat_release(hat);
}

#endif