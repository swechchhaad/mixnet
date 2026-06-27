#ifndef MIX_H
#define MIX_H

#include <stddef.h>
#include "poly.h"
#include "mlwe.h"
#include "otse.h"

#define SEED_RING (KLWR * (KLWE + 1))

typedef struct {
    poly *data;
    int   len;
} flatct;

void flatct_free(flatct *c);

int mix_layer_msglen(int rho, int layer);
otse_pp *mix_setup(int rho);

void mix_encrypt(flatct *out, const poly *m,
                 const mlwe_pk *pks, const otse_pp *pps, int rho);

void mix_run(flatct *list, size_t n, const mlwe_sk *sks,
             const otse_pp *pps, int rho);

#endif
