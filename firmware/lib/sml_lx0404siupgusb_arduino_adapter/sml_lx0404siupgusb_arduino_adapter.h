#ifndef SML_LX0404SIUPGUSB_ARDUINO_ADAPTER_H_
#define SML_LX0404SIUPGUSB_ARDUINO_ADAPTER_H_

#include <Arduino.h>
#include "sml_lx0404siupgusb.h"

typedef struct SML_LX0404SIUPGUSBArduinoAnalogAdapter
{
    SML_LX0404SIUPGUSBAnalogOutputInterface parent;
    pin_size_t pin;
    int max_value;
} SML_LX0404SIUPGUSBArduinoAnalogAdapter;

SML_LX0404SIUPGUSBArduinoAnalogAdapter *sml_lx0404siupgusb_arduino_analog_adapter_new(const pin_size_t pin, const int resolution);
inline void sml_lx0404siupgusb_arduino_analog_adapter_delete(SML_LX0404SIUPGUSBArduinoAnalogAdapter *self)
{
    free(self);
}

typedef struct SML_LX0404SIUPGUSBArduinoDigitalAdapter
{
    SML_LX0404SIUPGUSBDigitalOutputInterface parent;
    pin_size_t pin;
} SML_LX0404SIUPGUSBArduinoDigitalAdapter;

SML_LX0404SIUPGUSBArduinoDigitalAdapter *sml_lx0404siupgusb_arduino_digital_adapter_new(const pin_size_t pin);
inline void sml_lx0404siupgusb_arduino_digital_adapter_delete(SML_LX0404SIUPGUSBArduinoDigitalAdapter *self)
{
    free(self);
}

#endif
