#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "qingpi.h"
#include "qingpi_adapters.h"

#include "nxamf.h"
#include "nxamf/nxmc2.h"
#include "nxamf/pokecon.h"

#include "ctinypalette.h"
#include "sml_lx0404siupgusb.h"
#include "sml_lx0404siupgusb_arduino_adapter.h"

#define QINGPI_XL
#ifdef QINGPI_XL
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
static const pin_size_t PIN_LED_ANODE = 23;
static const pin_size_t PIN_LED_RED = 24;
static const pin_size_t PIN_LED_GREEN = 25;
static const pin_size_t PIN_I2C_SDA = 26;
static const pin_size_t PIN_I2C_SCL = 27;
static const pin_size_t PIN_LED_BLUE = 28;

static const uint16_t TOUCHSCREEN_X_MAX = 320;
static const uint8_t TOUCHSCREEN_Y_MAX = 240;
#endif

static GPIOAdapter *gpio_y = NULL;
static QpiButton *btn_y = NULL;
static GPIOAdapter *gpio_b = NULL;
static QpiButton *btn_b = NULL;
static GPIOAdapter *gpio_a = NULL;
static QpiButton *btn_a = NULL;
static GPIOAdapter *gpio_x = NULL;
static QpiButton *btn_x = NULL;
static GPIOAdapter *gpio_l = NULL;
static QpiButton *btn_l = NULL;
static GPIOAdapter *gpio_r = NULL;
static QpiButton *btn_r = NULL;
static GPIOAdapter *gpio_select = NULL;
static QpiButton *btn_select = NULL;
static GPIOAdapter *gpio_start = NULL;
static QpiButton *btn_start = NULL;
static GPIOAdapter *gpio_home = NULL;
static QpiButton *btn_home = NULL;
static GPIOAdapter *gpio_power = NULL;
static QpiButton *btn_power = NULL;
static GPIOAdapter *gpio_wireless = NULL;
static QpiButton *btn_wireless = NULL;

static GPIOAdapter *gpio_up = NULL;
static QpiButton *hat_up = NULL;
static GPIOAdapter *gpio_right = NULL;
static QpiButton *hat_right = NULL;
static GPIOAdapter *gpio_down = NULL;
static QpiButton *hat_down = NULL;
static GPIOAdapter *gpio_left = NULL;
static QpiButton *hat_left = NULL;
static QpiHat *hat = NULL;

static DS4432 *dac = NULL;
static DS4432Adapter *dac_v = NULL;
static DS4432Adapter *dac_h = NULL;
static QpiSlidePad *sp = NULL;

static AD840X *pots = NULL;
static AD840XAdapter *pots_v = NULL;
static AD840XTripleAdapter *pots_h = NULL;
static ADG801 *sw = NULL;
static ADG801Adapter *sw_adapter = NULL;
static QpiTouchScreen *ts = NULL;

static const int ANALOG_WRITE_RESOLUTION = 16;
static SML_LX0404SIUPGUSBArduinoDigitalAdapter *anode;
static SML_LX0404SIUPGUSBArduinoAnalogAdapter *pwm_red;
static SML_LX0404SIUPGUSBArduinoAnalogAdapter *pwm_green;
static SML_LX0404SIUPGUSBArduinoAnalogAdapter *pwm_blue;
static SML_LX0404SIUPGUSB *led;

static const int SERIAL_INACTIVE_TIMEOUT = 100;
static int inactive_count = 0;

static Nxmc2Protocol *nxmc2;
static PokeConProtocol *pokecon;
static NxamfBytesProtocolInterface *protocols[2];
static NxamfProtocolMultiplexer *mux;
static NxamfBytesBuffer *buffer;

static int64_t led_off(alarm_id_t id, void *user_data)
{
    sml_lx0404siupgusb_off(led);
    return false;
}

static void async_led_on_for_100ms()
{
    sml_lx0404siupgusb_on(led);
    alarm_id_t alarm_id = add_alarm_in_ms(100, led_off, NULL, false);
}

