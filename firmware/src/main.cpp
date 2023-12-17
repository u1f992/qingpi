#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "adapters.h"

#include "ncm.h"

#include "nxamf.h"
#include "nxamf/nxmc2.h"
#include "nxamf/pokecon.h"

#define NEOCTRLMOD_XL
#ifdef NEOCTRLMOD_XL
static const pin_size_t PIN_SELECT = 2;
static const pin_size_t PIN_HOME = 3;
static const pin_size_t PIN_POWER = 4;
static const pin_size_t PIN_START = 5;
static const pin_size_t PIN_LEFT = 6;
static const pin_size_t PIN_DOWN = 7;
static const pin_size_t PIN_UP = 8;
static const pin_size_t PIN_RIGHT = 9;
static const pin_size_t PIN_WIRELESS = 10;
static const pin_size_t PIN_B = 11;
static const pin_size_t PIN_X = 12;
static const pin_size_t PIN_Y = 13;
static const pin_size_t PIN_A = 14;
static const pin_size_t PIN_R = 15;
static const pin_size_t PIN_L = 16;
// static const pin_size_t PIN_IRDA = 17;
static const pin_size_t PIN_SPI_SCK = 18;
static const pin_size_t PIN_SPI_TX = 19;
static const pin_size_t PIN_SPI_CS = 20;
static const pin_size_t PIN_POTS_SHDN = 21;
static const pin_size_t PIN_SW_IN = 22;
static const pin_size_t PIN_LED_BUILTIN = 25;
static const pin_size_t PIN_I2C_SDA = 26;
static const pin_size_t PIN_I2C_SCL = 27;
#endif

static GPIOAdapter *gpio_y = NULL;
static NcmButton *btn_y = NULL;
static GPIOAdapter *gpio_b = NULL;
static NcmButton *btn_b = NULL;
static GPIOAdapter *gpio_a = NULL;
static NcmButton *btn_a = NULL;
static GPIOAdapter *gpio_x = NULL;
static NcmButton *btn_x = NULL;
static GPIOAdapter *gpio_l = NULL;
static NcmButton *btn_l = NULL;
static GPIOAdapter *gpio_r = NULL;
static NcmButton *btn_r = NULL;
static GPIOAdapter *gpio_select = NULL;
static NcmButton *btn_select = NULL;
static GPIOAdapter *gpio_start = NULL;
static NcmButton *btn_start = NULL;
static GPIOAdapter *gpio_home = NULL;
static NcmButton *btn_home = NULL;
static GPIOAdapter *gpio_power = NULL;
static NcmButton *btn_power = NULL;
static GPIOAdapter *gpio_wireless = NULL;
static NcmButton *btn_wireless = NULL;

static GPIOAdapter *gpio_up = NULL;
static NcmButton *hat_up = NULL;
static GPIOAdapter *gpio_right = NULL;
static NcmButton *hat_right = NULL;
static GPIOAdapter *gpio_down = NULL;
static NcmButton *hat_down = NULL;
static GPIOAdapter *gpio_left = NULL;
static NcmButton *hat_left = NULL;
static NcmHat *hat = NULL;

static DS4432 *dac = NULL;
static DS4432Adapter *dac_v = NULL;
static DS4432Adapter *dac_h = NULL;
static NcmSlidePad *sp = NULL;

static AD840X *pots = NULL;
static AD840XAdapter *pots_v = NULL;
static AD840XTripleAdapter *pots_h = NULL;
static ADG801 *sw = NULL;
static ADG801Adapter *sw_adapter = NULL;
static NcmTouchScreen *ts = NULL;

static const int SERIAL_INACTIVE_TIMEOUT = 100;
static int inactive_count = 0;

static Nxmc2Protocol *nxmc2;
static PokeConProtocol *pokecon;
static NxamfBytesProtocolInterface *protocols[2];
static NxamfProtocolMultiplexer *mux;
static NxamfBytesBuffer *buffer;

static int64_t led_off(alarm_id_t id, void *user_data)
{
    digitalWrite(PIN_LED_BUILTIN, LOW);
    return false;
}

static void async_led_on_for_100ms()
{
    digitalWriteFast(PIN_LED_BUILTIN, HIGH);
    alarm_id_t alarm_id = add_alarm_in_ms(100, led_off, NULL, false);
}

