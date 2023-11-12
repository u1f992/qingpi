#include "ncm.h"

#include <stdlib.h>

typedef enum TestSwitchState
{
    TEST_SWITCH_ON,
    TEST_SWITCH_OFF
} TestSwitchState;

typedef struct TestSwitch
{
    NcmSwitchInterface parent;
    TestSwitchState state;
} TestSwitch;

static void test_switch_on(NcmSwitchInterface *parent)
{
    TestSwitch *self = (TestSwitch *)parent;
    self->state = TEST_SWITCH_ON;
}

static void test_switch_off(NcmSwitchInterface *parent)
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