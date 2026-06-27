#ifndef MIX_H
#define MIX_H

#include <stddef.h>
#include "poly.h"
#include "mlwe.h"
#include "otse.h"

/*
 * Decryption mixnet (Sections 2.6, 3.2).
 *
 * Each user layer-encrypts a single ring element m under rho public keys:
 *     c_{i,1} = Enc_pk1(Enc_pk2(... Enc_pkrho(m_i) ...))
 * using HPKE for each layer. The outer ciphertext is stored flat, as a vector
 * of ring elements. Layer j adds kLWR*(kLWE+1) ring elements (the MLPKE
 * encryption of the seed); the OTSE part does not expand. Hence a depth-rho
 * ciphertext has rho*kLWR*(kLWE+1) + 1 ring elements.
 *
 * Server j (0-indexed) holds sk_{j+1}, removes one HPKE layer from every
 * ciphertext, shuffles the resulting (shorter) list, and forwards it. The last
 * server outputs the plaintext ring elements.
 */

/* Ring elements added per encryption layer: c' = kLWR MLPKE ciphertexts. */
#define SEED_RING (KLWR * (KLWE + 1))

/* A flat ciphertext: len ring elements. */
typedef struct {
    poly *data;
    int   len;
} flatct;

void flatct_free(flatct *c);

/* Message length (in ring elements) handled by HPKE at the given layer
 * (0 = outermost). Innermost layer carries a single ring element. */
int mix_layer_msglen(int rho, int layer);

/*
 * Set up shared OTSE public parameters, one per layer (lengths differ).
 * Returns an array of rho otse_pp; free each with otse_free and free the array.
 */
otse_pp *mix_setup(int rho);

/* Layer-encrypt a single ring element m under pks[0..rho-1] (pks[0] outermost). */
void mix_encrypt(flatct *out, const poly *m,
                 const mlwe_pk *pks, const otse_pp *pps, int rho);

/*
 * Run the full mixnet over n ciphertexts in place: for each server, remove one
 * layer from every ciphertext and shuffle. After rho servers, list[i].len == 1
 * and list[i].data[0] is a plaintext (in shuffled order).
 */
void mix_run(flatct *list, size_t n, const mlwe_sk *sks,
             const otse_pp *pps, int rho);

#endif
