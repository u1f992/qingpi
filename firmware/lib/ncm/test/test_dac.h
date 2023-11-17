#include "ncm.h"

#include <stdlib.h>

typedef enum TestDACState
{
    TEST_DAC_SINK,
    TEST_DAC_SOURCE
} TestDACState;

typedef struct TestDAC
{
    NcmCurrentDAConverterInterface parent;
    TestDACState state;
    double value;
} TestDAC;

static void test_dac_sink(NcmCurrentDAConverterInterface *parent, double value)
{
    TestDAC *self = (TestDAC *)parent;
    self->state = TEST_DAC_SINK;
    self->value = value;
}

static void test_dac_source(NcmCurrentDAConverterInterface *parent, double value)
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
    self->value = 0.0;

    return self;
}

void test_dac_delete(TestDAC *self)
{
    free(self);
}