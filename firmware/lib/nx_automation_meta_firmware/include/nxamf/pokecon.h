#ifndef POKECON_PROTOCOL_H_
#define POKECON_PROTOCOL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../nxamf.h"

typedef struct PokeConProtocol
{
    NxamfBytesProtocolInterface parent;

    uint8_t prev_lx;
    uint8_t prev_ly;
    uint8_t prev_rx;
    uint8_t prev_ry;
} PokeConProtocol;

PokeConProtocol *pokecon_protocol_new(void);
void pokecon_protocol_delete(PokeConProtocol *self);

#ifdef __cplusplus
}
#endif

#endif // POKECON_PROTOCOL_H_