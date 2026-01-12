#include "otse.h"

void expand_seed(poly *mask, const int seed[K]) {
    for (int i = 0; i < N; i++) {
        int acc = 0;
        for (int j = 0; j < K; j++) {
            acc += seed[j] * (i + 1 + j);
        }
        acc %= Q;
        if (acc < 0) acc += Q;

        mask->coeffs[i] = (acc * P) / Q;
    }
}