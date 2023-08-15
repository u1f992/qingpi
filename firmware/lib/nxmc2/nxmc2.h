#ifndef NXMC2_H_
#define NXMC2_H_

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    NXMC2_BUTTON_RELEASED,
    NXMC2_BUTTON_PRESSED
} nxmc2_button_t;

typedef enum
{
    NXMC2_HAT_UP,
    NXMC2_HAT_UPRIGHT,
    NXMC2_HAT_RIGHT,
    NXMC2_HAT_DOWNRIGHT,
    NXMC2_HAT_DOWN,
    NXMC2_HAT_DOWNLEFT,
    NXMC2_HAT_LEFT,
    NXMC2_HAT_UPLEFT,
    NXMC2_HAT_NEUTRAL
} nxmc2_hat_t;

const uint8_t NXMC2_STICK_NEUTRAL = 128U;

typedef union
{
    uint8_t raw[11];
    struct
    {
        uint8_t header;

        nxmc2_button_t y : 1;
        nxmc2_button_t b : 1;
        nxmc2_button_t a : 1;
        nxmc2_button_t x : 1;
        nxmc2_button_t l : 1;
        nxmc2_button_t r : 1;
        nxmc2_button_t zl : 1;
        nxmc2_button_t zr : 1;

        nxmc2_button_t minus : 1;
        nxmc2_button_t plus : 1;
        nxmc2_button_t l_click : 1;
        nxmc2_button_t r_click : 1;
        nxmc2_button_t home : 1;
        nxmc2_button_t capture : 1;
        uint8_t _ : 2;

        nxmc2_hat_t hat;
        uint8_t lx;
        uint8_t ly;
        uint8_t rx;
        uint8_t ry;

        uint8_t ext0;
        uint8_t ext1;
        uint8_t ext2;
    };
} nxmc2_command_t;

const uint8_t NXMC2_COMMAND_HEADER = 0xABU;

typedef struct
{
    nxmc2_command_t cmd_;
    size_t ptr_;
} nxmc2_builder_t;

inline void nxmc2_builder_clear(nxmc2_builder_t *builder)
{
    builder->ptr_ = 0;
}

inline bool nxmc2_builder_can_build(nxmc2_builder_t *builder)
{
    if (builder->ptr_ != (sizeof(builder->cmd_.raw) / sizeof(uint8_t)))
    {
        return false;
    }
    return true;
}

inline void nxmc2_builder_append(nxmc2_builder_t *builder, uint8_t packet)
{
    if ((builder->ptr_ == 0 && packet != NXMC2_COMMAND_HEADER) ||
        (builder->ptr_ == 3 && 8 < packet))
    {
        nxmc2_builder_clear(builder);
        return;
    }

    if (nxmc2_builder_can_build(builder))
    {
        nxmc2_builder_clear(builder);
    }
    builder->cmd_.raw[builder->ptr_] = packet;
    builder->ptr_++;
}

inline nxmc2_command_t *nxmc2_builder_build(nxmc2_builder_t *builder)
{
    assert(nxmc2_builder_can_build(builder));
    return &(builder->cmd_);
}

inline void nxmc2_builder_init(nxmc2_builder_t *builder)
{
    nxmc2_builder_clear(builder);
}

typedef struct
{
    void (*y)(nxmc2_button_t);
    void (*b)(nxmc2_button_t);
    void (*a)(nxmc2_button_t);
    void (*x)(nxmc2_button_t);
    void (*l)(nxmc2_button_t);
    void (*r)(nxmc2_button_t);
    void (*zl)(nxmc2_button_t);
    void (*zr)(nxmc2_button_t);
    void (*minus)(nxmc2_button_t);
    void (*plus)(nxmc2_button_t);
    void (*l_click)(nxmc2_button_t);
    void (*r_click)(nxmc2_button_t);
    void (*home)(nxmc2_button_t);
    void (*capture)(nxmc2_button_t);
    void (*hat)(nxmc2_hat_t);
    void (*l_stick)(uint8_t, uint8_t);
    void (*r_stick)(uint8_t, uint8_t);
    void (*ext)(uint8_t, uint8_t, uint8_t);
} nxmc2_handlers_t;

inline void nxmc2_command_exec(nxmc2_command_t *cmd, nxmc2_handlers_t *handlers)
{
    if (handlers->y != NULL)
    {
        handlers->y(cmd->y);
    }
    if (handlers->b != NULL)
    {
        handlers->b(cmd->b);
    }
    if (handlers->a != NULL)
    {
        handlers->a(cmd->a);
    }
    if (handlers->x != NULL)
    {
        handlers->x(cmd->x);
    }
    if (handlers->l != NULL)
    {
        handlers->l(cmd->l);
    }
    if (handlers->r != NULL)
    {
        handlers->r(cmd->r);
    }
    if (handlers->zl != NULL)
    {
        handlers->zl(cmd->zl);
    }
    if (handlers->zr != NULL)
    {
        handlers->zr(cmd->zr);
    }
    if (handlers->minus != NULL)
    {
        handlers->minus(cmd->minus);
    }
    if (handlers->plus != NULL)
    {
        handlers->plus(cmd->plus);
    }
    if (handlers->l_click != NULL)
    {
        handlers->l_click(cmd->l_click);
    }
    if (handlers->r_click != NULL)
    {
        handlers->r_click(cmd->r_click);
    }
    if (handlers->home != NULL)
    {
        handlers->home(cmd->home);
    }
    if (handlers->capture != NULL)
    {
        handlers->capture(cmd->capture);
    }
    if (handlers->hat != NULL)
    {
        handlers->hat(cmd->hat);
    }
    if (handlers->l_stick != NULL)
    {
        handlers->l_stick(cmd->lx, cmd->ly);
    }
    if (handlers->r_stick != NULL)
    {
        handlers->r_stick(cmd->rx, cmd->ry);
    }
    if (handlers->ext != NULL)
    {
        handlers->ext(cmd->ext0, cmd->ext1, cmd->ext2);
    }
}

#endif