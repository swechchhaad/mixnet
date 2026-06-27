#include "mlwe.h"
#include "sample.h"
#include "params.h"

static void poly_mul_acc(poly *r, const poly *a, const poly *b) {
    poly t;
    poly_mul(&t, a, b);
    poly_add(r, r, &t);
}

void mlwe_keygen(mlwe_pk *pk, mlwe_sk *sk) {
    poly e1[KLWE];

    for (int i = 0; i < KLWE; i++)
        for (int j = 0; j < KLWE; j++)
            sample_uniform(&pk->A[i][j]);

    vec_binomial(sk->s, KLWE);
    vec_binomial(e1, KLWE);

    /* t = A s + e1. */
    for (int i = 0; i < KLWE; i++) {
        poly_zero(&pk->t[i]);
        for (int j = 0; j < KLWE; j++)
            poly_mul_acc(&pk->t[i], &pk->A[i][j], &sk->s[j]);
        poly_add(&pk->t[i], &pk->t[i], &e1[i]);
    }
}

void mlwe_encrypt(mlwe_ct *ct, const mlwe_pk *pk, const poly *msg) {
    poly r[KLWE], e2[KLWE], e3;

    vec_binomial(r, KLWE);
    vec_binomial(e2, KLWE);
    sample_binomial(&e3);

    /* u^T = r^T A + e2^T, i.e. u[j] = sum_i r[i] * A[i][j] + e2[j]. */
    for (int j = 0; j < KLWE; j++) {
        poly_zero(&ct->u[j]);
        for (int i = 0; i < KLWE; i++)
            poly_mul_acc(&ct->u[j], &r[i], &pk->A[i][j]);
        poly_add(&ct->u[j], &ct->u[j], &e2[j]);
    }

    /* v = r^T t + e3 + round(q/2) m. */
    poly_zero(&ct->v);
    for (int i = 0; i < KLWE; i++)
        poly_mul_acc(&ct->v, &r[i], &pk->t[i]);
    poly_add(&ct->v, &ct->v, &e3);

    poly scaled;
    int half = (Q + 1) / 2;           // round(q/2)
    for (int i = 0; i < N; i++)
        scaled.coeffs[i] = (msg->coeffs[i] & 1) ? half : 0;
    poly_add(&ct->v, &ct->v, &scaled);
}

void mlwe_decrypt(poly *msg, const mlwe_sk *sk, const mlwe_ct *ct) {
    /* m~ = v - u^T s. */
    poly m;
    poly_copy(&m, &ct->v);
    for (int i = 0; i < KLWE; i++) {
        poly t;
        poly_mul(&t, &ct->u[i], &sk->s[i]);
        poly_sub(&m, &m, &t);
    }

    /* coefficient closer to q/2 than to 0 -> bit 1, else bit 0. */
    for (int i = 0; i < N; i++) {
        int c = poly_center(m.coeffs[i]);   // in (-q/2, q/2]
        if (c < 0) c = -c;
        msg->coeffs[i] = (c > Q / 4) ? 1 : 0;
    }
}
