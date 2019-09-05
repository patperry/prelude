#include "prelude/rng.h"
#include "prelude/test.h"

static void case_sameSeed(void) {
    Rng r1, r2;
    Rng_New(&r1, 31337);
    Defer(Rng_Drop, &r1);
    Rng_New(&r2, 31337);
    Defer(Rng_Drop, &r2);

    Int i, n = 1000;
    for (i = 0; i < n; i++) {
        Int x1 = Rng_Next(&r1);
        Int x2 = Rng_Next(&r2);
        Assert_IntEq(x1, x2);
    }
}

int main(int argc, const char **argv) {
    Initialize();
    Open();

    TestSuite s = TestSuite_Init;
    Defer(TestSuite_Drop, &s);

    TestSuite_AddCase(&s, S("same seed"), case_sameSeed);

    int ret = Test_Main(argc, argv, &s);
    Close();
    Finalize();
    return ret;
}
