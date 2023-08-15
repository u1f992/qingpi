#ifndef DS4432_H_
#define DS4432_H_

#include <Arduino.h>
#include <Wire.h>

typedef struct
{
    HardwareI2C *wire_;
} ds4432_t;

static const uint8_t DS4432_ADDRESS = (0x90U >> 1);

typedef enum
{
    DS4432_SINK,
    DS4432_SOURCE
} ds4432_sign_bit_t;

typedef enum
{
    DS4432_OUT0 = 0xF8U,
    DS4432_OUT1 = 0xF9U
} ds4432_memory_address_t;

inline void ds4432_set(ds4432_t *dac, ds4432_memory_address_t addr, ds4432_sign_bit_t sign, uint8_t data)
{
    if (0b111'1111 < data)
    {
        return;
    }

    uint8_t d = ((uint8_t)sign << 7) | data;

    dac->wire_->beginTransmission(DS4432_ADDRESS);
    dac->wire_->write((uint8_t)addr);
    dac->wire_->write(d);
    dac->wire_->endTransmission();
}

inline void ds4432_init(ds4432_t *dac, HardwareI2C *wire)
{
    dac->wire_ = wire;
}

#endif