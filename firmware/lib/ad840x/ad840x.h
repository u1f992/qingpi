#ifndef AD840X_H_
#define AD840X_H_

#include <Arduino.h>
#include <SPI.h>

typedef struct
{
    HardwareSPI *spi_;
    pin_size_t cs_;
    int8_t shdn_;
} ad840x_t;

typedef enum
{
    AD840X_RDAC1,
    AD840X_RDAC2,
    AD840X_RDAC3,
    AD840X_RDAC4
} ad840x_address_t;

inline void ad840x_set(ad840x_t *pots, ad840x_address_t addr, uint8_t data)
{
    uint16_t d = (((uint8_t)addr) << 8) | data;

    digitalWrite(pots->cs_, LOW);
    pots->spi_->transfer16(d);
    digitalWrite(pots->cs_, HIGH);
}

inline void ad840x_shutdown(ad840x_t *pots)
{
    if (pots->shdn_ == -1)
    {
        return;
    }
    digitalWrite(pots->shdn_, LOW);
}

inline void ad840x_power_on(ad840x_t *pots)
{
    if (pots->shdn_ == -1)
    {
        return;
    }
    digitalWrite(pots->shdn_, HIGH);
}

inline void ad840x_init(ad840x_t *pots, HardwareSPI *spi, pin_size_t cs, int8_t shdn = -1)
{
    pots->spi_ = spi;
    pots->cs_ = cs;
    pinMode(pots->cs_, OUTPUT);
    digitalWrite(pots->cs_, HIGH);
    pots->shdn_ = shdn;
    if (pots->shdn_ != -1)
    {
        pinMode(pots->shdn_, OUTPUT);
    }
}

#endif