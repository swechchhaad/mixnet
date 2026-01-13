#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "params.h"
#include "poly.h"
#include "mlwe.h"
#include "phase1.h"
#include "mix.h"


int main(void) {
    srand(42);

    mlwe_pk pk;
    mlwe_sk sk;
    mlwe_keygen(&pk, &sk);

    const size_t n = 3;

    poly messages[n];
    for (size_t i = 0; i < n; i++) {
        poly_zero(&messages[i]);
        messages[i].coeffs[0] = (int)(i + 1); 
    }

    ciphertext ct_in[n];
    for (size_t i = 0; i < n; i++) {
        phase1_encrypt(&ct_in[i], &messages[i], &pk);
    }

    ciphertext ct_out[n];
    mix_shuffle(ct_out, ct_in, n, &pk);

    printf("Decrypted messages after shuffle:\n");

    for (size_t i = 0; i < n; i++) {
        poly rec;
        phase1_decrypt(&rec, &ct_out[i], &sk);
        printf("  %d\n", rec.coeffs[0]);
    }

    return 0;
}