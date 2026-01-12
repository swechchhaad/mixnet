#include "poly.h"

static int round_q_to_p(int x) {
    // map x in [0, Q) to [0, P)
    return (x * P) / Q;
}

void poly_zero(poly *p) {
    for (int i = 0; i < N; i++) {
        p->coeffs[i] = 0;
    }
}

void poly_add(poly *r, const poly *a, const poly *b) {
    for (int i = 0; i < N; i++) {
        r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
    }
    poly_mod(r);
}

void poly_mod(poly *p) {
    for (int i = 0; i < N; i++) {
        p->coeffs[i] %= Q;
        if (p->coeffs[i] < 0)
            p->coeffs[i] += Q;
    }
}
void poly_sub(poly *r, const poly *a, const poly *b) {
    for (int i = 0; i < N; i++) {
        r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
    }
    poly_mod(r);
}
void poly_mul(poly *r, const poly *a, const poly *b) {
    int tmp[N] = {0};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tmp[(i + j) % N] += a->coeffs[i] * b->coeffs[j];
        }
    }

    for (int i = 0; i < N; i++) {
        r->coeffs[i] = tmp[i];
    }

    poly_mod(r);
}