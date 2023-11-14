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

static button_t btn_y;
static button_t btn_b;
static button_t btn_a;
static button_t btn_x;
static button_t btn_l;
static button_t btn_r;
static button_t btn_select;
static button_t btn_start;
static button_t btn_home;
static button_t btn_power;
static button_t btn_wifi;
void handle_y(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_y);
    else
        button_release(&btn_y);
}
void handle_b(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_b);
    else
        button_release(&btn_b);
}
void handle_a(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_a);
    else
        button_release(&btn_a);
}
void handle_x(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_x);
    else
        button_release(&btn_x);
}
void handle_l(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_l);
    else
        button_release(&btn_l);
}
void handle_r(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_r);
    else
        button_release(&btn_r);
}
void handle_select(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_select);
    else
        button_release(&btn_select);
}
void handle_start(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_start);
    else
        button_release(&btn_start);
}
void handle_home(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_home);
    else
        button_release(&btn_home);
}
void handle_power(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_power);
    else
        button_release(&btn_power);
}
void handle_wifi(nxmc2_button_t state)
{
    if (state == NXMC2_BUTTON_PRESSED)
        button_hold(&btn_wifi);
    else
        button_release(&btn_wifi);
}

static button_t hat_up;
static button_t hat_right;
static button_t hat_down;
static button_t hat_left;
static hat_t hat;
void handle_hat(nxmc2_hat_t state)
{
    hat_hold(&hat, (hat_direction_t)state);
}

static DS4432 *dac;
static slidepad_t sp;
void handle_slidepad(uint8_t x, uint8_t y)
{
    if (x != NXMC2_STICK_NEUTRAL || y != NXMC2_STICK_NEUTRAL)
        slidepad_hold(&sp, x, y);
    else
        slidepad_release(&sp);
}

static AD840X *pots;
static G3VM_XWR relay;
static touchscreen_t ts;
void handle_touchscreen(uint8_t x_low, uint8_t x_high, uint8_t y)
{
    uint16_t x = x_low | x_high << 8;
    if (0 < x && x <= 320 && 0 < y && y <= 240)
        touchscreen_hold(&ts, x, y);
    else
        touchscreen_release(&ts);
}

static nxmc2_builder_t builder;
static nxmc2_handlers_t handlers;

void setup()
{
    Serial.begin(9600);

    Wire.setSDA(0);
    Wire.setSCL(1);
    Wire.begin();

    SPI.setSCK(2);
    SPI.setTX(3);
    // SPI.setCS(5);
    SPI.begin();

    button_init(&btn_y, 13);
    button_init(&btn_b, 11);
    button_init(&btn_a, 10);
    button_init(&btn_x, 12);
    button_init(&btn_l, 9);
    button_init(&btn_r, 14);
    button_init(&btn_select, 19);
    button_init(&btn_start, 20);
    button_init(&btn_home, 18);
    button_init(&btn_power, 16);
    button_init(&btn_wifi, 22);

    button_init(&hat_up, 7);
    button_init(&hat_right, 17);
    button_init(&hat_down, 21);
    button_init(&hat_left, 8);
    hat_init(&hat, &hat_up, &hat_right, &hat_down, &hat_left);

    dac = ds4432_new(&Wire);
    slidepad_init(&sp, dac);

    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    pots = ad840x_new(&SPI, 5, 4);
    g3vm_xwr_new(&relay, 6);
    touchscreen_init(&ts, pots, &relay);

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
    handlers.r_click = handle_wifi;
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
