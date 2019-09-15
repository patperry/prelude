#ifndef PRELUDE_RNG
#define PRELUDE_RNG

#include "prelude.h"
#include "prelude/array.h"

#define Splitmix64_StateLen 1
typedef struct Splitmix64 {
    Word64 state[Splitmix64_StateLen];
} Splitmix64;

#define Splitmix64_Init (Splitmix64){{0}}

void Splitmix64_Seed(Splitmix64 *rng, Int seed);
Word64 Splitmix64_Next(Splitmix64 *rng);
Float Splitmix64_Uniform(Splitmix64 *rng);

#define Xoshiro256plus_StateLen 4
typedef struct Xoshiro256plus {
    Word64 state[Xoshiro256plus_StateLen];
} Xoshiro256plus;

#define Xoshiro256plus_Init (Xoshiro256plus){{ \
    0xe220a8397b1dcdaf, \
    0x6e789e6aa1b965f4, \
    0x06c45d188009454f, \
    0xf88bb8a8724c81ec \
}}

void Xoshiro256plus_Seed(Xoshiro256plus *rng, Int seed);
Word64 Xoshiro256plus_Next(Xoshiro256plus *rng);
Float Xoshiro256plus_Uniform(Xoshiro256plus *rng);

/* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */
void Xoshiro256plus_Jump(Xoshiro256plus *rng);

/* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */
void Xoshiro256plus_LongJump(Xoshiro256plus *rng);

typedef struct RngType {
    Int state_len;
    void (*seed)(Word64 *state, Int seed);
    Float (*uniform)(Word64 *state);
} RngType;

extern RngType *Rng_Splitmix64;
extern RngType *Rng_Xoshiro256plus;

typedef struct Rng {
    RngType *type;
    Word64Array state;
} Rng;

void Rng_New(Rng *rng, RngType *t); /* seeds with time */
void Rng_FromCopy(Rng *rng, Rng *other);
void Rng_Drop(void *arg);

void Rng_Seed(Rng *rng, Int seed);
Float Rng_Uniform(Rng *rng);

typedef struct RngMakerType {
    Int state_len;
    void (*seed)(Word64 *state, Int seed);
    void (*make)(Rng *rng, Word64 *state);
} RngMakerType;

extern RngMakerType *RngMaker_Xoshiro256plus;

typedef struct RngMaker {
    RngMakerType *type;
    Word64Array state;
} RngMaker;

void RngMaker_New(RngMaker *m, RngMakerType *t);
void RngMaker_Drop(void *arg);
void RngMaker_Seed(RngMaker *m, Int seed);
void RngMaker_Make(RngMaker *m, Rng *rng);


void Random_Uniform(void);
void Random_Seed(Int seed);
Rng *Random_Rng(void);

void Random_SeedMaker(Int seed);
RngMaker *Random_RngMaker(void);

#endif /* PRELUDE_RNG */
