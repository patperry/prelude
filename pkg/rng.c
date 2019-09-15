
/* Public Domain (CC0) 2015 by Sebastiano Vigna
   http://prng.di.unimi.it/splitmix64.c

   next (Splitmix64_Next) */

/* Public Domain (CC0) 2018 by David Blackman and Sebastiano Vigna
   http://prng.di.unimi.it/xoshiro256plus.c

   rotl      (ROTL)
   next      (Xoshiro256plus_Next)
   jump      (Xoshiro256plus_Jump)
   long_jump (Xoshiro256plus_LongJump)
 */

#include "prelude/rng.h"

void Splitmix64_Seed(Splitmix64 *rng, Int seed) {
    rng->state[0] = (Word64)seed; // TODO: handle NA seed
}


Word64 Splitmix64_Next(Splitmix64 *rng) {
    Word64 z = (rng->state[0] += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

void Xoshiro256plus_Seed(Xoshiro256plus *rng, Int seed) {
    Splitmix64 s;
    Splitmix64_Seed(&s, seed);

    Int i, n = Xoshiro256plus_StateLen;
    for (i = 0; i < n; i++) {
        rng->state[i] = Splitmix64_Next(&s);
    }
}

#define ROTL(x, k) ((x) << (k)) | ((x) >> (64 - (k)));

Word64 Xoshiro256plus_Next(Xoshiro256plus *rng) {
    Word64 *s = rng->state;
    Word64 result = s[0] + s[3];

	Word64 t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = ROTL(s[3], 45);

	return result;
}

void Xoshiro256plus_Jump(Xoshiro256plus *rng) {
    static const Word64 Jump[] = {
        0x180ec6d33cfd0aba,
        0xd5a61266f0c9392c,
        0xa9582618e03fc9aa,
        0x39abdc4529b1661c
    };

    Word64 *s = rng->state;

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
            Xoshiro256plus_Next(rng);
        }
    }

    s[0] = s0;
    s[1] = s1;
    s[2] = s2;
    s[3] = s3;
}

void Xoshiro256plus_LongJump(Xoshiro256plus *rng) {
    static const Word64 LongJump[] = {
        0x76e15d3efefdcbbf,
        0xc5004e441c522fb3,
        0x77710069854ee241,
        0x39109bb02acbe635
    };

    Word64 *s = rng->state;

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
            Xoshiro256plus_Next(rng);
        }
    }

    s[0] = s0;
    s[1] = s1;
    s[2] = s2;
    s[3] = s3;
}


/* Recommended at http://prng.di.unimi.it/ */
#define uniform(x) \
    ((x) >> 11) * (1.0 / (W64(1) << 53))

Float Splitmix64_Uniform(Splitmix64 *rng) {
    Word64 x = Splitmix64_Next(rng);
    return uniform(x);
}

Float Xoshiro256plus_Uniform(Xoshiro256plus *rng) {
    Word64 x = Xoshiro256plus_Next(rng);
    return uniform(x);
}
