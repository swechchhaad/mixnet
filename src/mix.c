#include <stdlib.h>
#include <string.h>

#include "mix.h"
#include "phase1.h"
#include "poly.h"
#include "params.h"

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

void mix_decrypt_shuffle(ciphertext *out,
                         const ciphertext *in,
                         size_t n,
                         const mlwe_sk *sk) {

    size_t *perm = malloc(n * sizeof(size_t));
    if (!perm) return;

    random_permutation(perm, n);

    for (size_t i = 0; i < n; i++) {
        size_t j = perm[i];

        poly decrypted;

        phase1_decrypt(&decrypted, &in[j], sk);

        out[i].u = in[j].u; 
        out[i].v = in[j].v;
        out[i].c_star = decrypted;
    }

    free(perm);
}
void mixnet_run(ciphertext *cts,
                size_t n,
                size_t num_servers,
                const mlwe_sk *sks) {

    ciphertext *tmp = malloc(n * sizeof(ciphertext));
    if (!tmp) return;

    for (size_t k = 0; k < num_servers; k++) {
        mix_decrypt_shuffle(tmp, cts, n, &sks[k]);

        for (size_t i = 0; i < n; i++) {
            cts[i] = tmp[i];
        }
    }

    free(tmp);
}