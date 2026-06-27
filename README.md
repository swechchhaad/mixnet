# Lattice-based decryption mixnet

An implementation of the *functional* (non-verifiable) core of the paper
**"Efficient Verifiable Mixnets from Lattices, Revisited"**
(https://eprint.iacr.org/2025/658).

## What is implemented

All algebra and parameters follow the paper (Figure 3): the ring is
`R_q = Z_q[x]/(x^n + 1)` with `q = 3109`, `n = 512` (so `q = 5 (mod 8)` and
`x^n + 1` splits into two degree-`n/2` factors, the structure the proof of
shuffle relies on).

## What is NOT implemented

The **zero-knowledge verifiability layer** is left out. The paper instantiates its `ZK⋆` subprotocol with the LaZer library; we've left this as future work. As a result this mixnet provides privacy through layered encryption but not
verifiability.

> Security note: randomness uses C `rand()` and is **not** cryptographically
> secure. `params.h` uses the paper's parameters but this is just a reference
> implementation, not production code.

## Build & test

```sh
make        
make test 
./mixnet
```
