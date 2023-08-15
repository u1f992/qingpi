#ifndef G3VM_XWR_H_
#define G3VM_XWR_H_

#include <Arduino.h>

typedef struct
{
    pin_size_t a_;
} g3vm_xwr_t;

inline void g3vm_xwr_on(g3vm_xwr_t *relay)
{
    digitalWrite(relay->a_, HIGH);
}

inline void g3vm_xwr_off(g3vm_xwr_t *relay)
{
    digitalWrite(relay->a_, LOW);
}

inline void g3vm_xwr_init(g3vm_xwr_t *relay, pin_size_t a)
{
    relay->a_ = a;
    pinMode(relay->a_, OUTPUT);
    g3vm_xwr_off(relay);
}

#endif