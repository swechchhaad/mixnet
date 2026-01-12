#ifndef POLY_H
#define POLY_H

#include "params.h"

typedef struct {
    int coeffs[N];
} poly;

void poly_zero(poly *p);
void poly_add(poly *r, const poly *a, const poly *b);
void poly_mod(poly *p);
void poly_sub(poly *r, const poly *a, const poly *b);
void poly_mul(poly *r, const poly *a, const poly *b);


#endif