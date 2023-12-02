#ifndef ADAPTERS_H_
#define ADAPTERS_H_

#include <Arduino.h>

#include "ncm.h"

#include "ad840x.h"
#include "adg801.h"
#include "ds4432.h"

typedef struct AD840XAdapter
{
    NcmDigitalPotentiometerInterface parent;
    AD840X *pots;
    AD840XAddress addr;
} AD840XAdapter;

AD840XAdapter *ad840x_adapter_new(AD840X *pots, AD840XAddress addr);
void ad840x_adapter_delete(AD840XAdapter *self);

typedef struct AD840XTripleAdapter
{
    NcmDigitalPotentiometerInterface parent;
    AD840X *pots;
    AD840XAddress addr1;
    AD840XAddress addr2;
    AD840XAddress addr3;
} AD840XTripleAdapter;

/**
 * │
 * U1─┐
 * │  │
 * x  │
 *    U3─
 * x  │
 * │  │
 * U2─┘
 * │
 */
AD840XTripleAdapter *ad840x_triple_adapter_new(AD840X *pots, AD840XAddress addr1, AD840XAddress addr2, AD840XAddress addr3);
void ad840x_triple_adapter_delete(AD840XTripleAdapter *self);

typedef struct ADG801Adapter
{
    NcmSPSTSwitchInterface parent;
    ADG801 *sw;
} ADG801Adapter;

ADG801Adapter *adg801_adapter_new(ADG801 *sw);
void adg801_adapter_delete(ADG801Adapter *self);

typedef struct DS4432Adapter
{
    NcmCurrentDAConverterInterface parent;
    DS4432 *dac;
    DS4432MemoryAddress addr;
} DS4432Adapter;

DS4432Adapter *ds4432_adapter_new(DS4432 *dac, DS4432MemoryAddress addr);
void ds4432_adapter_delete(DS4432Adapter *self);

typedef struct GPIOAdapter
{
    NcmGeneralPurposeIOInterface parent;
    pin_size_t pin;
} GPIOAdapter;

GPIOAdapter *gpio_adapter_new(pin_size_t pin);
void gpio_adapter_delete(GPIOAdapter *self);

#endif