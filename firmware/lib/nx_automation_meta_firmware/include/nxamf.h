#ifndef NXAMF_H_
#define NXAMF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum NxamfButtonState
{
    NXAMF_BUTTON_STATE_RELEASED,
    NXAMF_BUTTON_STATE_PRESSED
} NxamfButtonState;

typedef enum NxamfHatState
{
    NXAMF_HAT_STATE_UP,
    NXAMF_HAT_STATE_UPRIGHT,
    NXAMF_HAT_STATE_RIGHT,
    NXAMF_HAT_STATE_DOWNRIGHT,
    NXAMF_HAT_STATE_DOWN,
    NXAMF_HAT_STATE_DOWNLEFT,
    NXAMF_HAT_STATE_LEFT,
    NXAMF_HAT_STATE_UPLEFT,
    NXAMF_HAT_STATE_NEUTRAL
} NxamfHatState;

#define NXAMF_STICK_STATE_NEUTRAL 128U

typedef struct NxamfStickState
{
    uint8_t x;
    uint8_t y;
} NxamfStickState;

typedef struct NxamfGamepadState
{
    NxamfButtonState y;
    NxamfButtonState b;
    NxamfButtonState a;
    NxamfButtonState x;
    NxamfButtonState l;
    NxamfButtonState r;
    NxamfButtonState zl;
    NxamfButtonState zr;
    NxamfButtonState minus;
    NxamfButtonState plus;
    NxamfButtonState l_click;
    NxamfButtonState r_click;
    NxamfButtonState home;
    NxamfButtonState capture;

    NxamfHatState hat;

    NxamfStickState l_stick;
    NxamfStickState r_stick;

    uint8_t extension[16];
} NxamfGamepadState;

void nxamf_gamepad_state_delete(NxamfGamepadState *self);

typedef struct NxamfBytesProtocolInterface
{
    bool (*is_acceptable)(struct NxamfBytesProtocolInterface *self, const uint8_t packet, const uint8_t buffer[], const size_t length);
    bool (*is_ready)(struct NxamfBytesProtocolInterface *self, const uint8_t buffer[], const size_t length);
    void (*convert)(struct NxamfBytesProtocolInterface *self, const uint8_t buffer[], const size_t length, NxamfGamepadState *state);
} NxamfBytesProtocolInterface;

typedef struct NxamfBytesBuffer
{
    NxamfBytesProtocolInterface *protocol;
    uint8_t buffer[64]; // isn't it enough?
    size_t length;
} NxamfBytesBuffer;

NxamfBytesBuffer *nxamf_bytes_buffer_new(NxamfBytesProtocolInterface *protocol);
void nxamf_bytes_buffer_delete(NxamfBytesBuffer *self);
NxamfGamepadState *nxamf_bytes_buffer_append(NxamfBytesBuffer *self, const uint8_t packet);
void nxamf_bytes_buffer_clear(NxamfBytesBuffer *self);

typedef struct NxamfProtocolMultiplexer
{
    NxamfBytesProtocolInterface parent;
    NxamfBytesProtocolInterface **protocols;
    size_t protocols_length;
    size_t ready_index;
} NxamfProtocolMultiplexer;

NxamfProtocolMultiplexer *nxamf_protocol_multiplexer_new(NxamfBytesProtocolInterface *protocols[], size_t length);
void nxamf_protocol_multiplexer_delete(NxamfProtocolMultiplexer *self);

#ifdef __cplusplus
}
#endif

#endif // NXAMF_H_