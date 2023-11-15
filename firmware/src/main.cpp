#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "button.h"
#include "hat.h"
#include "slidepad.h"
#include "touchscreen.h"

#include "nxmc2.h"

#include "ncm.h"

#include "gpio_adapter.h"
#include "adg801.h"
#include "adg801_adapter.h"
#include "ds4432.h"
#include "ds4432_adapter.h"
#include "ad840x.h"
#include "ad840x_adapter.h"
#include "ad840x_triple_adapter.h"

#include "nxamf.h"
#include "nxamf/nxmc2.h"
#include "nxamf/pokecon.h"

static const pin_size_t PIN_Y = 2;
static const pin_size_t PIN_B = 3;
static const pin_size_t PIN_A = 4;
static const pin_size_t PIN_X = 5;
static const pin_size_t PIN_L = 6;
static const pin_size_t PIN_R = 7;
static const pin_size_t PIN_SELECT = 8;
static const pin_size_t PIN_START = 9;
static const pin_size_t PIN_HOME = 10;
static const pin_size_t PIN_POWER = 11;
static const pin_size_t PIN_WIRELESS = 12;
static const pin_size_t PIN_UP = 13;
static const pin_size_t PIN_RIGHT = 14;
static const pin_size_t PIN_DOWN = 15;
static const pin_size_t PIN_LEFT = 16;
// static const pin_size_t PIN_IRDA = 17;
static const pin_size_t PIN_SPI_SCK = 18;
static const pin_size_t PIN_SPI_TX = 19;
static const pin_size_t PIN_SPI_CS = 20;
static const pin_size_t PIN_POTS_SHDN = 21;
static const pin_size_t PIN_SW_IN = 22;
static const pin_size_t PIN_LED_BUILTIN = 25;
static const pin_size_t PIN_I2C_SDA = 26;
static const pin_size_t PIN_I2C_SCL = 27;

static GPIOAdapter *gpio_y = gpio_adapter_new(PIN_Y);
static GPIOAdapter *gpio_b = gpio_adapter_new(PIN_B);
static GPIOAdapter *gpio_a = gpio_adapter_new(PIN_A);
static GPIOAdapter *gpio_x = gpio_adapter_new(PIN_X);
static GPIOAdapter *gpio_l = gpio_adapter_new(PIN_L);
static GPIOAdapter *gpio_r = gpio_adapter_new(PIN_R);
static GPIOAdapter *gpio_select = gpio_adapter_new(PIN_SELECT);
static GPIOAdapter *gpio_start = gpio_adapter_new(PIN_START);
static GPIOAdapter *gpio_home = gpio_adapter_new(PIN_HOME);
static GPIOAdapter *gpio_power = gpio_adapter_new(PIN_POWER);
static GPIOAdapter *gpio_wireless = gpio_adapter_new(PIN_WIRELESS);
static NcmButton *btn_y = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_y);
static NcmButton *btn_b = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_b);
static NcmButton *btn_a = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_a);
static NcmButton *btn_x = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_x);
static NcmButton *btn_l = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_l);
static NcmButton *btn_r = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_r);
static NcmButton *btn_select = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_select);
static NcmButton *btn_start = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_start);
static NcmButton *btn_home = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_home);
static NcmButton *btn_power = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_power);
static NcmButton *btn_wireless = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_wireless);
void handle_y(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_y);
    else
        ncm_button_release(btn_y);
}
void handle_b(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_b);
    else
        ncm_button_release(btn_b);
}
void handle_a(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_a);
    else
        ncm_button_release(btn_a);
}
void handle_x(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_x);
    else
        ncm_button_release(btn_x);
}
void handle_l(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_l);
    else
        ncm_button_release(btn_l);
}
void handle_r(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_r);
    else
        ncm_button_release(btn_r);
}
void handle_select(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_select);
    else
        ncm_button_release(btn_select);
}
void handle_start(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_start);
    else
        ncm_button_release(btn_start);
}
void handle_home(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_home);
    else
        ncm_button_release(btn_home);
}
void handle_power(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_power);
    else
        ncm_button_release(btn_power);
}
void handle_wireless(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        ncm_button_hold(btn_wireless);
    else
        ncm_button_release(btn_wireless);
}

