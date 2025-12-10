#include "ctinypalette.h"

#include <assert.h>
#include <stdio.h>

typedef struct TestCase_HSV16ToRGB16
{
    uint16_t param_hue;
    uint16_t param_saturation;
    uint16_t param_value;

    uint16_t expected_red;
    uint16_t expected_green;
    uint16_t expected_blue;
} TestCase_HSV16ToRGB16;

static const uint16_t PARAM_IRRELEVANT = 0;

static inline uint16_t uint16_abs(const uint16_t a, const uint16_t b)
{
    int diff = (int)a - (int)b;
    return (uint16_t)abs(diff);
}

static const uint16_t ALLOWANCE_LIMIT = 5;

static int test_hsv_16_to_rgb_16(TestCase_HSV16ToRGB16 cases[], const size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        TestCase_HSV16ToRGB16 test = cases[i];
        CtpHSV16 *hsv = ctp_hsv_16_new(test.param_hue, test.param_saturation, test.param_value);
        CtpRGB16 *rgb = ctp_hsv_16_to_rgb_16(hsv);
        ctp_hsv_16_delete(hsv);

        assert(rgb != NULL);
        if (ALLOWANCE_LIMIT < uint16_abs(rgb->red, test.expected_red) ||
            ALLOWANCE_LIMIT < uint16_abs(rgb->green, test.expected_green) ||
            ALLOWANCE_LIMIT < uint16_abs(rgb->blue, test.expected_blue))
        {
            ctp_rgb_16_delete(rgb);
            return i;
        }
        ctp_rgb_16_delete(rgb);
    }
    return -1;
}

int main(void)
{
    int index = -1;

    TestCase_HSV16ToRGB16 cases[] = {
        {// Red
         .param_hue = 0,
         .param_saturation = UINT16_MAX,
         .param_value = UINT16_MAX,
         .expected_red = UINT16_MAX,
         .expected_green = 0,
         .expected_blue = 0},
        {// Green
         .param_hue = UINT16_MAX * 120 / 360,
         .param_saturation = UINT16_MAX,
         .param_value = UINT16_MAX,
         .expected_red = 0,
         .expected_green = UINT16_MAX,
         .expected_blue = 0},
        {// Blue
         .param_hue = UINT16_MAX * 240 / 360,
         .param_saturation = UINT16_MAX,
         .param_value = UINT16_MAX,
         .expected_red = 0,
         .expected_green = 0,
         .expected_blue = UINT16_MAX},
        {// White
         .param_hue = PARAM_IRRELEVANT,
         .param_saturation = 0,
         .param_value = UINT16_MAX,
         .expected_red = UINT16_MAX,
         .expected_green = UINT16_MAX,
         .expected_blue = UINT16_MAX},
        {// Black
         .param_hue = PARAM_IRRELEVANT,
         .param_saturation = 0,
         .param_value = 0,
         .expected_red = 0,
         .expected_green = 0,
         .expected_blue = 0},
        {// Randomly generated
         .param_hue = 10767,
         .param_saturation = 43381,
         .param_value = 52903,
         .expected_red = 52903,
         .expected_green = 52405,
         .expected_blue = 17883}};
    size_t length = sizeof(cases) / sizeof(cases[0]);
    index = test_hsv_16_to_rgb_16(cases, length);
    if (0 <= index)
    {
        fprintf(stderr, "[test_hsv_16_to_rgb_16] Assertion failed on index %d", index);
        return 1;
    }

    printf("OK\n");
    return 0;
}
