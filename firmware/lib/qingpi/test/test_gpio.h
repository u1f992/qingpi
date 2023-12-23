#include "qingpi.h"

#include <stdlib.h>

typedef enum TestGPIOState
{
    TEST_GPIO_LOW,
    TEST_GPIO_HIGH,
    TEST_GPIO_HI_Z
} TestGPIOState;

typedef struct TestGPIO
{
    QpiGeneralPurposeIOInterface parent;
    TestGPIOState state;
} TestGPIO;

static void test_gpio_set_low(QpiGeneralPurposeIOInterface *parent)
{
    TestGPIO *self = (TestGPIO *)parent;
    self->state = TEST_GPIO_LOW;
}

static void test_gpio_set_hi_z(QpiGeneralPurposeIOInterface *parent)
{
    TestGPIO *self = (TestGPIO *)parent;
    self->state = TEST_GPIO_HI_Z;
}

TestGPIO *test_gpio_new(void)
{
    TestGPIO *self = (TestGPIO *)malloc(sizeof(TestGPIO));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.set_low = test_gpio_set_low;
    self->parent.set_hi_z = test_gpio_set_hi_z;

    self->state = TEST_GPIO_HI_Z;

    return self;
}

void test_gpio_delete(TestGPIO *self)
{
    free(self);
}