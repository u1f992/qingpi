#pragma once

#include <Arduino.h>

typedef struct
{
    pin_size_t a;
} g3vm_x1wr_t;

void g3vm_x1wr_on(g3vm_x1wr_t *relay)
{
    digitalWrite(relay->a, HIGH);
}

void g3vm_x1wr_off(g3vm_x1wr_t *relay)
{
    digitalWrite(relay->a, LOW);
}

void g3vm_x1wr_init(g3vm_x1wr_t *relay, pin_size_t a)
{
    relay->a = a;
    pinMode(relay->a, OUTPUT);
    g3vm_x1wr_off(relay);
}