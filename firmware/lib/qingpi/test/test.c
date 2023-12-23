#include "qingpi.h"

#include <assert.h>
#include <stdio.h>

#include "test_gpio.h"
#include "test_dac.h"
#include "test_digipot.h"
#include "test_switch.h"

int test_button(void)
{
    int index = -1;

    TestGPIO *gpio = test_gpio_new();
    assert(gpio != NULL);

    QpiButton *btn = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio);
    assert(btn != NULL);
    assert(gpio->state == TEST_GPIO_HI_Z);

    qpi_button_hold(btn);
    if (!(gpio->state == TEST_GPIO_LOW))
    {
        index = 0;
        goto cleanup;
    }

    qpi_button_release(btn);
    if (!(gpio->state == TEST_GPIO_HI_Z))
    {
        index = 1;
        goto cleanup;
    }

cleanup:
    qpi_button_delete(btn);
    test_gpio_delete(gpio);

    return index;
}

int test_hat(void)
{
    int index = -1;

    TestGPIO *gpio_up = test_gpio_new();
    assert(gpio_up != NULL);
    QpiButton *btn_up = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_up);
    assert(btn_up != NULL);

    TestGPIO *gpio_right = test_gpio_new();
    assert(gpio_right != NULL);
    QpiButton *btn_right = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_right);
    assert(btn_right != NULL);

    TestGPIO *gpio_down = test_gpio_new();
    assert(gpio_down != NULL);
    QpiButton *btn_down = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_down);
    assert(btn_down != NULL);

    TestGPIO *gpio_left = test_gpio_new();
    assert(gpio_left != NULL);
    QpiButton *btn_left = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_left);
    assert(btn_left != NULL);

    QpiHat *hat = qpi_hat_new(btn_up, btn_right, btn_down, btn_left);
    assert(hat != NULL);
    assert(gpio_up->state == TEST_GPIO_HI_Z &&
           gpio_right->state == TEST_GPIO_HI_Z &&
           gpio_down->state == TEST_GPIO_HI_Z &&
           gpio_left->state == TEST_GPIO_HI_Z);

    qpi_hat_hold(hat, QPI_HAT_UP);
    if (!(gpio_up->state == TEST_GPIO_LOW &&
          gpio_right->state == TEST_GPIO_HI_Z &&
          gpio_down->state == TEST_GPIO_HI_Z &&
          gpio_left->state == TEST_GPIO_HI_Z))
    {
        index = 0;
        goto cleanup;
    }

    qpi_hat_hold(hat, QPI_HAT_UPRIGHT);
    if (!(gpio_up->state == TEST_GPIO_LOW &&
          gpio_right->state == TEST_GPIO_LOW &&
          gpio_down->state == TEST_GPIO_HI_Z &&
          gpio_left->state == TEST_GPIO_HI_Z))
    {
        index = 1;
        goto cleanup;
    }

    qpi_hat_hold(hat, QPI_HAT_RIGHT);
    if (!(gpio_up->state == TEST_GPIO_HI_Z &&
          gpio_right->state == TEST_GPIO_LOW &&
          gpio_down->state == TEST_GPIO_HI_Z &&
          gpio_left->state == TEST_GPIO_HI_Z))
    {
        index = 2;
        goto cleanup;
    }

    qpi_hat_hold(hat, QPI_HAT_DOWNRIGHT);
    if (!(gpio_up->state == TEST_GPIO_HI_Z &&
          gpio_right->state == TEST_GPIO_LOW &&
          gpio_down->state == TEST_GPIO_LOW &&
          gpio_left->state == TEST_GPIO_HI_Z))
    {
        index = 3;
        goto cleanup;
    }

    qpi_hat_hold(hat, QPI_HAT_DOWN);
    if (!(gpio_up->state == TEST_GPIO_HI_Z &&
          gpio_right->state == TEST_GPIO_HI_Z &&
          gpio_down->state == TEST_GPIO_LOW &&
          gpio_left->state == TEST_GPIO_HI_Z))
    {
        index = 4;
        goto cleanup;
    }

    qpi_hat_hold(hat, QPI_HAT_DOWNLEFT);
    if (!(gpio_up->state == TEST_GPIO_HI_Z &&
          gpio_right->state == TEST_GPIO_HI_Z &&
          gpio_down->state == TEST_GPIO_LOW &&
          gpio_left->state == TEST_GPIO_LOW))
    {
        index = 5;
        goto cleanup;
    }

    qpi_hat_hold(hat, QPI_HAT_LEFT);
    if (!(gpio_up->state == TEST_GPIO_HI_Z &&
          gpio_right->state == TEST_GPIO_HI_Z &&
          gpio_down->state == TEST_GPIO_HI_Z &&
          gpio_left->state == TEST_GPIO_LOW))
    {
        index = 6;
        goto cleanup;
    }

    qpi_hat_hold(hat, QPI_HAT_UPLEFT);
    if (!(gpio_up->state == TEST_GPIO_LOW &&
          gpio_right->state == TEST_GPIO_HI_Z &&
          gpio_down->state == TEST_GPIO_HI_Z &&
          gpio_left->state == TEST_GPIO_LOW))
    {
        index = 7;
        goto cleanup;
    }

    qpi_hat_hold(hat, QPI_HAT_NEUTRAL);
    if (!(gpio_up->state == TEST_GPIO_HI_Z &&
          gpio_right->state == TEST_GPIO_HI_Z &&
          gpio_down->state == TEST_GPIO_HI_Z &&
          gpio_left->state == TEST_GPIO_HI_Z))
    {
        index = 8;
        goto cleanup;
    }

    qpi_hat_release(hat);
    if (!(gpio_up->state == TEST_GPIO_HI_Z &&
          gpio_right->state == TEST_GPIO_HI_Z &&
          gpio_down->state == TEST_GPIO_HI_Z &&
          gpio_left->state == TEST_GPIO_HI_Z))
    {
        index = 9;
        goto cleanup;
    }

