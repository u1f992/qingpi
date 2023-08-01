#include <Arduino.h>
#include <SPI.h>

#include "button.h"
#include "hat.h"
#include "slidepad.h"
#include "touchscreen.h"

button_t btn_y;
button_t btn_b;
button_t btn_a;
button_t btn_x;
button_t btn_l;
button_t btn_r;
// button_t btn_zl;
// button_t btn_zr;
button_t btn_select;
button_t btn_start;
// button_t btn_lclick;
// button_t btn_rclick;
button_t btn_home;
button_t btn_power;
button_t *btn_all[14] = {
    &btn_y,
    &btn_b,
    &btn_a,
    &btn_x,
    &btn_l,
    &btn_r,
    NULL,
    NULL,
    &btn_select,
    &btn_start,
    NULL,
    NULL,
    &btn_home,
    &btn_power};

button_t hat_up;
button_t hat_right;
button_t hat_down;
button_t hat_left;
hat_t hat;

mcp4922_t dac;
slidepad_t sp;

ad840x_t pots;
adg752_t sw;
g3vm_x1wr_t relay;
touchscreen_t ts;

void setup()
{
    Serial.begin(9600);

    SPI.setSCK(2);
    SPI.setTX(3);
    // SPI.setCS(1);
    // SPI.setCS(5);
    SPI.begin();

    mcp4922_init(&dac, &SPI, 1, 0, 4);
    slidepad_init(&sp, &dac);

    ad840x_init(&pots, &SPI, 5, 6);
    adg752_init(&sw, 7);
    g3vm_x1wr_init(&relay, 8);
    touchscreen_init(&ts, &pots, &sw, &relay);

    button_init(&btn_y, 9);
    button_init(&btn_b, 10);
    button_init(&btn_a, 11);
    button_init(&btn_x, 12);
    button_init(&btn_l, 13);
    button_init(&btn_r, 14);
    button_init(&btn_select, 15);
    button_init(&btn_start, 16);
    button_init(&btn_home, 17);
    button_init(&btn_power, 18);

    button_init(&hat_up, 19);
    button_init(&hat_right, 20);
    button_init(&hat_down, 21);
    button_init(&hat_left, 22);
    hat_init(&hat, &hat_up, &hat_right, &hat_down, &hat_left);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    delay(5000);
}

uint8_t data[11];
size_t len = sizeof(data) / sizeof(data[0]);
size_t ptr = 0;
#define SERIAL_TIMEOUT 100
int c = 0;

void loop()
{
    if (Serial.available() == 0)
    {
        c++;
        if (c == SERIAL_TIMEOUT)
        {
            c = 0;
            ptr = 0;
        }
        return;
    }
    c = 0;

    data[ptr] = Serial.read();
    if (ptr == 0 && data[0] != 0xAB)
    {
        return;
    }
    else if (ptr != len - 1)
    {
        ptr++;
        return;
    }
    ptr = 0;

    /**
     * @code {.unparsed}
     * | Button (NXMC)   | Mask                  |
     * | --------------- | --------------------- |
     * | Y               | 0b0000'0000'0000'0001 |
     * | B               | 0b0000'0000'0000'0010 |
     * | A               | 0b0000'0000'0000'0100 |
     * | X               | 0b0000'0000'0000'1000 |
     * | L               | 0b0000'0000'0001'0000 |
     * | R               | 0b0000'0000'0010'0000 |
     * | (ZL)            | 0b0000'0000'0100'0000 |
     * | (ZR)            | 0b0000'0000'1000'0000 |
     * | Select (-)      | 0b0000'0001'0000'0000 |
     * | Start (+)       | 0b0000'0010'0000'0000 |
     * | (L Click)       | 0b0000'0100'0000'0000 |
     * | (R Click)       | 0b0000'1000'0000'0000 |
     * | Home            | 0b0001'0000'0000'0000 |
     * | Power (Capture) | 0b0010'0000'0000'0000 |
     * @endcode
     */
    uint16_t btns = data[1] | data[2] << 8;
    /**
     * @code {.unparsed}
     * | Hat | Value |
     * | --- | ----- |
     * | ↑   | 0     |
     * | ↗   | 1     |
     * | →   | 2     |
     * | ↘   | 3     |
     * | ↓   | 4     |
     * | ↙   | 5     |
     * | ←   | 6     |
     * | ↖   | 7     |
     * | x   | 8     |
     * @endcode
     */
    uint8_t dir = data[3];
    uint8_t lx = data[4];
    uint8_t ly = data[5];
    // uint8_t rx = data[6];
    // uint8_t ry = data[7];
    // In NXMC these are used for keyboard input, but used for touchscreen here.
    uint8_t tx = data[8] | data[9] << 8;
    uint16_t ty = data[10];
    Serial.printf("btns:%u,dir:%u,lx:%u,ly:%u,tx:%u,ty:%u", btns, dir, lx, ly, tx, ty);

    for (int i = 0; i < 14; i++)
    {
        if (btn_all[i] == NULL)
        {
            continue;
        }

        if (btns & (1 << i))
        {
            button_hold(btn_all[i]);
        }
        else
        {
            button_release(btn_all[i]);
        }
    }

    if (dir <= 8)
    {
        hat_hold(&hat, (hat_direction_t)dir);
    }

    if (lx != 128 || ly != 128)
    {
        slidepad_hold(&sp, lx, ly);
    }
    else
    {
        slidepad_release(&sp);
    }

    if (0 < tx && 0 < ty)
    {
        touchscreen_hold(&ts, tx, ty);
    }
    else
    {
        touchscreen_release(&ts);
    }
}