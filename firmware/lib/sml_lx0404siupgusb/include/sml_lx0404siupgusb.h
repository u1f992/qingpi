#ifndef SML_LX0404SIUPGUSB_H_
#define SML_LX0404SIUPGUSB_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdlib.h>

typedef enum SML_LX0404SIUPGUSBDigitalOutputState
{
    SML_LX0404SIUPGUSB_LOW,
    SML_LX0404SIUPGUSB_HIGH
} SML_LX0404SIUPGUSBDigitalOutputState;

typedef struct SML_LX0404SIUPGUSBDigitalOutputInterface
{
    void (*write)(struct SML_LX0404SIUPGUSBDigitalOutputInterface *self, const SML_LX0404SIUPGUSBDigitalOutputState value);
} SML_LX0404SIUPGUSBDigitalOutputInterface;

/**
 * This should be useful if the anode is connected directly to the power supply.
 */
typedef struct SML_LX0404SIUPGUSBNullDigitalOutput
{
    SML_LX0404SIUPGUSBDigitalOutputInterface parent;
} SML_LX0404SIUPGUSBNullDigitalOutput;

SML_LX0404SIUPGUSBNullDigitalOutput *sml_lx0404siupgusb_null_digital_output_new(void);
inline void sml_lx0404siupgusb_null_digital_output_delete(SML_LX0404SIUPGUSBNullDigitalOutput *self)
{
    free(self);
}

typedef struct SML_LX0404SIUPGUSBAnalogOutputInterface
{
    void (*write)(struct SML_LX0404SIUPGUSBAnalogOutputInterface *self, const uint16_t value);
} SML_LX0404SIUPGUSBAnalogOutputInterface;

typedef struct SML_LX0404SIUPGUSB
{
    SML_LX0404SIUPGUSBDigitalOutputInterface *anode;
    SML_LX0404SIUPGUSBAnalogOutputInterface *red;
    SML_LX0404SIUPGUSBAnalogOutputInterface *green;
    SML_LX0404SIUPGUSBAnalogOutputInterface *blue;
} SML_LX0404SIUPGUSB;

typedef enum SML_LX0404SIUPGUSBColorState
{
    SML_LX0404SIUPGUSB_ON = UINT16_MAX,
    SML_LX0404SIUPGUSB_OFF = 0
} SML_LX0404SIUPGUSBState;

inline void sml_lx0404siupgusb_set(SML_LX0404SIUPGUSB *self, const uint16_t red, const uint16_t green, const uint16_t blue)
{
    if (self == NULL)
    {
        return;
    }

    self->red->write(self->red, UINT16_MAX - red);
    self->green->write(self->green, UINT16_MAX - green);
    self->blue->write(self->blue, UINT16_MAX - blue);
}

inline void sml_lx0404siupgusb_on(SML_LX0404SIUPGUSB *self)
{
    if (self == NULL)
    {
        return;
    }

    self->anode->write(self->anode, SML_LX0404SIUPGUSB_HIGH);
}

inline void sml_lx0404siupgusb_off(SML_LX0404SIUPGUSB *self)
{
    if (self == NULL)
    {
        return;
    }

    self->anode->write(self->anode, SML_LX0404SIUPGUSB_LOW);
    sml_lx0404siupgusb_set(self, SML_LX0404SIUPGUSB_OFF, SML_LX0404SIUPGUSB_OFF, SML_LX0404SIUPGUSB_OFF);
}

inline SML_LX0404SIUPGUSB *sml_lx0404siupgusb_new(SML_LX0404SIUPGUSBDigitalOutputInterface *anode, SML_LX0404SIUPGUSBAnalogOutputInterface *red, SML_LX0404SIUPGUSBAnalogOutputInterface *green, SML_LX0404SIUPGUSBAnalogOutputInterface *blue)
{
    if (anode == NULL || red == NULL || green == NULL || blue == NULL)
    {
        return NULL;
    }

    SML_LX0404SIUPGUSB *self = (SML_LX0404SIUPGUSB *)malloc(sizeof(SML_LX0404SIUPGUSB));
    if (self == NULL)
    {
        return NULL;
    }

    self->anode = anode;
    self->red = red;
    self->green = green;
    self->blue = blue;

    sml_lx0404siupgusb_off(self);

    return self;
}

inline void sml_lx0404siupgusb_delete(SML_LX0404SIUPGUSB *self)
{
    if (self == NULL)
    {
        return;
    }

    sml_lx0404siupgusb_off(self);

    free(self);
}

#ifdef __cplusplus
}
#endif

#endif