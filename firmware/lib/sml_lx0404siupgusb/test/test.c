#include "sml_lx0404siupgusb.h"

#include <assert.h>
#include <stdio.h>

#include "analog_output_mock.h"

typedef struct TestCase
{
    uint16_t param_red;
    uint16_t param_green;
    uint16_t param_blue;

    uint16_t expected_red;
    uint16_t expected_green;
    uint16_t expected_blue;
} TestCase;

int test(TestCase cases[], size_t length)
{
    int index = -1;

    SML_LX0404SIUPGUSBNullDigitalOutput *anode = sml_lx0404siupgusb_null_digital_output_new();
    AnalogOutputMock *red = analog_output_mock_new(0);
    AnalogOutputMock *green = analog_output_mock_new(0);
    AnalogOutputMock *blue = analog_output_mock_new(0);
    SML_LX0404SIUPGUSB *led = sml_lx0404siupgusb_new((SML_LX0404SIUPGUSBDigitalOutputInterface *)anode, (SML_LX0404SIUPGUSBAnalogOutputInterface *)red, (SML_LX0404SIUPGUSBAnalogOutputInterface *)green, (SML_LX0404SIUPGUSBAnalogOutputInterface *)blue);
    assert(led != NULL);
    assert(red->value == UINT16_MAX);
    assert(green->value == UINT16_MAX);
    assert(blue->value == UINT16_MAX);

    for (size_t i = 0; i < length; i++)
    {
        sml_lx0404siupgusb_set(led, cases[i].param_red, cases[i].param_green, cases[i].param_blue);
        if (red->value != cases[i].expected_red ||
            green->value != cases[i].expected_green ||
            blue->value != cases[i].expected_blue)
        {
            index = i;
            goto cleanup;
        }
    }

cleanup:
    sml_lx0404siupgusb_delete(led);
    led = NULL;
    assert(red->value == UINT16_MAX);
    assert(green->value == UINT16_MAX);
    assert(blue->value == UINT16_MAX);
    analog_output_mock_delete(blue);
    blue = NULL;
    analog_output_mock_delete(green);
    green = NULL;
    analog_output_mock_delete(red);
    red = NULL;
    sml_lx0404siupgusb_null_digital_output_delete(anode);
    anode = NULL;

    return index;
}

int main(void)
{
    int index = test((TestCase[]){
                         {.param_red = SML_LX0404SIUPGUSB_OFF,
                          .param_green = SML_LX0404SIUPGUSB_OFF,
                          .param_blue = SML_LX0404SIUPGUSB_OFF,
                          .expected_red = UINT16_MAX,
                          .expected_green = UINT16_MAX,
                          .expected_blue = UINT16_MAX},
                         {.param_red = SML_LX0404SIUPGUSB_ON,
                          .param_green = SML_LX0404SIUPGUSB_ON,
                          .param_blue = SML_LX0404SIUPGUSB_ON,
                          .expected_red = 0,
                          .expected_green = 0,
                          .expected_blue = 0},
                         {.param_red = SML_LX0404SIUPGUSB_ON,
                          .param_green = SML_LX0404SIUPGUSB_OFF,
                          .param_blue = SML_LX0404SIUPGUSB_OFF,
                          .expected_red = 0,
                          .expected_green = UINT16_MAX,
                          .expected_blue = UINT16_MAX},
                         {.param_red = SML_LX0404SIUPGUSB_OFF,
                          .param_green = SML_LX0404SIUPGUSB_ON,
                          .param_blue = SML_LX0404SIUPGUSB_OFF,
                          .expected_red = UINT16_MAX,
                          .expected_green = 0,
                          .expected_blue = UINT16_MAX},
                         {.param_red = SML_LX0404SIUPGUSB_OFF,
                          .param_green = SML_LX0404SIUPGUSB_OFF,
                          .param_blue = SML_LX0404SIUPGUSB_ON,
                          .expected_red = UINT16_MAX,
                          .expected_green = UINT16_MAX,
                          .expected_blue = 0},
                         {.param_red = 100,
                          .param_green = 200,
                          .param_blue = 300,
                          .expected_red = 65435 /* UINT16_MAX - 100 */,
                          .expected_green = 65335 /* UINT16_MAX - 200 */,
                          .expected_blue = 65235 /* UINT16_MAX - 300 */}},
                     6);
    if (0 <= index)
    {
        fprintf(stderr, "[test] Assertion failed on index %d", index);
        return 1;
    }

    printf("OK\n");
    return 0;
}