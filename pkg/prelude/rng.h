#ifndef PRELUDE_RNG
#define PRELUDE_RNG

#include "prelude.h"
#include "prelude/array.h"

typedef struct RngType {
    Int state_len;
    void (*seed)(Word64 *state, Int seed);
    Word64 (*next)(Word64 *state);
    Float (*uniform)(Word64 *state);
} RngType;

#define Rng_None (RngType *)NULL
extern RngType *Rng_Splitmix64;
extern RngType *Rng_Xoshiro256plus;

typedef struct Rng {
    RngType *type;
    Word64Array state;
} Rng;

void Rng_New(Rng *rng, RngType *t);
/* void Rng_FromCopy(Rng *rng, Rng *other); */
void Rng_Drop(void *arg);

void Rng_Seed(Rng *rng, Int seed);
Word64 Rng_Next(Rng *rng);
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
