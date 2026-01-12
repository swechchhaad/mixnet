#include <stdlib.h>
#include "mlwe.h"
#include "params.h"

static int sample_noise(void) {
    return (rand() % (2*ETA + 1)) - ETA;
}

static void sample_small_poly(poly *p) {
    for (int i = 0; i < N; i++) {
        p->coeffs[i] = sample_noise();
    }
}

void mlwe_keygen(mlwe_pk *pk, mlwe_sk *sk) {
    poly e;

    // sample a uniformly random a
    for (int i = 0; i < N; i++) {
        pk->a.coeffs[i] = rand() % Q;
    }

    sample_small_poly(&sk->s);
    sample_small_poly(&e);

    poly_mul(&pk->b, &pk->a, &sk->s);
    poly_add(&pk->b, &pk->b, &e);
}

void mlwe_encrypt(poly *u, poly *v,
                  const mlwe_pk *pk,
                  const poly *msg) {

    poly r, e1, e2;

    sample_small_poly(&r);
    sample_small_poly(&e1);
    sample_small_poly(&e2);

    poly_mul(u, &pk->a, &r);
    poly_add(u, u, &e1);

    poly_mul(v, &pk->b, &r);
    poly_add(v, v, &e2);
    poly_add(v, v, msg);
}

void mlwe_decrypt(poly *msg,
                  const mlwe_sk *sk,
                  const poly *u,
                  const poly *v) {

    poly tmp;

    poly_mul(&tmp, &sk->s, u);
    poly_sub(msg, v, &tmp);
}