#include <stdlib.h>

#include "prelude/test.h"

Implement_Array(Test)
Implement_Array(TestGroup)

void TestSuite_Drop(void *arg) {
    TestSuite *s = arg;
    Int n = s->groups.len;
    while (n-- > 0) {
        TestGroup_Drop(&s->groups.items[n]);
    }
    TestGroupArray_Drop(&s->groups);
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
    TestGroup_New(&s->groups.items[n], name);
    s->groups.len = n + 1;
    return n;
}

void TestSuite_Property(TestSuite *s, Int group, String *name,
                        void (*prop)(Int n)) {
    TestGroup_Property(&s->groups.items[group], name, prop);
}

void TestGroup_New(TestGroup *g, String *name) {
    *g = (TestGroup){};
    Open();
    String_FromCopy(&g->name, name);
    Trap(String_Drop, &g->name);
    g->tests = Array_Init(Test);
    Close();
}

void TestGroup_Drop(void *arg) {
    TestGroup *g = arg;
    Int n = g->tests.len;
    while (n-- > 0) {
        Test_Drop(&g->tests.items[n]);
    }
    TestArray_Drop(&g->tests);
    String_Drop(&g->name);
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

void TestGroup_Property(TestGroup *g, String *name, void (*prop)(Int n)) {
    TestArray_Grow(&g->tests, 1);
    Int n = g->tests.len;
    Test_NewProperty(&g->tests.items[n], name, prop);
    g->tests.len = n + 1;
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

Bool Test_Run(Test *t) {
    Debug(S("running test \"%s\""), &t->name);
    switch (t->type) {
    case Test_None:
        break;
    case Test_Property:
        (t->func.prop)(10); // TODO fix n;
        break;
    }
    return True;
}

int Test_Main(int argc, const char **argv, TestSuite *suite) {
    (void)argc;
    (void)argv;
    Bool succ = TestSuite_Run(suite);
    return succ == True ? EXIT_SUCCESS : EXIT_FAILURE;
}