static void reflect_state(NxamfGamepadState *state)
{
    if (state == NULL)
    {
        return;
    }

    async_led_on_for_100ms();

    if (state->y == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_y);
    else
        ncm_button_release(btn_y);

    if (state->b == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_b);
    else
        ncm_button_release(btn_b);

    if (state->a == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_a);
    else
        ncm_button_release(btn_a);

    if (state->x == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_x);
    else
        ncm_button_release(btn_x);

    if (state->l == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_l);
    else
        ncm_button_release(btn_l);

    if (state->r == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_r);
    else
        ncm_button_release(btn_r);

    if (state->minus == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_select);
    else
        ncm_button_release(btn_select);

    if (state->plus == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_start);
    else
        ncm_button_release(btn_start);

    if (state->home == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_home);
    else
        ncm_button_release(btn_home);

    if (state->l_click == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_power);
    else
        ncm_button_release(btn_power);

    if (state->r_click == NXAMF_BUTTON_STATE_PRESSED)
        ncm_button_hold(btn_wireless);
    else
        ncm_button_release(btn_wireless);

    switch (state->hat)
    {
    case NXAMF_HAT_STATE_UP:
        ncm_hat_hold(hat, NCM_HAT_UP);
        break;
    case NXAMF_HAT_STATE_UPRIGHT:
        ncm_hat_hold(hat, NCM_HAT_UPRIGHT);
        break;
    case NXAMF_HAT_STATE_RIGHT:
        ncm_hat_hold(hat, NCM_HAT_RIGHT);
        break;
    case NXAMF_HAT_STATE_DOWNRIGHT:
        ncm_hat_hold(hat, NCM_HAT_DOWNRIGHT);
        break;
    case NXAMF_HAT_STATE_DOWN:
        ncm_hat_hold(hat, NCM_HAT_DOWN);
        break;
    case NXAMF_HAT_STATE_DOWNLEFT:
        ncm_hat_hold(hat, NCM_HAT_DOWNLEFT);
        break;
    case NXAMF_HAT_STATE_LEFT:
        ncm_hat_hold(hat, NCM_HAT_LEFT);
        break;
    case NXAMF_HAT_STATE_UPLEFT:
        ncm_hat_hold(hat, NCM_HAT_UPLEFT);
        break;
    case NXAMF_HAT_STATE_NEUTRAL:
    default:
        ncm_hat_release(hat);
        break;
    }

    if (state->l_stick.x != NXAMF_STICK_STATE_NEUTRAL || state->l_stick.y != NXAMF_STICK_STATE_NEUTRAL)
        ncm_slidepad_hold(sp, (double)state->l_stick.x / 255, (double)state->l_stick.y / 255);
    else
        ncm_slidepad_release(sp);

    uint16_t ts_x = state->extension[0] | state->extension[1] << 8;
    uint8_t ts_y = state->extension[2];
    if (0 < ts_x && ts_x <= 320 && 0 < ts_y && ts_y <= 240)
        ncm_touchscreen_hold(ts, (double)ts_x / 320, (double)ts_y / 240);
    else
        ncm_touchscreen_release(ts);
}

