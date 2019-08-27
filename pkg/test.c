#include <stdlib.h>

#include "prelude/test.h"

Implement_Array(Test)
Implement_Array(TestGroup)

void TestSuite_Setup(TestSuite *s) {
    *s = (TestSuite){};
    TestGroupArray_Setup(&s->groups);
}

void TestSuite_Teardown(void *arg) {
    TestSuite *s = arg;
    Int n = s->groups.len;
    while (n-- > 0) {
        TestGroup_Teardown(&s->groups.items[n]);
    }
    TestGroupArray_Teardown(&s->groups);
}

Bool TestSuite_Run(TestSuite *s) {
    Bool succ = True;
    Int i, n = s->groups.len;
    for (i = 0; i < n; i++) {
        Bool g = TestGroup_Run(&s->groups.items[i]);
        if (g == False) {
            succ = False;
        }
    }
    return succ;
}

Int TestSuite_Group(TestSuite *s, String *name) {
    Int i, n = s->groups.len;
    for (i = 0; i < n; i++) {
        if (String_Eq(&s->groups.items[i].name, name)) {
            return i;
        }
    }
    TestGroupArray_Grow(&s->groups, 1);
    TestGroup_Setup(&s->groups.items[n], name);
    s->groups.len = n + 1;
    return n;
}

void TestSuite_Property(TestSuite *s, Int group, String *name,
                        void (*prop)(void)) {
    TestGroup_Property(&s->groups.items[group], name, prop);
}

void TestGroup_Setup(TestGroup *g, String *name) {
    *g = (TestGroup){};
    Open();
    String_SetupWithCopy(&g->name, name);
    Trap(String_Teardown, &g->name);
    TestArray_Setup(&g->tests);
    Close();
}

void TestGroup_Teardown(void *arg) {
    TestGroup *g = arg;
    Int n = g->tests.len;
    while (n-- > 0) {
        Test_Teardown(&g->tests.items[n]);
    }
    TestArray_Teardown(&g->tests);
    String_Teardown(&g->name);
}

Bool TestGroup_Run(TestGroup *g) {
   Bool succ = True;
    Int i, n = g->tests.len;
    for (i = 0; i < n; i++) {
        Bool t = Test_Run(&g->tests.items[i]);
        if (t == False) {
            succ = False;
        }
    }
    return succ;
}

static void TestGroup_Add(TestGroup *g, TestType type, String *name,
                          void (*func)(void)) {
    TestArray_Grow(&g->tests, 1);
    Int n = g->tests.len;
    Test_Setup(&g->tests.items[n], type, name, func);
    g->tests.len = n + 1;
}

void TestGroup_Property(TestGroup *g, String *name, void (*prop)(void)) {
    TestGroup_Add(g, Test_Property, name, prop);
}

void Test_Setup(Test *t, TestType type, String *name, void (*func)(void)) {
    *t = (Test){};
    String_SetupWithCopy(&t->name, name);
    t->type = type;
    t->func = func;
}

void Test_Teardown(void *arg) {
    Test *t = arg;
    String_Teardown(&t->name);
}

Bool Test_Run(Test *t) {
    Debug(S("running test \"%s\""), &t->name);
    (t->func)();
    return True;
}

String *Test_GenString(void) {
    // TODO: better
    return S("hello world");
}

int Test_Main(int argc, const char **argv, TestSuite *suite) {
    (void)argc;
    (void)argv;
    Bool succ = TestSuite_Run(suite);
    return succ == True ? EXIT_SUCCESS : EXIT_FAILURE;
}
