#ifndef INTERNAL_RNG_H
#define INTERNAL_RNG_H

#include "prelude.h"

/* Recommended at http://prng.di.unimi.it/ */
#define uniformFromWord64(x) \
    ((x) >> 11) * (1.0 / (W64(1) << 53))

#define Splitmix64_StateLen 1
void Splitmix64_Seed(Word64 *state, Int seed);
Word64 Splitmix64_Next(Word64 *state);
Float Splitmix64_Uniform(Word64 *state);

#define Xoshiro256plus_StateLen 4
void Xoshiro256plus_Seed(Word64 *state, Int seed);
Word64 Xoshiro256plus_Next(Word64 *state);
Float Xoshiro256plus_Uniform(Word64 *state);

/* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */
void Xoshiro256plus_Jump(Word64 *state);

/* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */
void Xoshiro256plus_LongJump(Word64 *state);

#endif /* INTERNAL_RNG_H */
