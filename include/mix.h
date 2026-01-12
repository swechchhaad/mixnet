#ifndef MIX_H
#define MIX_H

#include <stddef.h>
#include "phase1.h"
#include "mlwe.h"

/*
 * Single-server shuffle:
 *  - randomly permutes ciphertexts
 *  - rerandomizes MLWE part
 *  - preserves plaintexts
 */
void mix_shuffle(ciphertext *out,
                 const ciphertext *in,
                 size_t n,
                 const mlwe_pk *pk);

#endif
