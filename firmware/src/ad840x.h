#pragma once

#include <Arduino.h>
#include <SPI.h>

typedef struct
{
    HardwareSPI *spi;
    pin_size_t cs;
    int8_t shdn = -1;
} ad840x_t;

typedef enum
{
    AD840X_RDAC1,
    AD840X_RDAC2,
    AD840X_RDAC3,
    AD840X_RDAC4
} ad840x_address_t;

void ad840x_set(ad840x_t *pots, ad840x_address_t addr, uint8_t data)
{
    uint16_t d = (static_cast<uint8_t>(addr) << 8) | data;

    digitalWrite(pots->cs, LOW);
    pots->spi->transfer16(d);
    digitalWrite(pots->cs, HIGH);
}

void ad840x_shutdown(ad840x_t *pots)
{
    if (pots->shdn == -1)
    {
        return;
    }
    digitalWrite(pots->shdn, LOW);
}

void ad840x_power_on(ad840x_t *pots)
{
    if (pots->shdn == -1)
    {
        return;
    }
    digitalWrite(pots->shdn, HIGH);
}

void ad840x_init(ad840x_t *pots, HardwareSPI *spi, pin_size_t cs, int8_t shdn = -1)
{
    pots->spi = spi;
    pots->cs = cs;
    pinMode(pots->cs, OUTPUT);
    digitalWrite(pots->cs, HIGH);
    pots->shdn = shdn;
    if (pots->shdn != -1)
    {
        pinMode(pots->shdn, OUTPUT);
    }
}