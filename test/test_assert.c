#include "prelude/test.h"

static void fail_stringEq(void *arg) {
    (void)arg;
    Assert_StringEq(S("a"), S("b"));
}

static void unit_stringEq(void) {
    Assert_Panic(fail_stringEq, NULL);
}

int main(int argc, const char **argv) {
    Initialize();
    Open();

    TestSuite s = TestSuite_Init;
    Defer(TestSuite_Drop, &s);

    Int g = TestSuite_Group(&s, S("assert"));
    TestSuite_AddUnit(&s, g, S("string eq"), unit_stringEq);

    int ret = Test_Main(argc, argv, &s);
    Close();
    Finalize();
    return ret;
}
