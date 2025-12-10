#ifndef NXMC2_PROTOCOL_H_
#define NXMC2_PROTOCOL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../nxamf.h"

typedef struct Nxmc2Protocol
{
    NxamfBytesProtocolInterface parent;
} Nxmc2Protocol;

Nxmc2Protocol *nxmc2_protocol_new(void);
void nxmc2_protocol_delete(Nxmc2Protocol *self);

#ifdef __cplusplus
}
#endif

#endif // NXMC2_PROTOCOL_H_