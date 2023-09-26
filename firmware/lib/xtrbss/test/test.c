#include <assert.h>
#include <stdio.h>

#include "xtrbss.h"

typedef enum TestGPIOState
{
    TEST_GPIO_LOW,
    TEST_GPIO_HIGH,
    TEST_GPIO_HI_Z
} TestGPIOState;

typedef struct TestGPIO
{
    XtrBssGeneralPurposeIOInterface parent;
    TestGPIOState state;
} TestGPIO;

XtrBssErrNo test_gpio_set_low(XtrBssGeneralPurposeIOInterface *parent)
{
    TestGPIO *self = (TestGPIO *)parent;
    self->state = TEST_GPIO_LOW;
    return XTRBSS_OK;
}

XtrBssErrNo test_gpio_set_high(XtrBssGeneralPurposeIOInterface *parent)
{
    TestGPIO *self = (TestGPIO *)parent;
    self->state = TEST_GPIO_HIGH;
    return XTRBSS_OK;
}

XtrBssErrNo test_gpio_set_hi_z(XtrBssGeneralPurposeIOInterface *parent)
{
    TestGPIO *self = (TestGPIO *)parent;
    self->state = TEST_GPIO_HI_Z;
    return XTRBSS_OK;
}

void test_gpio_new(TestGPIO *gpio)
{
    gpio->parent.set_low = test_gpio_set_low;
    gpio->parent.set_high = test_gpio_set_high;
    gpio->parent.set_hi_z = test_gpio_set_hi_z;

    gpio->state = TEST_GPIO_HI_Z;
}

void test_button(void)
{
    TestGPIO gpio;
    test_gpio_new(&gpio);

    XtrBssButton btn;
    assert(xtrbss_button_new(&btn, &gpio.parent) == XTRBSS_OK);
    assert(gpio.state == TEST_GPIO_HI_Z);

    assert(xtrbss_button_hold(&btn) == XTRBSS_OK);
    assert(gpio.state == TEST_GPIO_LOW);

    assert(xtrbss_button_release(&btn) == XTRBSS_OK);
    assert(gpio.state == TEST_GPIO_HI_Z);
}

void test_hat(void)
{
    TestGPIO gpio_up;
    test_gpio_new(&gpio_up);
    XtrBssButton btn_up;
    xtrbss_button_new(&btn_up, &gpio_up.parent);

    TestGPIO gpio_right;
    test_gpio_new(&gpio_right);
    XtrBssButton btn_right;
    xtrbss_button_new(&btn_right, &gpio_right.parent);

    TestGPIO gpio_down;
    test_gpio_new(&gpio_down);
    XtrBssButton btn_down;
    xtrbss_button_new(&btn_down, &gpio_down.parent);

    TestGPIO gpio_left;
    test_gpio_new(&gpio_left);
    XtrBssButton btn_left;
    xtrbss_button_new(&btn_left, &gpio_left.parent);

    XtrBssHat hat;
    assert(xtrbss_hat_new(&hat, &btn_up, &btn_right, &btn_down, &btn_left) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_HI_Z &&
           gpio_right.state == TEST_GPIO_HI_Z &&
           gpio_down.state == TEST_GPIO_HI_Z &&
           gpio_left.state == TEST_GPIO_HI_Z);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_UP) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_LOW &&
           gpio_right.state == TEST_GPIO_HI_Z &&
           gpio_down.state == TEST_GPIO_HI_Z &&
           gpio_left.state == TEST_GPIO_HI_Z);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_UPRIGHT) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_LOW &&
           gpio_right.state == TEST_GPIO_LOW &&
           gpio_down.state == TEST_GPIO_HI_Z &&
           gpio_left.state == TEST_GPIO_HI_Z);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_RIGHT) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_HI_Z &&
           gpio_right.state == TEST_GPIO_LOW &&
           gpio_down.state == TEST_GPIO_HI_Z &&
           gpio_left.state == TEST_GPIO_HI_Z);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_DOWNRIGHT) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_HI_Z &&
           gpio_right.state == TEST_GPIO_LOW &&
           gpio_down.state == TEST_GPIO_LOW &&
           gpio_left.state == TEST_GPIO_HI_Z);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_DOWN) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_HI_Z &&
           gpio_right.state == TEST_GPIO_HI_Z &&
           gpio_down.state == TEST_GPIO_LOW &&
           gpio_left.state == TEST_GPIO_HI_Z);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_DOWNLEFT) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_HI_Z &&
           gpio_right.state == TEST_GPIO_HI_Z &&
           gpio_down.state == TEST_GPIO_LOW &&
           gpio_left.state == TEST_GPIO_LOW);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_LEFT) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_HI_Z &&
           gpio_right.state == TEST_GPIO_HI_Z &&
           gpio_down.state == TEST_GPIO_HI_Z &&
           gpio_left.state == TEST_GPIO_LOW);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_UPLEFT) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_LOW &&
           gpio_right.state == TEST_GPIO_HI_Z &&
           gpio_down.state == TEST_GPIO_HI_Z &&
           gpio_left.state == TEST_GPIO_LOW);

    assert(xtrbss_hat_hold(&hat, XTRBSS_HAT_NEUTRAL) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_HI_Z &&
           gpio_right.state == TEST_GPIO_HI_Z &&
           gpio_down.state == TEST_GPIO_HI_Z &&
           gpio_left.state == TEST_GPIO_HI_Z);

    assert(xtrbss_hat_release(&hat) == XTRBSS_OK);
    assert(gpio_up.state == TEST_GPIO_HI_Z &&
           gpio_right.state == TEST_GPIO_HI_Z &&
           gpio_down.state == TEST_GPIO_HI_Z &&
           gpio_left.state == TEST_GPIO_HI_Z);
}

