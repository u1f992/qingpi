#pragma once

#include <Arduino.h>

typedef struct
{
    pin_size_t pin;
} button_t;

void button_hold(button_t *btn)
{
    pinMode(btn->pin, OUTPUT);
    digitalWrite(btn->pin, LOW);
}

void button_release(button_t *btn)
{
    pinMode(btn->pin, INPUT); // Hi-Z
}

void button_init(button_t *btn, pin_size_t pin)
{
    btn->pin = pin;
    button_release(btn);
}