#ifndef CTINYPALETTE_H_
#define CTINYPALETTE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdlib.h>

typedef struct CtpRGB16
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} CtpRGB16;

inline CtpRGB16 *ctp_rgb_16_new(const uint16_t red, const uint16_t green, const uint16_t blue)
{
    CtpRGB16 *self = (CtpRGB16 *)malloc(sizeof(CtpRGB16));
    if (self == NULL)
    {
        return NULL;
    }

    self->red = red;
    self->green = green;
    self->blue = blue;

    return self;
}

inline void ctp_rgb_16_delete(CtpRGB16 *self)
{
    free(self);
}

typedef struct CtpHSV16
{
    /**
     * [0, 360) deg mapped to [0, UINT16_MAX].
     * hue=UINT16_MAX does not imply hue=0, but means the closest representable value to 360deg.
     */
    uint16_t hue;
    /**
     * [0, 1] mapped to [0, UINT16_MAX].
     */
    uint16_t saturation;
    /**
     * [0, 1] mapped to [0, UINT16_MAX].
     */
    uint16_t value;
} CtpHSV16;

inline CtpHSV16 *ctp_hsv_16_new(const uint16_t hue, const uint16_t saturation, const uint16_t value)
{
    CtpHSV16 *self = (CtpHSV16 *)malloc(sizeof(CtpHSV16));
    if (self == NULL)
    {
        return NULL;
    }

    self->hue = hue;
    self->saturation = saturation;
    self->value = value;

    return self;
}

inline void ctp_hsv_16_delete(CtpHSV16 *self)
{
    free(self);
}

CtpRGB16 *ctp_hsv_16_to_rgb_16(CtpHSV16 *self);

#ifdef __cplusplus
}
#endif

#endif
