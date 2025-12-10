#include "nxamf.h"

typedef struct TestProtocolParams
{
    uint8_t *data;
    size_t length;
    NxamfGamepadState expected;
} TestProtocolParams;

static inline void compare_states(NxamfGamepadState *s1, NxamfGamepadState *s2)
{
    assert(s1->y == s2->y);
    assert(s1->b == s2->b);
    assert(s1->a == s2->a);
    assert(s1->x == s2->x);
    assert(s1->l == s2->l);
    assert(s1->r == s2->r);
    assert(s1->zl == s2->zl);
    assert(s1->zr == s2->zr);
    assert(s1->minus == s2->minus);
    assert(s1->plus == s2->plus);
    assert(s1->l_click == s2->l_click);
    assert(s1->r_click == s2->r_click);
    assert(s1->home == s2->home);
    assert(s1->capture == s2->capture);
    assert(s1->hat == s2->hat);
    assert(s1->l_stick.x == s2->l_stick.x);
    assert(s1->l_stick.y == s2->l_stick.y);
    assert(s1->r_stick.x == s2->r_stick.x);
    assert(s1->r_stick.y == s2->r_stick.y);
    assert(memcmp(s1->extension, s2->extension, 16) == 0);
}

void test_protocol_params_run(NxamfBytesProtocolInterface *protocol, TestProtocolParams params[], size_t length)
{
    for (int i = 0; i < length; i++)
    {
        NxamfBytesBuffer *buffer = nxamf_bytes_buffer_new(protocol);
        assert(buffer != NULL);

        TestProtocolParams p = params[i];
        for (int j = 0; j < p.length - 1; j++)
        {
            assert(nxamf_bytes_buffer_append(buffer, p.data[j]) == NULL);
        }
        NxamfGamepadState *state = nxamf_bytes_buffer_append(buffer, p.data[p.length - 1]);
        assert(state != NULL);
        compare_states(state, &p.expected);

        nxamf_gamepad_state_delete(state);
        nxamf_bytes_buffer_delete(buffer);
    }
}