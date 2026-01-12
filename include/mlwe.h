#ifndef MLWE_H
#define MLWE_H

#include "poly.h"

typedef struct {
    poly a;
    poly b;
} mlwe_pk;

typedef struct {
    poly s;
} mlwe_sk;

void mlwe_keygen(mlwe_pk *pk, mlwe_sk *sk);
void mlwe_encrypt(poly *u, poly *v, const mlwe_pk *pk, const poly *msg);
void mlwe_decrypt(poly *msg, const mlwe_sk *sk, const poly *u, const poly *v);

#endif