#include <stdio.h>
#include <stdlib.h>
#include "otse.h"
#include "sample.h"

static int run_L(int L, int trials) {
    otse_pp pp;
    otse_setup(&pp, L);

    int fails = 0;
    poly *m   = malloc((size_t)L * sizeof(poly));
    poly *c   = malloc((size_t)L * sizeof(poly));
    poly *rec = malloc((size_t)L * sizeof(poly));

    for (int t = 0; t < trials; t++) {
        poly sbar[KLWR];
        otse_keygen(sbar);
        for (int i = 0; i < L; i++) sample_uniform(&m[i]);  // arbitrary ring elements

        otse_enc(c, &pp, sbar, m);
        otse_dec(rec, &pp, sbar, c);

        for (int i = 0; i < L; i++)
            if (!poly_equal(&m[i], &rec[i])) { fails++; break; }
    }

    /* sanity check: ciphertext should differ from message (masking happened). */
    int masked = 0;
    for (int i = 0; i < L; i++) if (!poly_equal(&m[i], &c[i])) masked = 1;

    printf("OTSE L=%-2d : %d/%d exact, masking=%s\n",
           L, trials - fails, trials, masked ? "yes" : "NO");

    free(m); free(c); free(rec);
    otse_free(&pp);
    return fails;
}

int main(void) {
    rng_seed(7);
    int fails = 0;
    fails += run_L(1, 100);
    fails += run_L(4, 100);
    fails += run_L(7, 50);
    return fails ? 1 : 0;
}
