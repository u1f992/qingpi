#include "qingpi.h"

#include <stdlib.h>

typedef enum TestDigiPotState
{
    TEST_DIGIPOT_POWER_ON,
    TEST_DIGIPOT_SHUTDOWN
} TestDigiPotState;

typedef struct TestDigiPot
{
    QpiDigitalPotentiometerInterface parent;
    TestDigiPotState state;
    uint16_t position;
} TestDigiPot;

static void test_digipot_set_wiper_position(QpiDigitalPotentiometerInterface *parent, uint16_t position)
{
    TestDigiPot *self = (TestDigiPot *)parent;
    self->position = position;
}

static void test_digipot_power_on(QpiDigitalPotentiometerInterface *parent)
{
    TestDigiPot *self = (TestDigiPot *)parent;
    self->state = TEST_DIGIPOT_POWER_ON;
}

static void test_digipot_shutdown(QpiDigitalPotentiometerInterface *parent)
{
    TestDigiPot *self = (TestDigiPot *)parent;
    self->state = TEST_DIGIPOT_SHUTDOWN;
}

TestDigiPot *test_digipot_new(void)
{
    TestDigiPot *self = (TestDigiPot *)malloc(sizeof(TestDigiPot));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.set_wiper_position = test_digipot_set_wiper_position;
    self->parent.power_on = test_digipot_power_on;
    self->parent.shutdown = test_digipot_shutdown;

    self->state = TEST_DIGIPOT_POWER_ON;
    self->position = 0;

    return self;
}

void test_digipot_delete(TestDigiPot *self)
{
    free(self);
}