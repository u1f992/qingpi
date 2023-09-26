#ifndef DS4432_H_
#define DS4432_H_

#include <Arduino.h>
#include <Wire.h>

typedef struct DS4432
{
    HardwareI2C *wire;
} DS4432;

#define DS4432_ADDRESS (0x90U >> 1)

typedef enum DS4432SignBit
{
    DS4432_SINK,
    DS4432_SOURCE
} DS4432SignBit;

typedef enum DS4432MemoryAddress
{
    DS4432_OUT0 = 0xF8U,
    DS4432_OUT1 = 0xF9U
} DS4432MemoryAddress;

inline void ds4432_set(DS4432 *dac, DS4432MemoryAddress addr, DS4432SignBit sign, uint8_t data)
{
    if (0b111'1111 < data)
    {
        return;
    }

    uint8_t d = ((uint8_t)sign << 7) | data;

    dac->wire->beginTransmission(DS4432_ADDRESS);
    dac->wire->write((uint8_t)addr);
    dac->wire->write(d);
    dac->wire->endTransmission();
}

inline void ds4432_new(DS4432 *dac, HardwareI2C *wire)
{
    dac->wire = wire;
}

#endif