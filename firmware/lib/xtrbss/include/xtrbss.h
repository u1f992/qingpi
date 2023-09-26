#ifndef XTRBSS_H_
#define XTRBSS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

typedef int XtrBssErrNo;
#define XTRBSS_OK 0
#define XTRBSS_EINVAL 22

typedef struct XtrBssGeneralPurposeIOInterface
{
    XtrBssErrNo (*set_low)(struct XtrBssGeneralPurposeIOInterface *self);
    XtrBssErrNo (*set_high)(struct XtrBssGeneralPurposeIOInterface *self);
    XtrBssErrNo (*set_hi_z)(struct XtrBssGeneralPurposeIOInterface *self);
} XtrBssGeneralPurposeIOInterface;

/**
 * @brief -263.79-263.79[uA] output is used, but slight differences should be able to be handled by calibration on the 3DS side.
 */
typedef struct XtrBssCurrentDAConverterInterface
{
    XtrBssErrNo (*sink)(struct XtrBssCurrentDAConverterInterface *self, double val);
    XtrBssErrNo (*source)(struct XtrBssCurrentDAConverterInterface *self, double val);
} XtrBssCurrentDAConverterInterface;

/**
 * @brief Total resistance value must be less than pull-up resistance (details unknown, 10kΩ?). Horizontal dimension is 320px and vertical dimension is 240px.
 */
typedef struct XtrBssDigitalPotentiometerInterface
{
    XtrBssErrNo (*set_wiper_position)(struct XtrBssDigitalPotentiometerInterface *self, double pos);
    XtrBssErrNo (*power_on)(struct XtrBssDigitalPotentiometerInterface *self);
    XtrBssErrNo (*shutdown)(struct XtrBssDigitalPotentiometerInterface *self);
} XtrBssDigitalPotentiometerInterface;

/**
 * @brief Due to fast switching and mounting area limitations, PhotoMOS relays will be almost the only option.
 */
typedef struct XtrBssRelayInterface
{
    XtrBssErrNo (*energize)(struct XtrBssRelayInterface *self);
    XtrBssErrNo (*de_energize)(struct XtrBssRelayInterface *self);
} XtrBssRelayInterface;

typedef struct XtrBssButton
{
    XtrBssGeneralPurposeIOInterface *gpio;
} XtrBssButton;

XtrBssErrNo xtrbss_button_hold(XtrBssButton *btn);
XtrBssErrNo xtrbss_button_release(XtrBssButton *btn);
XtrBssErrNo xtrbss_button_new(XtrBssButton *btn, XtrBssGeneralPurposeIOInterface *gpio);

typedef struct XtrBssHat
{
    XtrBssButton *up;
    XtrBssButton *right;
    XtrBssButton *down;
    XtrBssButton *left;
} XtrBssHat;

typedef enum XtrBssHatDirection
{
    XTRBSS_HAT_UP,
    XTRBSS_HAT_UPRIGHT,
    XTRBSS_HAT_RIGHT,
    XTRBSS_HAT_DOWNRIGHT,
    XTRBSS_HAT_DOWN,
    XTRBSS_HAT_DOWNLEFT,
    XTRBSS_HAT_LEFT,
    XTRBSS_HAT_UPLEFT,
    XTRBSS_HAT_NEUTRAL
} XtrBssHatDirection;

XtrBssErrNo xtrbss_hat_hold(XtrBssHat *hat, XtrBssHatDirection direction);
XtrBssErrNo xtrbss_hat_release(XtrBssHat *hat);
XtrBssErrNo xtrbss_hat_new(XtrBssHat *hat, XtrBssButton *up, XtrBssButton *right, XtrBssButton *down, XtrBssButton *left);

typedef struct XtrBssSlidePad
{
    XtrBssCurrentDAConverterInterface *vertical;
    XtrBssCurrentDAConverterInterface *horizontal;
} XtrBssSlidePad;

#define XTRBSS_SLIDEPAD_NEUTRAL 0.5

XtrBssErrNo xtrbss_slidepad_hold(XtrBssSlidePad *sp, double x, double y);
XtrBssErrNo xtrbss_slidepad_release(XtrBssSlidePad *sp);
XtrBssErrNo xtrbss_slidepad_new(XtrBssSlidePad *sp, XtrBssCurrentDAConverterInterface *vertical, XtrBssCurrentDAConverterInterface *horizontal);

typedef struct XtrBssTouchScreen
{
    XtrBssDigitalPotentiometerInterface *vertical;
    XtrBssDigitalPotentiometerInterface *horizontal;
    XtrBssRelayInterface *relay;
} XtrBssTouchScreen;

XtrBssErrNo xtrbss_touchscreen_hold(XtrBssTouchScreen *ts, double x, double y);
XtrBssErrNo xtrbss_touchscreen_release(XtrBssTouchScreen *ts);
XtrBssErrNo xtrbss_touchscreen_new(XtrBssTouchScreen *ts, XtrBssDigitalPotentiometerInterface *vertical, XtrBssDigitalPotentiometerInterface *horizontal, XtrBssRelayInterface *relay);

#ifdef __cplusplus
}
#endif

#endif