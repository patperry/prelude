#include "prelude/test.h"

static void panic1(void *arg) {
    (void)arg;
    Panic(S("panic!"));
}

static void noop(void *arg) {
    (void)arg;
}

static void panic2(void *arg) {
    (void)arg;
    Assert_Panic(noop, arg);
}

static void panic3(void *arg) {
    (void)arg;
    Assert_NotPanic(panic1, arg);
}

static void unit_panic(void) {
    Assert_Panic(panic1, NULL);
    Assert_Panic(panic2, NULL);
    Assert_Panic(panic3, NULL);
}

static void catch_panic(void *arg) {
    (void)arg;
    Error err = Error_Init;
    Try(panic1, NULL, &err);
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
