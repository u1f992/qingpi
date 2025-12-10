#include "nxamf/pokecon.h"

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "re.h"

static const char *POKECON_PROTOCOL_ALLOWED[18] = {
    // 使えなさそうなもの
    // - |
    // - ()

    // ボタンとスティックは2通りのフォーマットがあるらしい
    // 0-ffff（画面クリックでスティック操作時）
    // 0x0000-0xffff（その他）
    // 少し不正確だけど0?x?と、0xの後に続かないのを許容するパターンでお茶を濁す

    "^0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[0-9a-f] ?$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[0-9a-f] [0-8]$",

    // 下位2bitは0b00で確定
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[048c] [0-8]\r$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[048c] [0-8]\r?\n$",

    // 下位2bitのどちらかが0b1
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] $",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] ?$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]$",

    // 下位2bitは0b10で確定 [26ae]
    // => しない。"Use LStick Mouse"、"Use RStick Mouse"を有効にすると、キャプチャ画面をクリックした際のbtnsの下位2ビットが、どちらかしか操作していない場合でも0b11になる。
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]\r$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]\r?\n$",

    // 最下位bitが0b1
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] $",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] ?$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]\r?$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]\r?\n$"};

static bool pokecon_protocol_is_acceptable(NxamfBytesProtocolInterface *parent, const uint8_t packet, const uint8_t buffer[], const size_t length)
{
    PokeConProtocol *self = (PokeConProtocol *)parent;

    char combined[length + 2]; // buffer + packet + '\0'
    memcpy(combined, buffer, length);
    combined[length] = (char)packet;
    combined[length + 1] = '\0';

    int _;
    for (int i = 0; i < 18; i++)
    {
        if (0 <= re_match(POKECON_PROTOCOL_ALLOWED[i], combined, &_))
        {
            return true;
        }
    }
    return false;
}

static bool pokecon_protocol_is_ready(NxamfBytesProtocolInterface *parent, const uint8_t buffer[], const size_t length)
{
    PokeConProtocol *self = (PokeConProtocol *)parent;

    char stringified[length + 1]; // buffer + '\0'
    memcpy(stringified, buffer, length);
    stringified[length] = '\0';

    size_t len = strlen(stringified);
    if (len < 1)
    {
        return false;
    }

    return stringified[len - 1] == '\n';
}

static void remove_prefix_0x(char *str)
{
    // "0x"はプレフィックスとしてしか存在しないはず
    char *p = str;
    while ((p = strstr(p, "0x")) != NULL)
    {
        memmove(p, p + 2, strlen(p + 2) + 1); // remove "0x" by moving the rest of the string over it
    }
}

static void pokecon_protocol_convert(NxamfBytesProtocolInterface *parent, const uint8_t buffer[], const size_t length, NxamfGamepadState *state)
{
    PokeConProtocol *self = (PokeConProtocol *)parent;

    char stringified[length + 1]; // buffer + '\0'
    memcpy(stringified, buffer, length);
    stringified[length] = '\0';

    remove_prefix_0x(stringified);

    uint16_t btns = 0;
    uint8_t hat = NXAMF_HAT_STATE_NEUTRAL;
    uint8_t lx = NXAMF_STICK_STATE_NEUTRAL;
    uint8_t ly = NXAMF_STICK_STATE_NEUTRAL;
    uint8_t rx = NXAMF_STICK_STATE_NEUTRAL;
    uint8_t ry = NXAMF_STICK_STATE_NEUTRAL;
    sscanf(stringified, "%hx %hhx %hhx %hhx %hhx %hhx", &btns, &hat, &lx, &ly, &rx, &ry);

    state->y = (btns & 0b0000000000000100) >> 2;
    state->b = (btns & 0b0000000000001000) >> 3;
    state->a = (btns & 0b0000000000010000) >> 4;
    state->x = (btns & 0b0000000000100000) >> 5;
    state->l = (btns & 0b0000000001000000) >> 6;
    state->r = (btns & 0b0000000010000000) >> 7;
    state->zl = (btns & 0b0000000100000000) >> 8;
    state->zr = (btns & 0b0000001000000000) >> 9;
    state->minus = (btns & 0b0000010000000000) >> 10;
    state->plus = (btns & 0b0000100000000000) >> 11;
    state->l_click = (btns & 0b0001000000000000) >> 12;
    state->r_click = (btns & 0b0010000000000000) >> 13;
    state->home = (btns & 0b0100000000000000) >> 14;
    state->capture = (btns & 0b1000000000000000) >> 15;

    state->hat = hat <= NXAMF_HAT_STATE_NEUTRAL ? (NxamfHatState)hat : NXAMF_HAT_STATE_NEUTRAL;

    bool update_ls = ((btns & 0b0000000000000010) >> 1) == 1;
    if (update_ls)
    {
        state->l_stick.x = lx;
        state->l_stick.y = ly;

        self->prev_lx = lx;
        self->prev_ly = ly;
    }
    else
    {
        state->l_stick.x = self->prev_lx;
        state->l_stick.y = self->prev_ly;
    }

    bool update_rs = (btns & 0b0000000000000001) == 1;
    if (update_rs)
    {
        state->r_stick.x = rx;
        state->r_stick.y = ry;

        self->prev_rx = rx;
        self->prev_ry = ry;
    }
    else
    {
        state->r_stick.x = self->prev_rx;
        state->r_stick.y = self->prev_ry;
    }
}

PokeConProtocol *pokecon_protocol_new(void)
{
    PokeConProtocol *self = (PokeConProtocol *)malloc(sizeof(PokeConProtocol));
    if (self == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < 18; i++)
    {
        if (re_compile(POKECON_PROTOCOL_ALLOWED[i]) == NULL)
        {
            free(self);
            return NULL;
        }
    }

    self->parent.is_acceptable = pokecon_protocol_is_acceptable;
    self->parent.is_ready = pokecon_protocol_is_ready;
    self->parent.convert = pokecon_protocol_convert;

    self->prev_lx = NXAMF_STICK_STATE_NEUTRAL;
    self->prev_ly = NXAMF_STICK_STATE_NEUTRAL;
    self->prev_rx = NXAMF_STICK_STATE_NEUTRAL;
    self->prev_ry = NXAMF_STICK_STATE_NEUTRAL;

    return self;
}

void pokecon_protocol_delete(PokeConProtocol *self)
{
    if (self == NULL)
    {
        return;
    }

    free(self);
}