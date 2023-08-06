#pragma once

#include <Arduino.h>
#include <SPI.h>

typedef struct
{
    HardwareSPI *spi;
    pin_size_t cs;
    int8_t ldac = -1;
    int8_t shdn = -1;
} mcp4922_t;

typedef enum
{
    MCP4922_DAC_A,
    MCP4922_DAC_B
} mcp4922_selection_t;

typedef enum
{
    MCP4922_UNBUFFERED,
    MCP4922_BUFFERED
} mcp4922_buffer_control_t;

typedef enum
{
    MCP4922_2X,
    MCP4922_1X
} mcp4922_gain_selection_t;

typedef enum
{
    MCP4922_SHUTDOWN,
    MCP4922_ACTIVE
} mcp4922_shutdown_control_t;

void mcp4922_set(mcp4922_t *dac, mcp4922_selection_t ab, mcp4922_buffer_control_t buf, mcp4922_gain_selection_t ga, mcp4922_shutdown_control_t shdn, uint16_t data)
{
    if (4095 < data)
    {
        return;
    }
    uint16_t d = (ab << 15) | (buf << 14) | (ga << 13) | (shdn << 12) | data;
    digitalWrite(dac->cs, LOW);
    dac->spi->transfer16(d);
    digitalWrite(dac->cs, HIGH);
}

void mcp4922_latch(mcp4922_t *dac)
{
    if (dac->ldac == -1)
    {
        return;
    }
    digitalWrite(dac->ldac, HIGH);
}

void mcp4922_unlatch(mcp4922_t *dac)
{
    if (dac->ldac == -1)
    {
        return;
    }
    digitalWrite(dac->ldac, LOW);
}

void mcp4922_shutdown(mcp4922_t *dac)
{
    if (dac->shdn == -1)
    {
        return;
    }
    digitalWrite(dac->shdn, LOW);
}

void mcp4922_power_on(mcp4922_t *dac)
{
    if (dac->shdn == -1)
    {
        return;
    }
    digitalWrite(dac->shdn, HIGH);
}

void mcp4922_init(mcp4922_t *dac, HardwareSPI *spi, pin_size_t cs, int8_t ldac = -1, int8_t shdn = -1)
{
    dac->spi = spi;
    dac->cs = cs;
    pinMode(dac->cs, OUTPUT);
    digitalWrite(dac->cs, HIGH);
    dac->ldac = ldac;
    if (dac->ldac != -1)
    {
        pinMode(dac->ldac, OUTPUT);
    }
    dac->shdn = shdn;
    if (dac->shdn != -1)
    {
        pinMode(dac->shdn, OUTPUT);
    }
}