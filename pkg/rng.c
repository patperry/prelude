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

static RngMakerType RngMaker_Xoshiro256plus_ = {
    Xoshiro256plus_StateLen,
    Xoshiro256plus_Seed,
    Xoshiro256plus_Make
};

RngType *Rng_Xoshiro256plus = &Rng_Xoshiro256plus_;
RngMakerType *RngMaker_Xoshiro256plus = &RngMaker_Xoshiro256plus_;


void Rng_New(Rng *rng, RngType *t) {
    rng->type = t ? t : Rng_Xoshiro256plus;
    rng->state = Array_Init(Word64);
    Word64Array_Grow(&rng->state, rng->type->state_len);
    Rng_Seed(rng, Int_None);
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

void RngMaker_New(RngMaker *m, RngMakerType *t) {
    m->type = t ? t : RngMaker_Xoshiro256plus;
    m->state = Array_Init(Word64);
    Word64Array_Grow(&m->state, m->type->state_len);
    RngMaker_Seed(m, Int_None);
}

void RngMaker_Drop(void *arg) {
    RngMaker *m = arg;
    Word64Array_Drop(&m->state);
}

void RngMaker_Seed(RngMaker *m, Int seed) {
    (m->type->seed)(m->state.items, seed);
}

void RngMaker_Make(RngMaker *m, Rng *rng) {
    (m->type->make)(m->state.items, rng);
}
