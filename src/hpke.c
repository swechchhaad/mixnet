#include <stdlib.h>
#include "hpke.h"
#include "params.h"

void hpke_keygen(mlwe_pk *pk, mlwe_sk *sk) {
    mlwe_keygen(pk, sk);
}

void hpke_enc(hpke_ct *ct, const otse_pp *pp, const mlwe_pk *pk,
              const poly *msg, int L) {
    ct->L = L;
    ct->cstar = malloc((size_t)L * sizeof(poly));

    poly sbar[KLWR];
    otse_keygen(sbar);

    for (int j = 0; j < KLWR; j++)
        mlwe_encrypt(&ct->cseed[j], pk, &sbar[j]);

    otse_enc(ct->cstar, pp, sbar, msg);
}

void hpke_dec(poly *msg, const otse_pp *pp, const mlwe_sk *sk, const hpke_ct *ct) {
    poly sbar[KLWR];
    for (int j = 0; j < KLWR; j++)
        mlwe_decrypt(&sbar[j], sk, &ct->cseed[j]);

    otse_dec(msg, pp, sbar, ct->cstar);
}

void hpke_ct_free(hpke_ct *ct) {
    free(ct->cstar);
    ct->cstar = NULL;
}