void setup()
{
    Serial.begin(9600);

    pinMode(PIN_Y, INPUT);
    gpio_y = gpio_adapter_new(PIN_Y);
    btn_y = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_y);
    pinMode(PIN_B, INPUT);
    gpio_b = gpio_adapter_new(PIN_B);
    btn_b = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_b);
    pinMode(PIN_A, INPUT);
    gpio_a = gpio_adapter_new(PIN_A);
    btn_a = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_a);
    pinMode(PIN_X, INPUT);
    gpio_x = gpio_adapter_new(PIN_X);
    btn_x = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_x);
    pinMode(PIN_L, INPUT);
    gpio_l = gpio_adapter_new(PIN_L);
    btn_l = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_l);
    pinMode(PIN_R, INPUT);
    gpio_r = gpio_adapter_new(PIN_R);
    btn_r = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_r);
    pinMode(PIN_SELECT, INPUT);
    gpio_select = gpio_adapter_new(PIN_SELECT);
    btn_select = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_select);
    pinMode(PIN_START, INPUT);
    gpio_start = gpio_adapter_new(PIN_START);
    btn_start = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_start);
    pinMode(PIN_HOME, INPUT);
    gpio_home = gpio_adapter_new(PIN_HOME);
    btn_home = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_home);
    pinMode(PIN_POWER, INPUT);
    gpio_power = gpio_adapter_new(PIN_POWER);
    btn_power = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_power);
    pinMode(PIN_WIRELESS, INPUT);
    gpio_wireless = gpio_adapter_new(PIN_WIRELESS);
    btn_wireless = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_wireless);

    pinMode(PIN_UP, INPUT);
    gpio_up = gpio_adapter_new(PIN_UP);
    hat_up = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_up);
    pinMode(PIN_RIGHT, INPUT);
    gpio_right = gpio_adapter_new(PIN_RIGHT);
    hat_right = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_right);
    pinMode(PIN_DOWN, INPUT);
    gpio_down = gpio_adapter_new(PIN_DOWN);
    hat_down = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_down);
    pinMode(PIN_LEFT, INPUT);
    gpio_left = gpio_adapter_new(PIN_LEFT);
    hat_left = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_left);
    hat = ncm_hat_new(hat_up, hat_right, hat_down, hat_left);

    Wire1.setSDA(PIN_I2C_SDA);
    Wire1.setSCL(PIN_I2C_SCL);
    Wire1.begin();
    dac = ds4432_new(&Wire1);
    dac_v = ds4432_adapter_new(dac, DS4432_OUT1);
    dac_h = ds4432_adapter_new(dac, DS4432_OUT0);
    sp = ncm_slidepad_new((NcmCurrentDAConverterInterface *)dac_v, (NcmCurrentDAConverterInterface *)dac_h);

    SPI.setSCK(PIN_SPI_SCK);
    SPI.setTX(PIN_SPI_TX);
    // SPI.setCS(PIN_SPI_CS);
    pinMode(PIN_SPI_CS, OUTPUT); // PIN_SPI_CS is not assigned to CS, but a normal GPIO.
    digitalWrite(PIN_SPI_CS, HIGH);
    SPI.begin();
    pinMode(PIN_POTS_SHDN, OUTPUT);
    digitalWrite(PIN_POTS_SHDN, LOW);
    pots = ad840x_new(&SPI, PIN_SPI_CS, PIN_POTS_SHDN);
    pots_v = ad840x_adapter_new(pots, AD840X_RDAC2);
    pots_h = ad840x_triple_adapter_new(pots, AD840X_RDAC3, AD840X_RDAC1, AD840X_RDAC4);
    pinMode(PIN_SW_IN, OUTPUT);
    sw = adg801_new(PIN_SW_IN);
    sw_adapter = adg801_adapter_new(sw);
    ts = ncm_touchscreen_new((NcmDigitalPotentiometerInterface *)pots_v, (NcmDigitalPotentiometerInterface *)pots_h, (NcmSPSTSwitchInterface *)sw_adapter);

    assert(
        btn_y != NULL &&
        btn_b != NULL &&
        btn_a != NULL &&
        btn_x != NULL &&
        btn_l != NULL &&
        btn_r != NULL &&
        btn_select != NULL &&
        btn_start != NULL &&
        btn_home != NULL &&
        btn_power != NULL &&
        btn_wireless != NULL &&
        hat != NULL &&
        sp != NULL &&
        ts != NULL);

    nxmc2 = nxmc2_protocol_new();
    pokecon = pokecon_protocol_new();
    protocols[0] = (NxamfBytesProtocolInterface *)nxmc2;
    protocols[1] = (NxamfBytesProtocolInterface *)pokecon;
    mux = nxamf_protocol_multiplexer_new(protocols, 3);
    buffer = nxamf_bytes_buffer_new((NxamfBytesProtocolInterface *)mux);
    assert(
        nxmc2 != NULL &&
        pokecon != NULL &&
        mux != NULL &&
        buffer != NULL);

    pinMode(PIN_LED_BUILTIN, OUTPUT);
    digitalWrite(PIN_LED_BUILTIN, LOW);

    // Blink if the setup routine is successful.
    digitalWrite(PIN_LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(PIN_LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(PIN_LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(PIN_LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(PIN_LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(PIN_LED_BUILTIN, LOW);
    delay(200);
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

    nxamf_gamepad_state_delete(state);
    state = NULL;
}
