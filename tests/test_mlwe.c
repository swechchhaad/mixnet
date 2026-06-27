#include <stdio.h>
#include "mlwe.h"
#include "sample.h"

int main(void) {
    rng_seed(42);

    mlwe_pk pk;
    mlwe_sk sk;
    mlwe_keygen(&pk, &sk);

    int trials = 200, fails = 0;
    for (int t = 0; t < trials; t++) {
        poly m, rec;
        sample_binary(&m);              // random binary message

        mlwe_ct ct;
        mlwe_encrypt(&ct, &pk, &m);
        mlwe_decrypt(&rec, &sk, &ct);

        if (!poly_equal(&m, &rec)) fails++;
    }

    printf("MLPKE decrypt: %d/%d correct\n", trials - fails, trials);
    return fails ? 1 : 0;
}
