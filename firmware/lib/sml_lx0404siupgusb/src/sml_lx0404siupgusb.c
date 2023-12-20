#include "sml_lx0404siupgusb.h"

extern inline void sml_lx0404siupgusb_set(SML_LX0404SIUPGUSB *self, const uint16_t red, const uint16_t green, const uint16_t blue);
extern inline void sml_lx0404siupgusb_on(SML_LX0404SIUPGUSB *self);
extern inline void sml_lx0404siupgusb_off(SML_LX0404SIUPGUSB *self);
extern inline SML_LX0404SIUPGUSB *sml_lx0404siupgusb_new(SML_LX0404SIUPGUSBDigitalOutputInterface *anode, SML_LX0404SIUPGUSBAnalogOutputInterface *red, SML_LX0404SIUPGUSBAnalogOutputInterface *green, SML_LX0404SIUPGUSBAnalogOutputInterface *blue);
extern inline void sml_lx0404siupgusb_delete(SML_LX0404SIUPGUSB *self);
