#include <stdlib.h>

#include "prelude/test.h"

Implement_Array(Test)

void TestSuite_Drop(void *arg) {
    TestSuite *s = arg;
    Int n = s->tests.len;
    while (n-- > 0) {
        Test_Drop(&s->tests.items[n]);
    }
    TestArray_Drop(&s->tests);
}

Bool TestSuite_Run(TestSuite *s) {
    Bool succ = True;
    Int i, n = s->tests.len;
    for (i = 0; i < n; i++) {
        Bool t = Test_Run(&s->tests.items[i]);
        if (t == False) {
            succ = False;
        }
    }
    return succ;
}

static int testSuite_GrowTests(TestSuite *s) {
    TestArray_Grow(&s->tests, 1);
    Int n = s->tests.len;
    s->tests.items[n] = Test_Init;
    s->tests.len = n + 1;
    return n;
}

void TestSuite_AddCase(TestSuite *s, String *name, void (*tcase)(void)) {
    Int i = testSuite_GrowTests(s);
    Test_NewCase(&s->tests.items[i], name, tcase);
}

void TestSuite_AddProperty(TestSuite *s, String *name, void (*prop)(Int n)) {
    Int i = testSuite_GrowTests(s);
    Test_NewProperty(&s->tests.items[i], name, prop);
}

void Test_NewCase(Test *t, String *name, void (*tcase)(void)) {
    t->type = Test_Case;
    String_FromCopy(&t->name, name);
    t->func.tcase = tcase;
}

void Test_NewProperty(Test *t, String *name, void (*prop)(Int n)) {
    t->type = Test_Property;
    String_FromCopy(&t->name, name);
    t->func.prop = prop;
}

void Test_Drop(void *arg) {
    Test *t = arg;
    String_Drop(&t->name);
}

static void tcase_run(void *arg) {
    Test *t = arg;
    t->func.tcase();
}

static Bool caseRun(Test *t) {
    Open();
    Assert(t->type == Test_Case);

    Error err = Error_Init;
    Try(tcase_run, t, &err);
    Bool succ = True;

    if (Error_Some(&err)) {
        Defer(Error_Drop, &err);
        Info(S("%s"), &err.string);
        succ = False;
    }

    Close();
    return succ;
}

Bool Test_Run(Test *t) {
    Info(S("running test \"%s\""), &t->name);
    Bool ret = True;
    switch (t->type) {
    case Test_None:
        break;
    case Test_Case:
        ret = caseRun(t);
        break;
    case Test_Property:
        (t->func.prop)(10); // TODO fix n;
        break;
    }
    return ret;
}

int Test_Main(int argc, const char **argv, TestSuite *suite) {
    (void)argc;
    (void)argv;
    Bool succ = TestSuite_Run(suite);
    return succ == True ? EXIT_SUCCESS : EXIT_FAILURE;
}
