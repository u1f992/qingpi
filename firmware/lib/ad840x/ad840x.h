#ifndef AD840X_H_
#define AD840X_H_

#include <Arduino.h>
#include <SPI.h>

typedef struct AD840X
{
    HardwareSPI *spi;
    pin_size_t cs;
    int8_t shdn;
} AD840X;

typedef enum AD840XAddress
{
    AD840X_RDAC1,
    AD840X_RDAC2,
    AD840X_RDAC3,
    AD840X_RDAC4
} AD840XAddress;

inline void ad840x_set(AD840X *pots, AD840XAddress addr, uint8_t data)
{
    uint16_t d = (((uint8_t)addr) << 8) | data;

    digitalWrite(pots->cs, LOW);
    pots->spi->transfer16(d);
    digitalWrite(pots->cs, HIGH);
}

inline void ad840x_shutdown(AD840X *pots)
{
    if (pots->shdn == -1)
    {
        return;
    }
    digitalWrite(pots->shdn, LOW);
}

inline void ad840x_power_on(AD840X *pots)
{
    if (pots->shdn == -1)
    {
        return;
    }
    digitalWrite(pots->shdn, HIGH);
}

inline void ad840x_new(AD840X *pots, HardwareSPI *spi, pin_size_t cs, int8_t shdn = -1)
{
    pots->spi = spi;
    pots->cs = cs;
    pots->shdn = shdn;

    pinMode(pots->cs, OUTPUT);
    digitalWrite(pots->cs, HIGH);
    if (pots->shdn != -1)
    {
        pinMode(pots->shdn, OUTPUT);
    }
}

#endif