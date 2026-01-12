#include <stdlib.h>

#include "phase1.h"
#include "otse.h"
#include "mlwe.h"
#include "poly.h"
#include "params.h"

static void sample_binary_seed(int seed[K]) {
    for (int i = 0; i < K; i++) {
        seed[i] = rand() % 2;
    }
}
static void seed_to_poly(poly *p, const int seed[K]) {
    poly_zero(p);
    for (int i = 0; i < K; i++) {
        p->coeffs[i] = seed[i] ? (Q / 2) : 0;
    }
}

static void poly_to_seed(int seed[K], const poly *p) {
    for (int i = 0; i < K; i++) {
        seed[i] = (p->coeffs[i] > Q / 4) ? 1 : 0;
    }
}

void phase1_encrypt(ciphertext *ct,
                    const poly *message,
                    const mlwe_pk *pk) {

    int seed[K];
    poly seed_poly;
    poly mask;

    sample_binary_seed(seed);

    seed_to_poly(&seed_poly, seed);
    mlwe_encrypt(&ct->u, &ct->v, pk, &seed_poly);

    expand_seed(&mask, seed);

    poly_add(&ct->c_star, message, &mask);
}
void phase1_decrypt(poly *message,
                    const ciphertext *ct,
                    const mlwe_sk *sk) {

    poly seed_poly;
    int seed[K];
    poly mask;

    mlwe_decrypt(&seed_poly, sk, &ct->u, &ct->v);
    poly_to_seed(seed, &seed_poly);
    expand_seed(&mask, seed);
    poly_sub(message, &ct->c_star, &mask);
}