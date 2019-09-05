#ifndef PRELUDE_RNG
#define PRELUDE_RNG

#include "prelude.h"

typedef struct Rng {
    Int state;
} Rng;

void Rng_New(Rng *r, Int seed);
void Rng_Drop(void *arg);
void Rng_Split(Rng *r, Rng *parent);
Int Rng_Next(Rng *r);

#endif /* PRELUDE_RNG */
