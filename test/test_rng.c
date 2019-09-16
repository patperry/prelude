#include "prelude/rng.h"
#include "prelude/test.h"

static void case_xoshiro256plusInit(void) {
    Open();

    Rng x, y;

    Rng_New(&x, Rng_Xoshiro256plus);
    Defer(Rng_Drop, &x);

    Rng_New(&y, Rng_Xoshiro256plus);
    Defer(Rng_Drop, &y);
    Rng_Seed(&y, Int_None);

    Assert(x.state.len == y.state.len);

    Int i, n = x.state.len;
    for (i = 0; i < n; i++) {
        Assert(x.state.items[i] == y.state.items[i]);
    }

    Close();
}

static void case_xoshiro256plusSeed(void) {
    Open();

    Rng s, xo;
    Int seed = 0x0fedc031337abcde;

    Rng_New(&s, Rng_Splitmix64);
    Defer(Rng_Drop, &s);

    Rng_New(&xo, Rng_Xoshiro256plus);
    Defer(Rng_Drop, &xo);

    Rng_Seed(&s, seed);
    Rng_Seed(&xo, seed);

    Int i, n = xo.state.len;
    for (i = 0; i < n; i++) {
        Word64 x = Rng_Next(&s);
        Assert(x == xo.state.items[i]);
    }

    Close();
}

static void case_globalInit(void) {
    Open();

    Rng r;

    Rng_New(&r, Rng_None);
    Defer(Rng_Drop, &r);

    Int i, n = 100;
    for (i = 0; i < n; i++) {
        Float x = Random_Uniform();
        Float y = Rng_Uniform(&r);
        Assert(x == y);
    }

    n = 100;
    for (i = 0; i < n; i++) {
        Word64 x = Random_Next();
        Word64 y = Rng_Next(&r);
        Assert(x == y);
    }

    Close();
}

int main(int argc, const char **argv) {
    Initialize();
    Open();

    TestSuite s = TestSuite_Init;
    Defer(TestSuite_Drop, &s);

    TestSuite_AddCase(&s, S("xoshiro256+ init"), case_xoshiro256plusInit);
    TestSuite_AddCase(&s, S("xoshiro256+ seed"), case_xoshiro256plusSeed);
    TestSuite_AddCase(&s, S("global init"), case_globalInit);

    int ret = Test_Main(argc, argv, &s);
    Close();
    Finalize();
    return ret;
}
