#ifndef PARAMS_H
#define PARAMS_H

#define N      512      // ring dimension n (power of 2)
#define Q      3109     // prime modulus q, q = 5 (mod 8)

#define KLWE   2        // module rank for MLWE
#define KLWR   1        // seed length in ring elements
#define KSIS   2        // MSIS rank for Ajtai commitments

#define ETA    2        // centered binomial parameter (eta)
#define ZETA   6        // small-ring exponent for LWR (zeta), zeta > 2*eta

/* Derived moduli used by OTSE (Figure 2). */
#define TWO_ZETA   (1 << ZETA)        // 2^zeta   = 64   (small LWR modulus)
#define TWO_2ETA   (1 << (2 * ETA))   // 2^{2eta} = 16   (rounding target)

#endif
