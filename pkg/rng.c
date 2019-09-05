#include "prelude/rng.h"

void Rng_New(Rng *r, Int seed) {
    (void)r;
    (void)seed;
}

void Rng_Drop(void *arg) {
    Rng *r = arg;
    (void)r;
}

void Rng_Split(Rng *r, Rng *child) {
    (void)r;
    (void)child;
}

Int Rng_Next(Rng *r) {
    (void)r;
    return 0;
}
