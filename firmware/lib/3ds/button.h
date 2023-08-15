#ifndef _3DS_BUTTON_H_
#define _3DS_BUTTON_H_

#include <Arduino.h>

typedef struct
{
    pin_size_t pin_;
} button_t;

inline void button_hold(button_t *btn)
{
    pinMode(btn->pin_, OUTPUT);
    digitalWrite(btn->pin_, LOW);
}

inline void button_release(button_t *btn)
{
    pinMode(btn->pin_, INPUT); // Hi-Z
}

inline void button_init(button_t *btn, pin_size_t pin)
{
    btn->pin_ = pin;
    button_release(btn);
}

#endif