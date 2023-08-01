#pragma once

#include <Arduino.h>

typedef struct
{
    pin_size_t in;
} adg752_t;

void adg752_s1(adg752_t *sw)
{
    digitalWrite(sw->in, LOW);
}

void adg752_s2(adg752_t *sw)
{
    digitalWrite(sw->in, HIGH);
}

void adg752_init(adg752_t *sw, pin_size_t in)
{
    sw->in = in;
    pinMode(sw->in, OUTPUT);
    adg752_s1(sw);
}