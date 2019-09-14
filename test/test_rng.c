#include "prelude/rng.h"
#include "prelude/test.h"

static void case_xoshiro256plusInit(void) {
    Splitmix64 s = Splitmix64_Init;
    Xoshiro256plus xo = Xoshiro256plus_Init;

    int i, n = Xoshiro256plus_StateLen;
    for (i = 0; i < n; i++) {
        Word64 x = Splitmix64_Next(&s);
        Assert(x == xo.state[i]);
    }
}

static void case_xoshiro256plusSeed(void) {
    Splitmix64 s = Splitmix64_Init;
    Xoshiro256plus xo = Xoshiro256plus_Init;
    Int seed = 0x0fedc031337abcde;

    Xoshiro256plus_Seed(&xo, seed);
    Splitmix64_Seed(&s, seed);

    Int i, n = Xoshiro256plus_StateLen;
    for (i = 0; i < n; i++) {
        Word64 x = Splitmix64_Next(&s);
        Assert(x == xo.state[i]);
    }
}

int main(int argc, const char **argv) {
    Initialize();
    Open();

    TestSuite s = TestSuite_Init;
    Defer(TestSuite_Drop, &s);

    TestSuite_AddCase(&s, S("xoshiro256+ init"), case_xoshiro256plusInit);
    TestSuite_AddCase(&s, S("xoshiro256+ seed"), case_xoshiro256plusSeed);

    int ret = Test_Main(argc, argv, &s);
    Close();
    Finalize();
    return ret;
}
