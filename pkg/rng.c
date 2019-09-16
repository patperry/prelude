#include "prelude/rng.h"
#include "internal/rng.h"

static RngType Rng_Splitmix64_ = {
    Splitmix64_StateLen,
    Splitmix64_Seed,
    Splitmix64_Next,
    Splitmix64_Uniform
};

RngType *Rng_Splitmix64 = &Rng_Splitmix64_;

static RngType Rng_Xoshiro256plus_ = {
    Xoshiro256plus_StateLen,
    Xoshiro256plus_Seed,
    Xoshiro256plus_Next,
    Xoshiro256plus_Uniform
};

RngType *Rng_Xoshiro256plus = &Rng_Xoshiro256plus_;

void Rng_New(Rng *rng, RngType *t) {
    rng->type = t ? t : Rng_Xoshiro256plus;
    rng->state = Array_Init(Word64);
    Word64Array_Grow(&rng->state, t->state_len);
    Rng_Seed(rng, 0);
}

void Rng_Drop(void *arg) {
    Rng *rng = arg;
    Word64Array_Drop(&rng->state);
}

void Rng_Seed(Rng *rng, Int seed) {
    (rng->type->seed)(rng->state.items, seed);
}

Word64 Rng_Next(Rng *rng) {
    return (rng->type->next)(rng->state.items);
}

Float Rng_Uniform(Rng *rng) {
    return (rng->type->uniform)(rng->state.items);
}
