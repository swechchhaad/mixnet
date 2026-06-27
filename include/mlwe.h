#ifndef MLWE_H
#define MLWE_H

#include "poly.h"
typedef struct {
    poly A[KLWE][KLWE];   // public matrix A
    poly t[KLWE];         // t = A s + e1
} mlwe_pk;

typedef struct {
    poly s[KLWE];         // secret key
} mlwe_sk;

typedef struct {
    poly u[KLWE];
    poly v;
} mlwe_ct;

void mlwe_keygen(mlwe_pk *pk, mlwe_sk *sk);

void mlwe_encrypt(mlwe_ct *ct, const mlwe_pk *pk, const poly *msg);

void mlwe_decrypt(poly *msg, const mlwe_sk *sk, const mlwe_ct *ct);

#endif
