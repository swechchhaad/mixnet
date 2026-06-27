#include <stdio.h>
#include <stdlib.h>
#include "hpke.h"
#include "sample.h"

int main(void) {
    rng_seed(123);

    int L = 4;                 // arbitrary message length in ring elements
    otse_pp pp;
    otse_setup(&pp, L);

    mlwe_pk pk;
    mlwe_sk sk;
    hpke_keygen(&pk, &sk);

    int trials = 100, fails = 0;
    poly *m   = malloc((size_t)L * sizeof(poly));
    poly *rec = malloc((size_t)L * sizeof(poly));

    for (int t = 0; t < trials; t++) {
        for (int i = 0; i < L; i++) sample_uniform(&m[i]);  // arbitrary ring elements

        hpke_ct ct;
        hpke_enc(&ct, &pp, &pk, m, L);
        hpke_dec(rec, &pp, &sk, &ct);
        hpke_ct_free(&ct);

        for (int i = 0; i < L; i++)
            if (!poly_equal(&m[i], &rec[i])) { fails++; break; }
    }

    printf("HPKE (L=%d): %d/%d correct\n", L, trials - fails, trials);

    free(m); free(rec);
    otse_free(&pp);
    return fails ? 1 : 0;
}
