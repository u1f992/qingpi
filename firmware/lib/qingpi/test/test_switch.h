#include "qingpi.h"

#include <stdlib.h>

typedef enum TestSwitchState
{
    TEST_SWITCH_ON,
    TEST_SWITCH_OFF
} TestSwitchState;

typedef struct TestSwitch
{
    QpiSPSTSwitchInterface parent;
    TestSwitchState state;
} TestSwitch;

static void test_switch_on(QpiSPSTSwitchInterface *parent)
{
    TestSwitch *self = (TestSwitch *)parent;
    self->state = TEST_SWITCH_ON;
}

static void test_switch_off(QpiSPSTSwitchInterface *parent)
{
    TestSwitch *self = (TestSwitch *)parent;
    self->state = TEST_SWITCH_OFF;
}

TestSwitch *test_switch_new(void)
{
    TestSwitch *self = (TestSwitch *)malloc(sizeof(TestSwitch));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.on = test_switch_on;
    self->parent.off = test_switch_off;

    self->state = TEST_SWITCH_OFF;

    return self;
}

void test_switch_delete(TestSwitch *self)
{
    free(self);
}