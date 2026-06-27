#ifndef HPKE_H
#define HPKE_H

#include "poly.h"
#include "mlwe.h"
#include "otse.h"

typedef struct {
    mlwe_ct cseed[KLWR];
    poly   *cstar; 
    int     L;
} hpke_ct;

void hpke_keygen(mlwe_pk *pk, mlwe_sk *sk);

void hpke_enc(hpke_ct *ct, const otse_pp *pp, const mlwe_pk *pk,
              const poly *msg, int L);

void hpke_dec(poly *msg, const otse_pp *pp, const mlwe_sk *sk, const hpke_ct *ct);

void hpke_ct_free(hpke_ct *ct);

#endif