static void reflect_state(NxamfGamepadState *state)
{
    if (state == NULL)
    {
        return;
    }

    uint16_t r = SML_LX0404SIUPGUSB_OFF;
    uint16_t g = SML_LX0404SIUPGUSB_OFF;
    uint16_t b = SML_LX0404SIUPGUSB_OFF;

    if (state->y == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_y);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_y);
    }

    if (state->b == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_b);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_b);
    }

    if (state->a == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_a);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_a);
    }

    if (state->x == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_x);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_x);
    }

    if (state->l == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_l);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_l);
    }

    if (state->r == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_r);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_r);
    }

    if (state->minus == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_select);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_select);
    }

    if (state->plus == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_start);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_start);
    }

    if (state->home == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_home);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_home);
    }

    if (state->l_click == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_power);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_power);
    }

    if (state->r_click == NXAMF_BUTTON_STATE_PRESSED)
    {
        qpi_button_hold(btn_wireless);
        r = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_button_release(btn_wireless);
    }

    switch (state->hat)
    {
    case NXAMF_HAT_STATE_UP:
        qpi_hat_hold(hat, QPI_HAT_UP);
        r = SML_LX0404SIUPGUSB_ON;
        g = SML_LX0404SIUPGUSB_ON;
        break;
    case NXAMF_HAT_STATE_UPRIGHT:
        qpi_hat_hold(hat, QPI_HAT_UPRIGHT);
        r = SML_LX0404SIUPGUSB_ON;
        g = SML_LX0404SIUPGUSB_ON;
        break;
    case NXAMF_HAT_STATE_RIGHT:
        qpi_hat_hold(hat, QPI_HAT_RIGHT);
        r = SML_LX0404SIUPGUSB_ON;
        g = SML_LX0404SIUPGUSB_ON;
        break;
    case NXAMF_HAT_STATE_DOWNRIGHT:
        qpi_hat_hold(hat, QPI_HAT_DOWNRIGHT);
        r = SML_LX0404SIUPGUSB_ON;
        g = SML_LX0404SIUPGUSB_ON;
        break;
    case NXAMF_HAT_STATE_DOWN:
        qpi_hat_hold(hat, QPI_HAT_DOWN);
        r = SML_LX0404SIUPGUSB_ON;
        g = SML_LX0404SIUPGUSB_ON;
        break;
    case NXAMF_HAT_STATE_DOWNLEFT:
        qpi_hat_hold(hat, QPI_HAT_DOWNLEFT);
        r = SML_LX0404SIUPGUSB_ON;
        g = SML_LX0404SIUPGUSB_ON;
        break;
    case NXAMF_HAT_STATE_LEFT:
        qpi_hat_hold(hat, QPI_HAT_LEFT);
        r = SML_LX0404SIUPGUSB_ON;
        g = SML_LX0404SIUPGUSB_ON;
        break;
    case NXAMF_HAT_STATE_UPLEFT:
        qpi_hat_hold(hat, QPI_HAT_UPLEFT);
        r = SML_LX0404SIUPGUSB_ON;
        g = SML_LX0404SIUPGUSB_ON;
        break;
    case NXAMF_HAT_STATE_NEUTRAL:
    default:
        qpi_hat_release(hat);
        break;
    }

    if (state->l_stick.x != NXAMF_STICK_STATE_NEUTRAL || state->l_stick.y != NXAMF_STICK_STATE_NEUTRAL)
    {
        qpi_slidepad_hold(sp, (uint16_t)(((double)state->l_stick.x / UINT8_MAX) * UINT16_MAX), (uint16_t)(((double)state->l_stick.y / UINT8_MAX) * UINT16_MAX));
        g = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_slidepad_release(sp);
    }

    uint16_t ts_x = state->extension[0] | state->extension[1] << 8;
    uint8_t ts_y = state->extension[2];
    if (0 < ts_x && ts_x <= TOUCHSCREEN_X_MAX && 0 < ts_y && ts_y <= TOUCHSCREEN_Y_MAX)
    {
        qpi_touchscreen_hold(ts, (uint16_t)(((double)ts_x / TOUCHSCREEN_X_MAX) * UINT16_MAX), (uint16_t)(((double)ts_y / TOUCHSCREEN_Y_MAX) * UINT16_MAX));
        b = SML_LX0404SIUPGUSB_ON;
    }
    else
    {
        qpi_touchscreen_release(ts);
    }

    sml_lx0404siupgusb_set(led, r, g, b);
    async_led_on_for_100ms();
}

