#ifndef QINGPI_H_
#define QINGPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct QpiGeneralPurposeIOInterface
{
    void (*set_low)(struct QpiGeneralPurposeIOInterface *self);
    void (*set_hi_z)(struct QpiGeneralPurposeIOInterface *self);
} QpiGeneralPurposeIOInterface;

/**
 * @brief -263.79-263.79[uA] output is used, but slight differences should be able to be handled by calibration on the 3DS side.
 */
typedef struct QpiCurrentDAConverterInterface
{
    void (*sink)(struct QpiCurrentDAConverterInterface *self, uint16_t value);
    void (*source)(struct QpiCurrentDAConverterInterface *self, uint16_t value);
} QpiCurrentDAConverterInterface;

/**
 * @brief Total resistance value must be less than pull-up resistance (details unknown, 10kΩ?). Horizontal dimension is 320px and vertical dimension is 240px.
 */
typedef struct QpiDigitalPotentiometerInterface
{
    void (*set_wiper_position)(struct QpiDigitalPotentiometerInterface *self, uint16_t position);
    void (*power_on)(struct QpiDigitalPotentiometerInterface *self);
    void (*shutdown)(struct QpiDigitalPotentiometerInterface *self);
} QpiDigitalPotentiometerInterface;

/**
 * @brief Use CMOS analog switches with low on-resistance.
 */
typedef struct QpiSPSTSwitchInterface
{
    void (*on)(struct QpiSPSTSwitchInterface *self);
    void (*off)(struct QpiSPSTSwitchInterface *self);
} QpiSPSTSwitchInterface;

typedef struct QpiButton
{
    QpiGeneralPurposeIOInterface *gpio;
} QpiButton;

void qpi_button_hold(QpiButton *self);
void qpi_button_release(QpiButton *self);
QpiButton *qpi_button_new(QpiGeneralPurposeIOInterface *gpio);
void qpi_button_delete(QpiButton *self);

typedef struct QpiHat
{
    QpiButton *up;
    QpiButton *right;
    QpiButton *down;
    QpiButton *left;
} QpiHat;

typedef enum QpiHatDirection
{
    QPI_HAT_UP,
    QPI_HAT_UPRIGHT,
    QPI_HAT_RIGHT,
    QPI_HAT_DOWNRIGHT,
    QPI_HAT_DOWN,
    QPI_HAT_DOWNLEFT,
    QPI_HAT_LEFT,
    QPI_HAT_UPLEFT,
    QPI_HAT_NEUTRAL
} QpiHatDirection;

void qpi_hat_hold(QpiHat *self, QpiHatDirection direction);
void qpi_hat_release(QpiHat *self);
QpiHat *qpi_hat_new(QpiButton *up, QpiButton *right, QpiButton *down, QpiButton *left);
void qpi_hat_delete(QpiHat *self);

typedef struct QpiSlidePad
{
    QpiCurrentDAConverterInterface *vertical;
    QpiCurrentDAConverterInterface *horizontal;
} QpiSlidePad;

#define QPI_SLIDEPAD_NEUTRAL ((uint16_t)32767)

void qpi_slidepad_hold(QpiSlidePad *self, uint16_t x, uint16_t y);
void qpi_slidepad_release(QpiSlidePad *self);
QpiSlidePad *qpi_slidepad_new(QpiCurrentDAConverterInterface *vertical, QpiCurrentDAConverterInterface *horizontal);
void qpi_slidepad_delete(QpiSlidePad *self);

typedef struct QpiTouchScreen
{
    QpiDigitalPotentiometerInterface *vertical;
    QpiDigitalPotentiometerInterface *horizontal;
    QpiSPSTSwitchInterface *sw;
} QpiTouchScreen;

void qpi_touchscreen_hold(QpiTouchScreen *self, uint16_t x, uint16_t y);
void qpi_touchscreen_release(QpiTouchScreen *self);
QpiTouchScreen *qpi_touchscreen_new(QpiDigitalPotentiometerInterface *vertical, QpiDigitalPotentiometerInterface *horizontal, QpiSPSTSwitchInterface *sw);
void qpi_touchscreen_delete(QpiTouchScreen *self);

#ifdef __cplusplus
}
#endif

#endif