typedef enum TestDACState
{
    TEST_DAC_SINK,
    TEST_DAC_SOURCE
} TestDACState;

typedef struct TestDAC
{
    XtrBssCurrentDAConverterInterface parent;
    TestDACState state;
    double current_value;
} TestDAC;

XtrBssErrNo test_dac_sink(XtrBssCurrentDAConverterInterface *parent, double val)
{
    assert(0.0 <= val && val <= 1.0);

    TestDAC *self = (TestDAC *)parent;
    self->state = TEST_DAC_SINK;
    self->current_value = val;
    return XTRBSS_OK;
}

XtrBssErrNo test_dac_source(XtrBssCurrentDAConverterInterface *parent, double val)
{
    assert(0.0 <= val && val <= 1.0);

    TestDAC *self = (TestDAC *)parent;
    self->state = TEST_DAC_SOURCE;
    self->current_value = val;
    return XTRBSS_OK;
}

void test_dac_new(TestDAC *dac)
{
    dac->parent.sink = test_dac_sink;
    dac->parent.source = test_dac_source;

    dac->state = TEST_DAC_SOURCE;
    dac->current_value = 0.0;
}

void test_slidepad(void)
{
    TestDAC v;
    test_dac_new(&v);
    TestDAC h;
    test_dac_new(&h);

    XtrBssSlidePad sp;
    assert(xtrbss_slidepad_new(&sp, &v.parent, &h.parent) == XTRBSS_OK);
    assert(v.state == TEST_DAC_SOURCE && v.current_value == 0.0 &&
           h.state == TEST_DAC_SOURCE && h.current_value == 0.0);

    assert(xtrbss_slidepad_hold(&sp, 0.0, 0.0) == XTRBSS_OK);
    assert(v.state == TEST_DAC_SINK && v.current_value == 1.0 &&
           h.state == TEST_DAC_SINK && h.current_value == 1.0);

    assert(xtrbss_slidepad_hold(&sp, 0.0, 1.0) == XTRBSS_OK);
    assert(v.state == TEST_DAC_SOURCE && v.current_value == 1.0 &&
           h.state == TEST_DAC_SINK && h.current_value == 1.0);

    assert(xtrbss_slidepad_hold(&sp, 1.0, 1.0) == XTRBSS_OK);
    assert(v.state == TEST_DAC_SOURCE && v.current_value == 1.0 &&
           h.state == TEST_DAC_SOURCE && h.current_value == 1.0);

    assert(xtrbss_slidepad_hold(&sp, 1.0, 0.0) == XTRBSS_OK);
    assert(v.state == TEST_DAC_SINK && v.current_value == 1.0 &&
           h.state == TEST_DAC_SOURCE && h.current_value == 1.0);

    assert(xtrbss_slidepad_release(&sp) == XTRBSS_OK);
    assert(v.state == TEST_DAC_SOURCE && v.current_value == 0.0 &&
           h.state == TEST_DAC_SOURCE && h.current_value == 0.0);
}

typedef enum TestDigiPotState
{
    TEST_DIGIPOT_POWER_ON,
    TEST_DIGIPOT_SHUTDOWN
} TestDigiPotState;

