#include <stdio.h>
#include <stdlib.h>
#include "params.h"
#include "poly.h"
#include "mlwe.h"
#include "mix.h"
#include "sample.h"

int main(void) {
    rng_seed(42);

    const int rho = 4;      // mixing servers
    const size_t n = 5;     // users

    mlwe_pk *pks = malloc(rho * sizeof(mlwe_pk));
    mlwe_sk *sks = malloc(rho * sizeof(mlwe_sk));
    for (int j = 0; j < rho; j++) mlwe_keygen(&pks[j], &sks[j]);
    otse_pp *pps = mix_setup(rho);

    poly *msgs = malloc(n * sizeof(poly));
    flatct *list = malloc(n * sizeof(flatct));
    for (size_t i = 0; i < n; i++) {
        poly_zero(&msgs[i]);
        msgs[i].coeffs[0] = (int)(i + 1);
        mix_encrypt(&list[i], &msgs[i], pks, pps, rho);
    }

    printf("layered ciphertext: %d ring elements (expected %d)\n",
           list[0].len, rho * SEED_RING + 1);

    mix_run(list, n, sks, pps, rho);

    printf("output plaintexts (constant coeff, shuffled order):\n  ");
    for (size_t i = 0; i < n; i++) printf("%d ", list[i].data[0].coeffs[0]);
    printf("\n");

    int *seen = calloc(n + 1, sizeof(int));
    int ok = 1;
    for (size_t i = 0; i < n; i++) {
        int val = list[i].data[0].coeffs[0];
        int rest_zero = 1;
        for (int c = 1; c < N; c++) if (list[i].data[0].coeffs[c] != 0) rest_zero = 0;
        if (val < 1 || val > (int)n || seen[val] || !rest_zero) ok = 0;
        else seen[val] = 1;
    }
    printf("permutation of inputs: %s\n", ok ? "yes" : "NO");

    for (size_t i = 0; i < n; i++) flatct_free(&list[i]);
    for (int j = 0; j < rho; j++) otse_free(&pps[j]);
    free(pks); free(sks); free(pps); free(msgs); free(list); free(seen);
    return ok ? 0 : 1;
}
