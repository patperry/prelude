#include <stdlib.h>

#include "prelude/test.h"

void TestSuite_Setup(TestSuite *s) {
    (void)s;
    TODO();
}

void TestSuite_Teardown(void *arg) {
    (void)arg;
    TODO();
}

void TestSuite_Add(TestSuite *s, TestGroup *g) {
    (void)s;
    (void)g;
    TODO();
}

void TestGroup_Setup(TestGroup *g, String *name) {
    (void)g;
    (void)name;
    TODO();
}

void TestGroup_Teardown(void *arg) {
    (void)arg;
    TODO();
}

void TestGroup_Property(TestGroup *g, String *name, void (*prop)(void)) {
    (void)g;
    (void)name;
    (void)prop;
    TODO();
}

String *Test_GenString(void)
{
    TODO();
    return NULL;
}

int Test_Main(int argc, const char **argv, TestSuite *suite) {
    (void)argc;
    (void)argv;
    (void)suite;
    TODO();
    return EXIT_SUCCESS;
}
