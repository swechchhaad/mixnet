#include <stdlib.h>
#include "sample.h"

void rng_seed(unsigned int seed) {
    srand(seed);
}

static int rand_bit(void) {
    return rand() & 1;
}

void sample_uniform(poly *p) {
    for (int i = 0; i < N; i++) {
        p->coeffs[i] = rand() % Q;
    }
}

void sample_binomial(poly *p) {
    for (int i = 0; i < N; i++) {
        int acc = 0;
        for (int j = 0; j < ETA; j++) {
            acc += rand_bit();
            acc -= rand_bit();
        }
        int v = acc % Q;
        if (v < 0) v += Q;
        p->coeffs[i] = v;
    }
}

void sample_binary(poly *p) {
    for (int i = 0; i < N; i++) {
        p->coeffs[i] = rand_bit();
    }
}

void vec_uniform(poly *v, int len) {
    for (int i = 0; i < len; i++) sample_uniform(&v[i]);
}

void vec_binomial(poly *v, int len) {
    for (int i = 0; i < len; i++) sample_binomial(&v[i]);
}

void vec_binary(poly *v, int len) {
    for (int i = 0; i < len; i++) sample_binary(&v[i]);
}

void poly_round(poly *out, const poly *in, int q1, int q2) {
    for (int i = 0; i < N; i++) {
        int w = in->coeffs[i] % q1;
        if (w < 0) w += q1;
        long num = (long)w * q2;
        int r = (int)((num + q1 / 2) / q1);
        r %= q2;
        if (r < 0) r += q2;
        out->coeffs[i] = r;
    }
}
