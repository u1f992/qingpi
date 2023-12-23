#include "qingpi.h"

#include <stdlib.h>

typedef enum TestDACState
{
    TEST_DAC_SINK,
    TEST_DAC_SOURCE
} TestDACState;

typedef struct TestDAC
{
    QpiCurrentDAConverterInterface parent;
    TestDACState state;
    uint16_t value;
} TestDAC;

static void test_dac_sink(QpiCurrentDAConverterInterface *parent, uint16_t value)
{
    TestDAC *self = (TestDAC *)parent;
    self->state = TEST_DAC_SINK;
    self->value = value;
}

static void test_dac_source(QpiCurrentDAConverterInterface *parent, uint16_t value)
{
    TestDAC *self = (TestDAC *)parent;
    self->state = TEST_DAC_SOURCE;
    self->value = value;
}

TestDAC *test_dac_new(void)
{
    TestDAC *self = (TestDAC *)malloc(sizeof(TestDAC));
    if (self == NULL)
    {
        return NULL;
    }

    self->parent.sink = test_dac_sink;
    self->parent.source = test_dac_source;

    self->state = TEST_DAC_SOURCE;
    self->value = 0;

    return self;
}

void test_dac_delete(TestDAC *self)
{
    free(self);
}