#include <stdlib.h>
#include <string.h>

#include "mix.h"
#include "poly.h"
#include "params.h"

static void sample_small_poly(poly *r) {
    for (int i = 0; i < N; i++) {
        r->coeffs[i] = (rand() % (2 * ETA + 1)) - ETA;
    }
}

static void random_permutation(size_t *perm, size_t n) {
    for (size_t i = 0; i < n; i++) {
        perm[i] = i;
    }

    // Fisher–Yates shuffle
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        size_t tmp = perm[i];
        perm[i] = perm[j];
        perm[j] = tmp;
    }
}

void mix_shuffle(ciphertext *out,
                 const ciphertext *in,
                 size_t n,
                 const mlwe_pk *pk) {

    size_t *perm = malloc(n * sizeof(size_t));
    if (!perm) return;

    random_permutation(perm, n);

    for (size_t i = 0; i < n; i++) {
        size_t j = perm[i];

        poly r;
        poly ar, br;

        sample_small_poly(&r);
        poly_mul(&ar, &pk->a, &r);
        poly_mul(&br, &pk->b, &r);
        poly_add(&out[i].u, &in[j].u, &ar);
        poly_add(&out[i].v, &in[j].v, &br);

        out[i].c_star = in[j].c_star;
    }

    free(perm);
}
void mixnet_run(ciphertext *cts,
                size_t n,
                size_t num_servers,
                const mlwe_pk *pk) {

    ciphertext *tmp = malloc(n * sizeof(ciphertext));
    if (!tmp) return;

    for (size_t k = 0; k < num_servers; k++) {
        mix_shuffle(tmp, cts, n, pk);
        for (size_t i = 0; i < n; i++) {
            cts[i] = tmp[i];
        }
    }

    free(tmp);
}