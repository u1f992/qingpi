#include "3ds.h"

#include "pico/platform.h"
#include "pico/time.h"
#include "hardware/adc.h"
#include "hardware/irq.h"

#include "queue.h"

static inline void set_timeout_us(int64_t (*fn)(alarm_id_t, void *), uint64_t us)
{
    add_alarm_in_us(us, fn, NULL, false);
}

// https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#rpip0daa31111f2d33a9fd08
#define ADC_CLKDIV 95
static inline void adc_set_callback(void (*fn)())
{
    /*
     * A/D変換後に割り込む関数をセットします。
     * ラウンドロビンとかは考えない仕様。
     */
    adc_select_input(1);
    adc_set_clkdiv(ADC_CLKDIV);
    // fifo, no dma, length 1, discard error bit, 16bit
    adc_fifo_setup(true, false, 1, false, false);

    irq_set_exclusive_handler(ADC_IRQ_FIFO, fn);
    irq_set_enabled(ADC_IRQ_FIFO, true);

    adc_irq_set_enabled(true);
}

static inline void adc_resume()
{
    adc_run(true);
}

static inline void adc_pause()
{
    adc_run(false);
}

static inline void write_x(double x)
{
}

static inline void write_y(double y)
{
}

static inline double read_x()
{
}

static inline void power_off_x()
{
}

static inline void power_off_y()
{
}

typedef struct
{
    bool pressing;
    queue_t *q;

    double x_volt;
    double y_volt;
} touchscreen_t;
static touchscreen_t ts;

static inline void set_coord(uint16_t x, uint8_t y)
{
    ts.x_volt = x / 320 * _3DS_LOGIC_LEVEL;
    ts.y_volt = y / 240 * _3DS_LOGIC_LEVEL;
}

int64_t __time_critical_func(rewind_x_y_)(alarm_id_t _1, void *_2)
{
    if (!ts.pressing)
    {
        adc_pause();        
        
        write_x(0);
        power_off_x();
        power_off_y();

        queue_clear(ts.q);
        return;
    }

    if (!queue_empty(ts.q))
    {
        uint32_t xy;
        dequeue(ts.q, &xy);
        uint16_t x = (uint16_t)(xy >> 16);
        uint8_t y = (uint8_t)xy;
        set_coord(x, y);
    }

    write_x(ts.x_volt);
    write_y(0);
    adc_resume();
}

int64_t __time_critical_func(write_x_y_)(alarm_id_t _1, void *_2)
{
    write_x(_3DS_LOGIC_LEVEL);
    write_y(_3DS_LOGIC_LEVEL);
    set_timeout_us(rewind_x_y_, 200);
}

int64_t __time_critical_func(write_y_)(alarm_id_t _1, void *_2)
{
    write_y(ts.y_volt);
    set_timeout_us(write_x_y_, 200);
}

void __time_critical_func(wait_x_to_respond_)(alarm_id_t _1, void *_2)
{
    double current_x = read_x();
    if (current_x < ts.x_volt * 0.99)
    {
        return;
    }
    adc_pause();
    set_timeout_us(write_y_, 500);
}

void touchscreen_init()
{
    ts.pressing = false;
    ts.q = queue_init();

    adc_set_callback(wait_x_to_respond_);
    adc_pause();
}

void touchscreen_hold(uint16_t x, uint8_t y)
{
    assert(0 < x && x <= 320 && 0 < y && y <= 240);

    if (ts.pressing)
    {
        enqueue(ts.q, (uint32_t)(x) << 16 | y);
        return;
    }
    ts.pressing = true;

    set_coord(x, y);
    write_x(ts.x_volt);
    write_y(ts.y_volt);
    adc_resume();
}

void touchscreen_release()
{
    ts.pressing = false;
}