#include <stdio.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "3ds/button.h"
#include "3ds/slidepad.h"
#include "3ds/touchscreen.h"

void button_init(button_t &btn, uint pin)
{
    btn.write_digital_low = [pin]()
    {
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, false);
    };
    btn.power_off = [pin]()
    {
        gpio_set_dir(pin, GPIO_IN);
    };
}

int main()
{
    bi_decl(bi_program_description("3ds-bss firmware"));

    stdio_init_all();

    constexpr uint PIN_A = 6;
    button_t button_a;
    gpio_init(PIN_A);
    button_init(button_a, PIN_A);
    bi_decl(bi_1pin_with_name(PIN_A, "Button A"));

    while (1)
    {
        button_press(button_a);
        sleep_ms(1000);
        button_release(button_a);
        sleep_ms(1000);
    }
}
