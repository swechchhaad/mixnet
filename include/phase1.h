#ifndef PHASE1_H
#define PHASE1_H

#include "poly.h"
#include "mlwe.h"

typedef struct {
    poly u;
    poly v;
    poly c_star;
} ciphertext;
void phase1_encrypt(ciphertext *ct,
                    const poly *message,
                    const mlwe_pk *pk);
void phase1_decrypt(poly *message,
                    const ciphertext *ct,
                    const mlwe_sk *sk);

#endif