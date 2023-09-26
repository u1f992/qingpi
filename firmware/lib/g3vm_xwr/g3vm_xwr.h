#ifndef G3VM_XWR_H_
#define G3VM_XWR_H_

#include <Arduino.h>

typedef struct G3VM_XWR
{
    pin_size_t a;
} G3VM_XWR;

inline void g3vm_xwr_energize(G3VM_XWR *relay)
{
    digitalWrite(relay->a, HIGH);
}

inline void g3vm_xwr_de_energize(G3VM_XWR *relay)
{
    digitalWrite(relay->a, LOW);
}

inline void g3vm_xwr_new(G3VM_XWR *relay, pin_size_t a)
{
    relay->a = a;

    pinMode(relay->a, OUTPUT);
    g3vm_xwr_de_energize(relay);
}

#endif