#include <Arduino.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "nxamf.h"
#include "nxamf/nxmc2.h"
#include "nxamf/pokecon.h"

static const int SERIAL_INACTIVE_TIMEOUT = 100;
static int inactive_count = 0;

static Nxmc2Protocol *nxmc2;
static PokeConProtocol *pokecon;
static NxamfBytesProtocolInterface *protocols[2];
static NxamfProtocolMultiplexer *mux;
static NxamfBytesBuffer *buffer;

static int64_t led_off(alarm_id_t id, void *user_data)
{
    digitalWrite(LED_BUILTIN, LOW);
    return false;
}

static void async_led_on_for_100ms()
{
    digitalWrite(LED_BUILTIN, HIGH);
    alarm_id_t alarm_id = add_alarm_in_ms(100, led_off, NULL, false);
}

static char buf[256];

static void print_button_state(const char *name, NxamfButtonState state)
{
    sprintf(buf, "%s\t%s", name, state == NXAMF_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial1.println(buf);
}

static void print_hat_state(NxamfHatState state)
{
    const char *tmp;
    switch (state)
    {
    case NXAMF_HAT_STATE_UP:
        tmp = "Up";
        break;
    case NXAMF_HAT_STATE_UPRIGHT:
        tmp = "UpRight";
        break;
    case NXAMF_HAT_STATE_RIGHT:
        tmp = "Right";
        break;
    case NXAMF_HAT_STATE_DOWNRIGHT:
        tmp = "DownRight";
        break;
    case NXAMF_HAT_STATE_DOWN:
        tmp = "Down";
        break;
    case NXAMF_HAT_STATE_DOWNLEFT:
        tmp = "DownLeft";
        break;
    case NXAMF_HAT_STATE_LEFT:
        tmp = "Left";
        break;
    case NXAMF_HAT_STATE_UPLEFT:
        tmp = "UpLeft";
        break;
    case NXAMF_HAT_STATE_NEUTRAL:
        tmp = "Neurtal";
        break;
    default:
        tmp = "[Error]";
        break;
    }
    sprintf(buf, "Hat\t%s", tmp);
    Serial1.println(buf);
}

static void print_stick_state(const char *name, NxamfStickState *state)
{
    sprintf(buf, "%s\tx:%d,y:%d", name, state->x, state->y);
    Serial1.println(buf);
}

static void print_extension(uint8_t extension[])
{
    sprintf(buf, "Extension\t%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", extension[0], extension[1], extension[2], extension[3], extension[4], extension[5], extension[6], extension[7], extension[8], extension[9], extension[10], extension[11], extension[12], extension[13], extension[14], extension[15]);
    Serial1.println(buf);
}

static void reflect_state(NxamfGamepadState *state)
{
    if (state == NULL)
    {
        return;
    }

    async_led_on_for_100ms();

    Serial1.println("--------------------");
    sprintf(buf, "Mode\t%s", mux->ready_index == 0 ? "NXMC2" : "PokeCon");
    Serial1.println(buf);

    print_button_state("Y", state->y);
    print_button_state("B", state->b);
    print_button_state("A", state->a);
    print_button_state("X", state->x);
    print_button_state("L", state->l);
    print_button_state("R", state->r);
    print_button_state("ZL", state->zl);
    print_button_state("ZR", state->zr);
    print_button_state("Minus", state->minus);
    print_button_state("Plus", state->plus);
    print_button_state("L Click", state->l_click);
    print_button_state("R Click", state->r_click);
    print_button_state("Home", state->home);
    print_button_state("Capture", state->capture);

    print_hat_state(state->hat);

    print_stick_state("L Stick", &state->l_stick);
    print_stick_state("R Stick", &state->r_stick);

    print_extension(state->extension);
}

void setup()
{
    Serial.begin(9600);

    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    nxmc2 = nxmc2_protocol_new();
    pokecon = pokecon_protocol_new();
    protocols[0] = (NxamfBytesProtocolInterface *)nxmc2;
    protocols[1] = (NxamfBytesProtocolInterface *)pokecon;
    mux = nxamf_protocol_multiplexer_new(protocols, 2);
    if (nxmc2 == NULL || pokecon == NULL || mux == NULL)
    {
        abort();
    }
    buffer = nxamf_bytes_buffer_new((NxamfBytesProtocolInterface *)mux);
    if (buffer == NULL)
    {
        abort();
    }
}

void loop()
{
    if (Serial.available() == 0)
    {
        inactive_count++;
        if (SERIAL_INACTIVE_TIMEOUT < inactive_count)
        {
            inactive_count = 0;
            nxamf_bytes_buffer_clear(buffer);
        }
        return;
    }
    inactive_count = 0;

    uint8_t packet = Serial.read();
    NxamfGamepadState *state = nxamf_bytes_buffer_append(buffer, packet);
    if (state == NULL)
    {
        return;
    }

    reflect_state(state);
}