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

void ds4432_set(DS4432 *self, DS4432MemoryAddress addr, DS4432SignBit sign, uint8_t data);
DS4432 *ds4432_new(HardwareI2C *wire);
void ds4432_delete(DS4432 *self);

#endif