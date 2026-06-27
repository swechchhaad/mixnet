#include <stdlib.h>
#include "otse.h"
#include "sample.h"
#include "params.h"

static void poly_mul_modm(poly *r, const poly *a, const poly *b, int mod) {
    long tmp[N] = {0};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            long prod = (long)a->coeffs[i] * b->coeffs[j];
            int k = i + j;
            if (k < N) tmp[k] += prod;
            else       tmp[k - N] -= prod;
        }
    }
    for (int i = 0; i < N; i++) {
        long v = tmp[i] % mod;
        if (v < 0) v += mod;
        r->coeffs[i] = (int)v;
    }
}

void otse_setup(otse_pp *pp, int L) {
    pp->L = L;
    pp->rows = KLWE + L;
    pp->H   = malloc((size_t)pp->rows * KLWR * sizeof(poly));
    pp->Hpp = malloc((size_t)L * KLWE * sizeof(poly));

    for (int i = 0; i < pp->rows * KLWR; i++)
        for (int c = 0; c < N; c++)
            pp->H[i].coeffs[c] = rand() % TWO_ZETA;

    for (int i = 0; i < L * KLWE; i++)
        sample_uniform(&pp->Hpp[i]);
}

void otse_free(otse_pp *pp) {
    free(pp->H);
    free(pp->Hpp);
    pp->H = NULL;
    pp->Hpp = NULL;
}

void otse_keygen(poly sbar[KLWR]) {
    vec_binary(sbar, KLWR);
}

void otse_expand(const otse_pp *pp, const poly sbar[KLWR],
                 poly *binom, poly *dagger) {
    for (int i = 0; i < pp->rows; i++) {
        poly acc;
        poly_zero(&acc);
        for (int j = 0; j < KLWR; j++) {
            poly t;
            poly_mul_modm(&t, &pp->H[i * KLWR + j], &sbar[j], TWO_ZETA);
            for (int c = 0; c < N; c++) acc.coeffs[c] += t.coeffs[c];
        }
        for (int c = 0; c < N; c++) acc.coeffs[c] %= TWO_ZETA;

        poly d;
        poly_round(&d, &acc, TWO_ZETA, TWO_2ETA);

        for (int c = 0; c < N; c++) {
            int val = d.coeffs[c];
            int b = 0;
            for (int k = 0; k < ETA; k++)        b += (val >> k) & 1;          // +
            for (int k = ETA; k < 2 * ETA; k++)  b -= (val >> k) & 1;          // -
            binom[i].coeffs[c] = b; 
        }

        if (dagger) {
            for (int k = 0; k < 2 * ETA; k++) {
                poly *dk = &dagger[k * pp->rows + i];
                for (int c = 0; c < N; c++)
                    dk->coeffs[c] = (d.coeffs[c] >> k) & 1;
            }
        }
    }
}

static void otse_pad(poly *a, const otse_pp *pp, const poly *binom) {
    for (int i = 0; i < pp->L; i++) {
        poly_zero(&a[i]);
        for (int j = 0; j < KLWE; j++) {
            poly t;
            poly_mul(&t, &pp->Hpp[i * KLWE + j], &binom[j]);
            poly_add(&a[i], &a[i], &t);
        }
        poly_add(&a[i], &a[i], &binom[KLWE + i]); 
    }
}

void otse_enc(poly *cstar, const otse_pp *pp, const poly sbar[KLWR], const poly *msg) {
    poly *binom = malloc((size_t)pp->rows * sizeof(poly));
    poly *a     = malloc((size_t)pp->L * sizeof(poly));
    otse_expand(pp, sbar, binom, NULL);
    otse_pad(a, pp, binom);
    for (int i = 0; i < pp->L; i++)
        poly_add(&cstar[i], &msg[i], &a[i]);
    free(binom);
    free(a);
}

void otse_dec(poly *msg, const otse_pp *pp, const poly sbar[KLWR], const poly *cstar) {
    poly *binom = malloc((size_t)pp->rows * sizeof(poly));
    poly *a     = malloc((size_t)pp->L * sizeof(poly));
    otse_expand(pp, sbar, binom, NULL);
    otse_pad(a, pp, binom);
    for (int i = 0; i < pp->L; i++)
        poly_sub(&msg[i], &cstar[i], &a[i]);
    free(binom);
    free(a);
}
