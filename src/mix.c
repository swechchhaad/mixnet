#include <stdlib.h>
#include "mix.h"
#include "hpke.h"
#include "sample.h"
#include "params.h"

void flatct_free(flatct *c) {
    free(c->data);
    c->data = NULL;
    c->len = 0;
}

int mix_layer_msglen(int rho, int layer) {
    /* Innermost layer (layer = rho-1) carries 1 ring element; each outer layer
     * wraps the inner ciphertext, adding SEED_RING ring elements. */
    return 1 + (rho - 1 - layer) * SEED_RING;
}

otse_pp *mix_setup(int rho) {
    otse_pp *pps = malloc((size_t)rho * sizeof(otse_pp));
    for (int layer = 0; layer < rho; layer++)
        otse_setup(&pps[layer], mix_layer_msglen(rho, layer));
    return pps;
}

/* Flatten an HPKE ciphertext into ring elements:
 *   [ cseed_0.u[0..kLWE-1], cseed_0.v, ... cseed_{kLWR-1}.* , cstar[0..L-1] ]. */
static void serialize_hpke(poly *out, const hpke_ct *ct) {
    int idx = 0;
    for (int j = 0; j < KLWR; j++) {
        for (int i = 0; i < KLWE; i++) poly_copy(&out[idx++], &ct->cseed[j].u[i]);
        poly_copy(&out[idx++], &ct->cseed[j].v);
    }
    for (int i = 0; i < ct->L; i++) poly_copy(&out[idx++], &ct->cstar[i]);
}

/* Inverse of serialize_hpke. cstar points into the caller's buffer (not owned). */
static void deserialize_hpke(hpke_ct *ct, poly *in, int total_len) {
    int idx = 0;
    for (int j = 0; j < KLWR; j++) {
        for (int i = 0; i < KLWE; i++) poly_copy(&ct->cseed[j].u[i], &in[idx++]);
        poly_copy(&ct->cseed[j].v, &in[idx++]);
    }
    ct->L = total_len - SEED_RING;
    ct->cstar = &in[idx];   // borrow; not freed by hpke_ct_free below
}

void mix_encrypt(flatct *out, const poly *m,
                 const mlwe_pk *pks, const otse_pp *pps, int rho) {
    /* Start with the plaintext as a 1-element flat ciphertext. */
    flatct cur;
    cur.len = 1;
    cur.data = malloc(sizeof(poly));
    poly_copy(&cur.data[0], m);

    for (int layer = rho - 1; layer >= 0; layer--) {
        hpke_ct ct;
        hpke_enc(&ct, &pps[layer], &pks[layer], cur.data, cur.len);

        flatct next;
        next.len = SEED_RING + ct.L;
        next.data = malloc((size_t)next.len * sizeof(poly));
        serialize_hpke(next.data, &ct);

        hpke_ct_free(&ct);
        flatct_free(&cur);
        cur = next;
    }
    *out = cur;
}

static void random_permutation(size_t *perm, size_t n) {
    for (size_t i = 0; i < n; i++) perm[i] = i;
    for (size_t i = n; i > 1; i--) {
        size_t j = (size_t)(rand() % (int)i);
        size_t tmp = perm[i - 1];
        perm[i - 1] = perm[j];
        perm[j] = tmp;
    }
}

/* One server: remove a layer from every ciphertext, then shuffle. */
static void server_step(flatct *list, size_t n, const mlwe_sk *sk,
                        const otse_pp *pp) {
    flatct *dec = malloc(n * sizeof(flatct));

    for (size_t i = 0; i < n; i++) {
        hpke_ct ct;
        deserialize_hpke(&ct, list[i].data, list[i].len);  // borrows list[i].data

        dec[i].len = ct.L;
        dec[i].data = malloc((size_t)ct.L * sizeof(poly));
        hpke_dec(dec[i].data, pp, sk, &ct);

        flatct_free(&list[i]);   // frees the buffer ct borrowed from
    }

    size_t *perm = malloc(n * sizeof(size_t));
    random_permutation(perm, n);
    for (size_t i = 0; i < n; i++) list[i] = dec[perm[i]];

    free(perm);
    free(dec);
}

void mix_run(flatct *list, size_t n, const mlwe_sk *sks,
             const otse_pp *pps, int rho) {
    for (int layer = 0; layer < rho; layer++)
        server_step(list, n, &sks[layer], &pps[layer]);
}
