/* Adapted from http://prng.di.unimi.it/xoshiro256plus.c

    + rotl
    + Xoshiro256plus_Next
    + Xoshiro256plus_Jump
    + Xoshiro256plus_LongJump
  
   Public Domain (CC0) 2018 by David Blackman and Sebastiano Vigna
 */

#include "prelude/rng.h"
#include "internal/rng.h"

void Xoshiro256plus_Seed(Word64 *state, Int seed) {
    Word64 s[Splitmix64_StateLen];
    Splitmix64_Seed(s, seed);

    Int i, n = Xoshiro256plus_StateLen;
    for (i = 0; i < n; i++) {
        state[i] = Splitmix64_Next(s);
    }
}

#define rotl(x, k) ((x) << (k)) | ((x) >> (64 - (k)));

Word64 Xoshiro256plus_Next(Word64 *state) {
    Word64 *s = state;
    Word64 result = s[0] + s[3];

	Word64 t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 45);

	return result;
}

void Xoshiro256plus_Jump(Word64 *state) {
    static const Word64 Jump[] = {
        0x180ec6d33cfd0aba,
        0xd5a61266f0c9392c,
        0xa9582618e03fc9aa,
        0x39abdc4529b1661c
    };

    Word64 *s = state;

    Word64 s0 = 0;
    Word64 s1 = 0;
    Word64 s2 = 0;
    Word64 s3 = 0;
    Int i, b;

    for(i = 0; i < 4; i++) {
        for(b = 0; b < 64; b++) {
            if (Jump[i] & W64(1) << b) {
                s0 ^= s[0];
                s1 ^= s[1];
                s2 ^= s[2];
                s3 ^= s[3];
            }
            Xoshiro256plus_Next(s);
        }
    }

    s[0] = s0;
    s[1] = s1;
    s[2] = s2;
    s[3] = s3;
}

void Xoshiro256plus_LongJump(Word64 *state) {
    static const Word64 LongJump[] = {
        0x76e15d3efefdcbbf,
        0xc5004e441c522fb3,
        0x77710069854ee241,
        0x39109bb02acbe635
    };

    Word64 *s = state;

    Word64 s0 = 0;
    Word64 s1 = 0;
    Word64 s2 = 0;
    Word64 s3 = 0;
    Int i, b;

    for(i = 0; i < 4; i++) {
        for(b = 0; b < 64; b++) {
            if (LongJump[i] & W64(1) << b) {
                s0 ^= s[0];
                s1 ^= s[1];
                s2 ^= s[2];
                s3 ^= s[3];
            }
            Xoshiro256plus_Next(s);
        }
    }

    s[0] = s0;
    s[1] = s1;
    s[2] = s2;
    s[3] = s3;
}

Float Xoshiro256plus_Uniform(Word64 *state) {
    Word64 x = Xoshiro256plus_Next(state);
    return uniformFromWord64(x);
}

void Xoshiro256plus_Make(Word64 *state, Rng *rng) {
    Rng_New(rng, Rng_Xoshiro256plus);
    Int i, n = Xoshiro256plus_StateLen;
    for (i = 0; i < n; i++) {
        rng->state.items[i] = state[i];
    }
    Xoshiro256plus_Jump(state);
}