typedef struct TestDigiPot
{
    XtrBssDigitalPotentiometerInterface parent;
    TestDigiPotState state;
    double current_position;
} TestDigiPot;

XtrBssErrNo test_digipot_set_wiper_position(XtrBssDigitalPotentiometerInterface *parent, double pos)
{
    TestDigiPot *self = (TestDigiPot *)parent;
    self->current_position = pos;
    return XTRBSS_OK;
}

XtrBssErrNo test_digipot_power_on(XtrBssDigitalPotentiometerInterface *parent)
{
    TestDigiPot *self = (TestDigiPot *)parent;
    self->state = TEST_DIGIPOT_POWER_ON;
    return XTRBSS_OK;
}

XtrBssErrNo test_digipot_shutdown(XtrBssDigitalPotentiometerInterface *parent)
{
    TestDigiPot *self = (TestDigiPot *)parent;
    self->state = TEST_DIGIPOT_SHUTDOWN;
    return XTRBSS_OK;
}

void test_digipot_new(TestDigiPot *pot)
{
    pot->parent.set_wiper_position = test_digipot_set_wiper_position;
    pot->parent.power_on = test_digipot_power_on;
    pot->parent.shutdown = test_digipot_shutdown;

    pot->state = TEST_DIGIPOT_POWER_ON;
    pot->current_position = 0.0;
}

typedef enum TestRelayState
{
    TEST_RELAY_ENERGIZED,
    TEST_RELAY_DE_ENERGIZED
} TestRelayState;

typedef struct TestRelay
{
    XtrBssRelayInterface parent;
    TestRelayState state;
} TestRelay;

XtrBssErrNo test_relay_energize(XtrBssRelayInterface *parent)
{
    TestRelay *self = (TestRelay *)parent;
    self->state = TEST_RELAY_ENERGIZED;
    return XTRBSS_OK;
}

XtrBssErrNo test_relay_de_energize(XtrBssRelayInterface *parent)
{
    TestRelay *self = (TestRelay *)parent;
    self->state = TEST_RELAY_DE_ENERGIZED;
    return XTRBSS_OK;
}

void test_relay_new(TestRelay *relay)
{
    relay->parent.energize = test_relay_energize;
    relay->parent.de_energize = test_relay_de_energize;

    relay->state = TEST_RELAY_DE_ENERGIZED;
}

void test_touchscreen(void)
{
    TestDigiPot v;
    test_digipot_new(&v);
    TestDigiPot h;
    test_digipot_new(&h);
    TestRelay r;
    test_relay_new(&r);

    XtrBssTouchScreen ts;
    assert(xtrbss_touchscreen_new(&ts, &v.parent, &h.parent, &r.parent) == XTRBSS_OK);
    assert(v.state == TEST_DIGIPOT_SHUTDOWN && h.state == TEST_DIGIPOT_SHUTDOWN);
    assert(r.state == TEST_RELAY_DE_ENERGIZED);

    xtrbss_touchscreen_hold(&ts, 0.0, 0.0);
    assert(v.state == TEST_DIGIPOT_POWER_ON && v.current_position == 0.0 &&
           h.state == TEST_DIGIPOT_POWER_ON && h.current_position == 0.0);
    assert(r.state == TEST_RELAY_ENERGIZED);

    xtrbss_touchscreen_hold(&ts, 0.5, 0.5);
    assert(v.state == TEST_DIGIPOT_POWER_ON && v.current_position == 0.5 &&
           h.state == TEST_DIGIPOT_POWER_ON && h.current_position == 0.5);
    assert(r.state == TEST_RELAY_ENERGIZED);
    
    xtrbss_touchscreen_hold(&ts, 1.0, 1.0);
    assert(v.state == TEST_DIGIPOT_POWER_ON && v.current_position == 1.0 &&
           h.state == TEST_DIGIPOT_POWER_ON && h.current_position == 1.0);
    assert(r.state == TEST_RELAY_ENERGIZED);

    xtrbss_touchscreen_release(&ts);
    assert(v.state == TEST_DIGIPOT_SHUTDOWN && h.state == TEST_DIGIPOT_SHUTDOWN);
    assert(r.state == TEST_RELAY_DE_ENERGIZED);
}

int main(void)
{
    test_button();
    test_hat();
    test_slidepad();
    test_touchscreen();

    printf("ok\n");
}