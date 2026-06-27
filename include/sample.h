#ifndef SAMPLE_H
#define SAMPLE_H

#include "poly.h"

/* these use rand which are not cryptographically secure */

/* seed the underlying PRNG. */
void rng_seed(unsigned int seed);

/* uniform element of R_q: each coefficient uniform in [0, Q). */
void sample_uniform(poly *p);

/* centered binomial B_eta over the ring */
void sample_binomial(poly *p);

/* binary element */
void sample_binary(poly *p);

/* vector helpers */
void vec_uniform(poly *v, int len);
void vec_binomial(poly *v, int len);
void vec_binary(poly *v, int len);

/* coeff wise rounding */
void poly_round(poly *out, const poly *in, int q1, int q2);

#endif
