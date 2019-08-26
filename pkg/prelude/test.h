#ifndef TEST_H
#define TEST_H

#include "prelude.h"
#include "assert.h"

typedef enum {
    Test_None = 0,
    Test_Property
} TestType;

typedef struct Test {
    TestType type;
    const char *name;
    void (*func)(void);
} Test;

typedef struct TestGroup {
    const char *name;
    Test *tests;
} TestGroup;

void TestGroup_Setup(TestGroup *g, String *name);
void TestGroup_Teardown(void *arg);
void TestGroup_Property(TestGroup *g, String *name, void (*prop)(void));

typedef struct TestSuite {
    TestGroup *groups;
} TestSuite;

void TestSuite_Setup(TestSuite *s);
void TestSuite_Teardown(void *arg);
void TestSuite_Add(TestSuite *s, TestGroup *g);

String *Test_GenString(void);

int Test_Main(int argc, const char **argv, TestSuite *suite);

#endif /* TEST_H */