void setup()
{
    Serial.begin(9600);

    pinMode(PIN_Y, INPUT);
    gpio_y = gpio_adapter_new(PIN_Y);
    btn_y = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_y);
    pinMode(PIN_B, INPUT);
    gpio_b = gpio_adapter_new(PIN_B);
    btn_b = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_b);
    pinMode(PIN_A, INPUT);
    gpio_a = gpio_adapter_new(PIN_A);
    btn_a = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_a);
    pinMode(PIN_X, INPUT);
    gpio_x = gpio_adapter_new(PIN_X);
    btn_x = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_x);
    pinMode(PIN_L, INPUT);
    gpio_l = gpio_adapter_new(PIN_L);
    btn_l = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_l);
    pinMode(PIN_R, INPUT);
    gpio_r = gpio_adapter_new(PIN_R);
    btn_r = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_r);
    pinMode(PIN_SELECT, INPUT);
    gpio_select = gpio_adapter_new(PIN_SELECT);
    btn_select = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_select);
    pinMode(PIN_START, INPUT);
    gpio_start = gpio_adapter_new(PIN_START);
    btn_start = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_start);
    pinMode(PIN_HOME, INPUT);
    gpio_home = gpio_adapter_new(PIN_HOME);
    btn_home = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_home);
    pinMode(PIN_POWER, INPUT);
    gpio_power = gpio_adapter_new(PIN_POWER);
    btn_power = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_power);
    pinMode(PIN_WIRELESS, INPUT);
    gpio_wireless = gpio_adapter_new(PIN_WIRELESS);
    btn_wireless = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_wireless);

    pinMode(PIN_UP, INPUT);
    gpio_up = gpio_adapter_new(PIN_UP);
    hat_up = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_up);
    pinMode(PIN_RIGHT, INPUT);
    gpio_right = gpio_adapter_new(PIN_RIGHT);
    hat_right = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_right);
    pinMode(PIN_DOWN, INPUT);
    gpio_down = gpio_adapter_new(PIN_DOWN);
    hat_down = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_down);
    pinMode(PIN_LEFT, INPUT);
    gpio_left = gpio_adapter_new(PIN_LEFT);
    hat_left = qpi_button_new((QpiGeneralPurposeIOInterface *)gpio_left);
    hat = qpi_hat_new(hat_up, hat_right, hat_down, hat_left);

    Wire1.setSDA(PIN_I2C_SDA);
    Wire1.setSCL(PIN_I2C_SCL);
    Wire1.begin();
    dac = ds4432_new(&Wire1);
    dac_v = ds4432_adapter_new(dac, DS4432_OUT0);
    dac_h = ds4432_adapter_new(dac, DS4432_OUT1);
    sp = qpi_slidepad_new((QpiCurrentDAConverterInterface *)dac_v, (QpiCurrentDAConverterInterface *)dac_h);

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
    ts = qpi_touchscreen_new((QpiDigitalPotentiometerInterface *)pots_v, (QpiDigitalPotentiometerInterface *)pots_h, (QpiSPSTSwitchInterface *)sw_adapter);

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
    mux = nxamf_protocol_multiplexer_new(protocols, 2);
    buffer = nxamf_bytes_buffer_new((NxamfBytesProtocolInterface *)mux);
    assert(
        nxmc2 != NULL &&
        pokecon != NULL &&
        mux != NULL &&
        buffer != NULL);

    analogWriteResolution(ANALOG_WRITE_RESOLUTION);
    pinMode(PIN_LED_ANODE, OUTPUT_8MA);
    digitalWrite(PIN_LED_ANODE, LOW);
    anode = sml_lx0404siupgusb_arduino_digital_adapter_new(PIN_LED_ANODE);
    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_RED, HIGH);
    pwm_red = sml_lx0404siupgusb_arduino_analog_adapter_new(PIN_LED_RED, ANALOG_WRITE_RESOLUTION);
    pinMode(PIN_LED_GREEN, OUTPUT);
    digitalWrite(PIN_LED_GREEN, HIGH);
    pwm_green = sml_lx0404siupgusb_arduino_analog_adapter_new(PIN_LED_GREEN, ANALOG_WRITE_RESOLUTION);
    pinMode(PIN_LED_BLUE, OUTPUT);
    digitalWrite(PIN_LED_BLUE, HIGH);
    pwm_blue = sml_lx0404siupgusb_arduino_analog_adapter_new(PIN_LED_BLUE, ANALOG_WRITE_RESOLUTION);
    led = sml_lx0404siupgusb_new((SML_LX0404SIUPGUSBDigitalOutputInterface *)anode,
                                 (SML_LX0404SIUPGUSBAnalogOutputInterface *)pwm_red,
                                 (SML_LX0404SIUPGUSBAnalogOutputInterface *)pwm_green,
                                 (SML_LX0404SIUPGUSBAnalogOutputInterface *)pwm_blue);
    assert(
        anode != NULL &&
        pwm_red != NULL &&
        pwm_green != NULL &&
        pwm_blue != NULL &&
        led != NULL);

    // Lights up in rainbow colors if the setup routine is successful.
    for (int hue = 0; hue < 2000; hue++)
    {
        CtpHSV16 *hsv = ctp_hsv_16_new(((double)hue / 2000) * UINT16_MAX, UINT16_MAX, UINT16_MAX);
        assert(hsv != NULL);

        CtpRGB16 *rgb = ctp_hsv_16_to_rgb_16(hsv);
        assert(rgb != NULL);
        ctp_hsv_16_delete(hsv);
        hsv = NULL;

        sml_lx0404siupgusb_set(led, rgb->red, rgb->green, rgb->blue);
        sml_lx0404siupgusb_on(led);
        delay(1);

        ctp_rgb_16_delete(rgb);
        rgb = NULL;
    }
    sml_lx0404siupgusb_off(led);
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
