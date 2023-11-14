#ifndef ADG801_H_
#define ADG801_H_

#include <Arduino.h>

typedef struct ADG801
{
    pin_size_t in;
} ADG801;

void adg801_on(ADG801 *self);
void adg801_off(ADG801 *self);
ADG801 *adg801_new(pin_size_t in);
void adg801_delete(ADG801 *self);

#endif