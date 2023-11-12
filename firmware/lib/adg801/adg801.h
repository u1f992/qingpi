#ifndef ADG801_H_
#define ADG801_H_

#include <Arduino.h>

typedef struct ADG801
{
    pin_size_t in;
} ADG801;

inline void adg801_on(ADG801 *sw)
{
    digitalWrite(sw->in, HIGH);
}

inline void adg801_off(ADG801 *sw)
{
    digitalWrite(sw->in, LOW);
}

inline ADG801 *adg801_new(pin_size_t in)
{
    ADG801 *sw = (ADG801 *)malloc(sizeof(ADG801));
    if (sw == NULL)
    {
        return NULL;
    }

    pinMode(sw->in, OUTPUT);    
    sw->in = in;

    adg801_off(sw);

    return sw;
}

#endif