cleanup:
    qpi_hat_delete(hat);
    qpi_button_delete(btn_left);
    test_gpio_delete(gpio_left);
    qpi_button_delete(btn_down);
    test_gpio_delete(gpio_down);
    qpi_button_delete(btn_right);
    test_gpio_delete(gpio_right);
    qpi_button_delete(btn_up);
    test_gpio_delete(gpio_up);

    return index;
}

int test_slidepad(void)
{
    int index = -1;

    TestDAC *v = test_dac_new();
    assert(v != NULL);
    TestDAC *h = test_dac_new();
    assert(h != NULL);

    QpiSlidePad *sp = qpi_slidepad_new((QpiCurrentDAConverterInterface *)v, (QpiCurrentDAConverterInterface *)h);
    assert(sp != NULL);
    assert(v->state == TEST_DAC_SOURCE && v->value == 0 &&
           h->state == TEST_DAC_SOURCE && h->value == 0);

    qpi_slidepad_hold(sp, 0, 0);
    if (!(v->state == TEST_DAC_SOURCE && v->value == UINT16_MAX &&
          h->state == TEST_DAC_SOURCE && h->value == UINT16_MAX))
    {
        index = 0;
        goto cleanup;
    }

    qpi_slidepad_hold(sp, 0, UINT16_MAX);
    if (!(v->state == TEST_DAC_SINK && v->value == UINT16_MAX &&
          h->state == TEST_DAC_SOURCE && h->value == UINT16_MAX))
    {
        index = 1;
        goto cleanup;
    }

    qpi_slidepad_hold(sp, UINT16_MAX, UINT16_MAX);
    if (!(v->state == TEST_DAC_SINK && v->value == UINT16_MAX &&
          h->state == TEST_DAC_SINK && h->value == UINT16_MAX))
    {
        index = 2;
        goto cleanup;
    }

    qpi_slidepad_hold(sp, UINT16_MAX, 0);
    if (!(v->state == TEST_DAC_SOURCE && v->value == UINT16_MAX &&
          h->state == TEST_DAC_SINK && h->value == UINT16_MAX))
    {
        index = 3;
        goto cleanup;
    }

    qpi_slidepad_release(sp);
    if (!(v->state == TEST_DAC_SOURCE && v->value == 0 &&
          h->state == TEST_DAC_SOURCE && h->value == 0))
    {
        index = 4;
        goto cleanup;
    }

cleanup:
    qpi_slidepad_delete(sp);
    test_dac_delete(h);
    test_dac_delete(v);

    return index;
}

int test_touchscreen(void)
{
    int index = -1;

    TestDigiPot *v = test_digipot_new();
    assert(v != NULL);
    TestDigiPot *h = test_digipot_new();
    assert(h != NULL);
    TestSwitch *sw = test_switch_new();
    assert(sw != NULL);

    QpiTouchScreen *ts = qpi_touchscreen_new((QpiDigitalPotentiometerInterface *)v, (QpiDigitalPotentiometerInterface *)h, (QpiSPSTSwitchInterface *)sw);
    assert(ts != NULL);
    assert(v->state == TEST_DIGIPOT_SHUTDOWN && h->state == TEST_DIGIPOT_SHUTDOWN);
    assert(sw->state == TEST_SWITCH_OFF);

    qpi_touchscreen_hold(ts, 0, 0);
    if (!(v->state == TEST_DIGIPOT_POWER_ON && v->position == 0 &&
          h->state == TEST_DIGIPOT_POWER_ON && h->position == 0 &&
          sw->state == TEST_SWITCH_ON))
    {
        index = 0;
        goto cleanup;
    }

    qpi_touchscreen_hold(ts, 32767, 32767);
    if (!(v->state == TEST_DIGIPOT_POWER_ON && v->position == 32767 &&
          h->state == TEST_DIGIPOT_POWER_ON && h->position == 32767 &&
          sw->state == TEST_SWITCH_ON))
    {
        index = 1;
        goto cleanup;
    }

    qpi_touchscreen_hold(ts, UINT16_MAX, UINT16_MAX);
    if (!(v->state == TEST_DIGIPOT_POWER_ON && v->position == UINT16_MAX &&
          h->state == TEST_DIGIPOT_POWER_ON && h->position == UINT16_MAX &&
          sw->state == TEST_SWITCH_ON))
    {
        index = 2;
        goto cleanup;
    }

    qpi_touchscreen_release(ts);
    if (!(v->state == TEST_DIGIPOT_SHUTDOWN && h->state == TEST_DIGIPOT_SHUTDOWN && sw->state == TEST_SWITCH_OFF))
    {
        index = 3;
        goto cleanup;
    }

cleanup:
    qpi_touchscreen_delete(ts);
    test_switch_delete(sw);
    test_digipot_delete(h);
    test_digipot_delete(v);

    return index;
}

int main(void)
{
    int index;

    index = test_button();
    if (0 <= index)
    {
        fprintf(stderr, "[test_button] Assertion failed on index %d", index);
        return 1;
    }
    index = test_hat();
    if (0 <= index)
    {
        fprintf(stderr, "[test_hat] Assertion failed on index %d", index);
        return 1;
    }
    index = test_slidepad();
    if (0 <= index)
    {
        fprintf(stderr, "[test_slidepad] Assertion failed on index %d", index);
        return 1;
    }
    index = test_touchscreen();
    if (0 <= index)
    {
        fprintf(stderr, "[test_touchscreen] Assertion failed on index %d", index);
        return 1;
    }

    printf("OK\n");
    return 0;
}