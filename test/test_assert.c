#include "prelude/test.h"

static void panic(void *arg) {
    (void)arg;
    Panic(S("%s"), S("panic!"));
}

static void unit_panic(void) {
    Assert_Panic(panic, NULL);
}

static void catch_panic(void *arg) {
    (void)arg;
    Error err = Error_Init;
    Try(panic, NULL, &err);
    Assert(Error_Some(&err));
    Error_Drop(&err);
}

static void unit_notPanic(void) {
    Assert_NotPanic(catch_panic, NULL);
}

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
    TestSuite_AddUnit(&s, g, S("panic"), unit_panic);
    TestSuite_AddUnit(&s, g, S("not panic"), unit_notPanic);
    TestSuite_AddUnit(&s, g, S("string eq"), unit_stringEq);

    int ret = Test_Main(argc, argv, &s);
    Close();
    Finalize();
    return ret;
}
