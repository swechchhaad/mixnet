#include "poly.h"

void poly_zero(poly *p) {
    for (int i = 0; i < N; i++) {
        p->coeffs[i] = 0;
    }
}

void poly_copy(poly *r, const poly *a) {
    for (int i = 0; i < N; i++) {
        r->coeffs[i] = a->coeffs[i];
    }
}

int poly_equal(const poly *a, const poly *b) {
    for (int i = 0; i < N; i++) {
        if (a->coeffs[i] != b->coeffs[i]) return 0;
    }
    return 1;
}

void poly_mod(poly *p) {
    for (int i = 0; i < N; i++) {
        p->coeffs[i] %= Q;
        if (p->coeffs[i] < 0)
            p->coeffs[i] += Q;
    }
}

void poly_add(poly *r, const poly *a, const poly *b) {
    for (int i = 0; i < N; i++) {
        r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
    }
    poly_mod(r);
}

void poly_sub(poly *r, const poly *a, const poly *b) {
    for (int i = 0; i < N; i++) {
        r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
    }
    poly_mod(r);
}
void poly_mul(poly *r, const poly *a, const poly *b) {
    long tmp[N] = {0};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int k = i + j;
            long prod = (long)a->coeffs[i] * b->coeffs[j];
            if (k < N) {
                tmp[k] += prod;
            } else {
                tmp[k - N] -= prod; 
            }
        }
    }

    for (int i = 0; i < N; i++) {
        long v = tmp[i] % Q;
        if (v < 0) v += Q;
        r->coeffs[i] = (int)v;
    }
}

int poly_center(int x) {
    x %= Q;
    if (x < 0) x += Q;
    if (x > Q / 2) x -= Q;
    return x;
}
