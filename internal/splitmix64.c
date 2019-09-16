/* Adapted from http://prng.di.unimi.it/splitmix64.c:

    + Splitmix64_Next

   Public Domain (CC0) 2015 by Sebastiano Vigna
 */

#include "internal/rng.h"

void Splitmix64_Seed(Word64 *state, Int seed) {
    state[0] = (Word64)seed; // TODO: handle NA seed
}

Word64 Splitmix64_Next(Word64 *state) {
    Word64 z = (state[0] += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

Float Splitmix64_Uniform(Word64 *state) {
    Word64 x = Splitmix64_Next(state);
    return uniformFromWord64(x);
}
