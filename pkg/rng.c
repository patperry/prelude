#include "prelude/rng.h"


void Splitmix64_Seed(Splitmix64 *rng, Int seed) {
    rng->state = (Word64)seed; // TODO: handle NA seed
}

Word64 Splitmix64_Next(Splitmix64 *rng) {
    /* Public Domain (CC0) 2015 by Sebastiano Vigna
     * http://prng.di.unimi.it/splitmix64.c
     */
    Word64 z = (rng->state += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

Float Splitmix64_Uniform(Splitmix64 *rng) {
    (void)rng; // TODO
    return 0;
}

void Xoshiro256plus_Seed(Xoshiro256plus *rng, Int seed) {
    Splitmix64 s;
    Splitmix64_Seed(&s, seed);

    Int i, n = Xoshiro256plus_StateLen;
    for (i = 0; i < n; i++) {
        rng->state[i] = Splitmix64_Next(&s);
    }
}

Float Rng_Uniform(Rng *rng) {
    (void)rng; // TODO
    return 0;
}
