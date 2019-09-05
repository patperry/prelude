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

static void case_panic(void) {
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

static void case_notPanic(void) {
    Assert_NotPanic(catch_panic, NULL);
}

static void fail_assert(void *arg) {
    (void)arg;
    Assert(False);
}

static void case_assert(void) {
    Assert_Panic(fail_assert, NULL);
}

static void fail_errorNone(void *arg) {
    (void)arg;
    Error err;
    Error_New(&err, S("error"));
    Assert_ErrorNone(&err);
}

static void case_errorNone(void) {
    Assert_Panic(fail_errorNone, NULL);
}

static void fail_stringEq(void *arg) {
    (void)arg;
    Assert_StringEq(S("a"), S("b"));
}

static void case_stringEq(void) {
    Assert_Panic(fail_stringEq, NULL);
}

static void fail_stringNotEq(void *arg) {
    (void)arg;
    Assert_StringNotEq(S(""), S(""));
}

static void case_stringNotEq(void) {
    Assert_Panic(fail_stringNotEq, NULL);
}

int main(int argc, const char **argv) {
    Initialize();
    Open();

    TestSuite s = TestSuite_Init;
    Defer(TestSuite_Drop, &s);

    TestSuite_AddCase(&s, S("panic"), case_panic);
    TestSuite_AddCase(&s, S("not panic"), case_notPanic);
    TestSuite_AddCase(&s, S("assert"), case_assert);
    TestSuite_AddCase(&s, S("error none"), case_errorNone);
    TestSuite_AddCase(&s, S("string eq"), case_stringEq);
    TestSuite_AddCase(&s, S("string not eq"), case_stringNotEq);

    int ret = Test_Main(argc, argv, &s);
    Close();
    Finalize();
    return ret;
}
