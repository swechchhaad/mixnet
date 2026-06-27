#ifndef OTSE_H
#define OTSE_H

#include "poly.h"


typedef struct {
    int  L; 
    int  rows; 
    poly *H;  
    poly *Hpp;   
} otse_pp;

void otse_setup(otse_pp *pp, int L);
void otse_free(otse_pp *pp);

void otse_keygen(poly sbar[KLWR]);

void otse_enc(poly *cstar, const otse_pp *pp, const poly sbar[KLWR], const poly *msg);
void otse_dec(poly *msg, const otse_pp *pp, const poly sbar[KLWR], const poly *cstar);

void otse_expand(const otse_pp *pp, const poly sbar[KLWR],
                 poly *binom, poly *dagger);

#endif
