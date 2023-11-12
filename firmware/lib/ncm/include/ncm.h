#ifndef NCM_H_
#define NCM_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct NcmGeneralPurposeIOInterface
{
    void (*set_low)(struct NcmGeneralPurposeIOInterface *self);
    void (*set_high)(struct NcmGeneralPurposeIOInterface *self);
    void (*set_hi_z)(struct NcmGeneralPurposeIOInterface *self);
} NcmGeneralPurposeIOInterface;

/**
 * @brief -263.79-263.79[uA] output is used, but slight differences should be able to be handled by calibration on the 3DS side.
 */
typedef struct NcmCurrentDAConverterInterface
{
    void (*sink)(struct NcmCurrentDAConverterInterface *self, double val);
    void (*source)(struct NcmCurrentDAConverterInterface *self, double val);
} NcmCurrentDAConverterInterface;

/**
 * @brief Total resistance value must be less than pull-up resistance (details unknown, 10kΩ?). Horizontal dimension is 320px and vertical dimension is 240px.
 */
typedef struct NcmDigitalPotentiometerInterface
{
    void (*set_wiper_position)(struct NcmDigitalPotentiometerInterface *self, double pos);
    void (*power_on)(struct NcmDigitalPotentiometerInterface *self);
    void (*shutdown)(struct NcmDigitalPotentiometerInterface *self);
} NcmDigitalPotentiometerInterface;

/**
 * @brief Use CMOS analog switches with low on-resistance.
 */
typedef struct NcmSwitchInterface
{
    void (*on)(struct NcmSwitchInterface *self);
    void (*off)(struct NcmSwitchInterface *self);
} NcmSwitchInterface;

typedef struct NcmButton
{
    NcmGeneralPurposeIOInterface *gpio;
} NcmButton;

void ncm_button_hold(NcmButton *self);
void ncm_button_release(NcmButton *self);
NcmButton *ncm_button_new(NcmGeneralPurposeIOInterface *gpio);
void ncm_button_delete(NcmButton *self);

typedef struct NcmHat
{
    NcmButton *up;
    NcmButton *right;
    NcmButton *down;
    NcmButton *left;
} NcmHat;

typedef enum NcmHatDirection
{
    NCM_HAT_UP,
    NCM_HAT_UPRIGHT,
    NCM_HAT_RIGHT,
    NCM_HAT_DOWNRIGHT,
    NCM_HAT_DOWN,
    NCM_HAT_DOWNLEFT,
    NCM_HAT_LEFT,
    NCM_HAT_UPLEFT,
    NCM_HAT_NEUTRAL
} NcmHatDirection;

void ncm_hat_hold(NcmHat *self, NcmHatDirection direction);
void ncm_hat_release(NcmHat *self);
NcmHat *ncm_hat_new(NcmButton *up, NcmButton *right, NcmButton *down, NcmButton *left);
void ncm_hat_delete(NcmHat *self);

typedef struct NcmSlidePad
{
    NcmCurrentDAConverterInterface *vertical;
    NcmCurrentDAConverterInterface *horizontal;
} NcmSlidePad;

#define NCM_SLIDEPAD_NEUTRAL 0.5

void ncm_slidepad_hold(NcmSlidePad *self, double x, double y);
void ncm_slidepad_release(NcmSlidePad *self);
NcmSlidePad *ncm_slidepad_new(NcmCurrentDAConverterInterface *vertical, NcmCurrentDAConverterInterface *horizontal);
void ncm_slidepad_delete(NcmSlidePad *self);

typedef struct NcmTouchScreen
{
    NcmDigitalPotentiometerInterface *vertical;
    NcmDigitalPotentiometerInterface *horizontal;
    NcmSwitchInterface *sw;
} NcmTouchScreen;

void ncm_touchscreen_hold(NcmTouchScreen *self, double x, double y);
void ncm_touchscreen_release(NcmTouchScreen *self);
NcmTouchScreen *ncm_touchscreen_new(NcmDigitalPotentiometerInterface *vertical, NcmDigitalPotentiometerInterface *horizontal, NcmSwitchInterface *sw);
void ncm_touchscreen_delete(NcmTouchScreen *self);

#ifdef __cplusplus
}
#endif

#endif