#ifndef AD840X_H_
#define AD840X_H_

#include <Arduino.h>
#include <SPI.h>

typedef struct AD840X
{
    HardwareSPI *spi;
    pin_size_t cs;
    pin_size_t shdn;
} AD840X;

typedef enum AD840XAddress
{
    AD840X_RDAC1,
    AD840X_RDAC2,
    AD840X_RDAC3,
    AD840X_RDAC4
} AD840XAddress;

void ad840x_set(AD840X *self, AD840XAddress addr, uint8_t data);
void ad840x_shutdown(AD840X *self);
void ad840x_power_on(AD840X *self);
AD840X *ad840x_new(HardwareSPI *spi, pin_size_t cs, pin_size_t shdn);
void ad840x_delete(AD840X *self);

#endif