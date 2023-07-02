#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>

#define _3DS_LOGIC_LEVEL (1.8)

typedef uint button_t;
void button_hold(button_t);
void button_release(button_t);

void slidepad_hold(uint8_t, uint8_t);
void slidepad_release();

void touchscreen_init();
void touchscreen_hold(uint16_t, uint8_t);
void touchscreen_release();