static GPIOAdapter *gpio_up = gpio_adapter_new(PIN_UP);
static GPIOAdapter *gpio_right = gpio_adapter_new(PIN_RIGHT);
static GPIOAdapter *gpio_down = gpio_adapter_new(PIN_DOWN);
static GPIOAdapter *gpio_left = gpio_adapter_new(PIN_UP);
static NcmButton *hat_up = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_up);
static NcmButton *hat_right = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_right);
static NcmButton *hat_down = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_down);
static NcmButton *hat_left = ncm_button_new((NcmGeneralPurposeIOInterface *)gpio_left);
static NcmHat *hat = ncm_hat_new(hat_up, hat_right, hat_down, hat_left);
void handle_hat(nxmc2_hat_t state)
{
    ncm_hat_hold(hat, (NcmHatDirection)state);
}

static DS4432 *dac = ds4432_new(&Wire1);
static DS4432Adapter *dac_v = ds4432_adapter_new(dac, DS4432_OUT1);
static DS4432Adapter *dac_h = ds4432_adapter_new(dac, DS4432_OUT0);
static NcmSlidePad *sp = ncm_slidepad_new((NcmCurrentDAConverterInterface *)dac_v, (NcmCurrentDAConverterInterface *)dac_h);
void handle_slidepad(uint8_t x, uint8_t y)
{
    if (x != NXMC2_STICK_NEUTRAL || y != NXMC2_STICK_NEUTRAL)
        ncm_slidepad_hold(sp, (double)x / 255, (double)y / 255);
    else
        ncm_slidepad_release(sp);
}

static AD840X *pots = ad840x_new(&SPI, PIN_SPI_CS, PIN_POTS_SHDN);
static AD840XAdapter *pots_v = ad840x_adapter_new(pots, AD840X_RDAC2);
static AD840XTripleAdapter *pots_h = ad840x_triple_adapter_new(pots, AD840X_RDAC3, AD840X_RDAC1, AD840X_RDAC4);
static ADG801 *sw = adg801_new(PIN_SW_IN);
static ADG801Adapter *sw_adapter = adg801_adapter_new(sw);
static NcmTouchScreen *ts = ncm_touchscreen_new((NcmDigitalPotentiometerInterface *)pots_v, (NcmDigitalPotentiometerInterface *)pots_v, (NcmSwitchInterface *)sw_adapter);
void handle_touchscreen(uint8_t x_low, uint8_t x_high, uint8_t y)
{
    uint16_t x = x_low | x_high << 8;
    if (0 < x && x <= 320 && 0 < y && y <= 240)
        ncm_touchscreen_hold(ts, (double)x / 320, (double)y / 240);
    else
        ncm_touchscreen_release(ts);
}

static nxmc2_builder_t builder;
static nxmc2_handlers_t handlers;

void setup()
{
    Serial.begin(9600);

    Wire1.setSDA(PIN_I2C_SDA);
    Wire1.setSCL(PIN_I2C_SCL);
    Wire1.begin();

    SPI.setSCK(PIN_SPI_SCK);
    SPI.setTX(PIN_SPI_TX);
    // SPI.setCS(PIN_SPI_CS);  // Not assigned to CS, a normal GPIO.
    pinMode(PIN_SPI_CS, OUTPUT);
    SPI.begin();

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

    nxmc2_builder_init(&builder);
    handlers.y = handle_y;
    handlers.b = handle_b;
    handlers.a = handle_a;
    handlers.x = handle_x;
    handlers.l = handle_l;
    handlers.r = handle_r;
    handlers.zl = NULL;
    handlers.zr = NULL;
    handlers.minus = handle_select;
    handlers.plus = handle_start;
    handlers.l_click = handle_power;
    handlers.r_click = handle_wireless;
    handlers.home = handle_home;
    handlers.capture = NULL;
    handlers.hat = handle_hat;
    handlers.l_stick = handle_slidepad;
    handlers.r_stick = NULL;
    handlers.ext = handle_touchscreen;

    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, LOW);

    delay(5000);
}

static const int SERIAL_TIMEOUT = 100;
static int cnt = 0;

void loop()
{
    if (Serial.available() == 0)
    {
        cnt++;
        if (cnt == SERIAL_TIMEOUT)
        {
            cnt = 0;
            nxmc2_builder_clear(&builder);
        }
        return;
    }
    cnt = 0;

    nxmc2_builder_append(&builder, Serial.read());
    if (!nxmc2_builder_can_build(&builder))
    {
        return;
    }
    nxmc2_command_t *cmd = nxmc2_builder_build(&builder);
    nxmc2_command_exec(cmd, &handlers);
    nxmc2_builder_clear(&builder);
}
