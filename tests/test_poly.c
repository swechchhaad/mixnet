#include <stdio.h>
#include "poly.h"

static int failures = 0;

static void check(int cond, const char *name) {
    if (cond) {
        printf("  [ok]   %s\n", name);
    } else {
        printf("  [FAIL] %s\n", name);
        failures++;
    }
}

int main(void) {
    printf("Negacyclic ring R_q = Z_%d[x]/(x^%d + 1)\n", Q, N);

    /* x * x^{n-1} = x^n = -1 mod (x^n + 1). */
    poly x, xn1, prod, minus_one;
    poly_zero(&x);       x.coeffs[1] = 1;          // x
    poly_zero(&xn1);     xn1.coeffs[N - 1] = 1;    // x^{n-1}
    poly_mul(&prod, &x, &xn1);
    poly_zero(&minus_one); minus_one.coeffs[0] = Q - 1;  // -1 mod q
    check(poly_equal(&prod, &minus_one), "x * x^{n-1} == -1");

    /* commutativity on a random-ish pair */
    poly a, b, ab, ba;
    for (int i = 0; i < N; i++) { a.coeffs[i] = (i * 7 + 3) % Q; b.coeffs[i] = (i * 5 + 1) % Q; }
    poly_mul(&ab, &a, &b);
    poly_mul(&ba, &b, &a);
    check(poly_equal(&ab, &ba), "a*b == b*a");

    /* multiplicative identity */
    poly one, a1;
    poly_zero(&one); one.coeffs[0] = 1;
    poly_mul(&a1, &a, &one);
    poly_mod(&a);
    check(poly_equal(&a1, &a), "a * 1 == a");

    /* distributivity */
    poly c, bc, a_bc, ac, ab_ac;
    for (int i = 0; i < N; i++) c.coeffs[i] = (i * 11 + 2) % Q;
    poly_add(&bc, &b, &c);
    poly_mul(&a_bc, &a, &bc);
    poly_mul(&ab, &a, &b);
    poly_mul(&ac, &a, &c);
    poly_add(&ab_ac, &ab, &ac);
    check(poly_equal(&a_bc, &ab_ac), "a*(b+c) == a*b + a*c");

    /* center maps q-1 to -1. */
    check(poly_center(Q - 1) == -1, "center(q-1) == -1");
    check(poly_center(1) == 1, "center(1) == 1");

    printf(failures ? "\n%d failure(s)\n" : "\nall passed\n", failures);
    return failures ? 1 : 0;
}
