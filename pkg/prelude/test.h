#ifndef TEST_H
#define TEST_H

#include "prelude.h"
#include "prelude/array.h"
#include "prelude/assert.h"

typedef enum {
    Test_None = 0,
    Test_Property
} TestType;

typedef struct Test {
    TestType type;
    String name;
    union {
        void (*prop)(Int n);
    } func;
} Test;

void Test_NewProperty(Test *t, String *name, void (*prop)(Int n));
void Test_Drop(void *arg);
Bool Test_Run(Test *t);

Define_Array(Test)

typedef struct TestGroup {
    String name;
    TestArray tests;
} TestGroup;

Define_Array(TestGroup)

void TestGroup_New(TestGroup *g, String *name);
void TestGroup_Drop(void *arg);
void TestGroup_Property(TestGroup *g, String *name,
                        void (*prop)(Int n));
Bool TestGroup_Run(TestGroup *g);

typedef struct TestSuite {
    TestGroupArray groups;
} TestSuite;

#define TestSuite_Init (TestSuite){Array_Init(TestGroup)}

void TestSuite_Drop(void *arg);
Int TestSuite_Group(TestSuite *s, String *name);
void TestSuite_Property(TestSuite *s, Int group, String *name,
                        void (*prop)(Int n));
Bool TestSuite_Run(TestSuite *s);

int Test_Main(int argc, const char **argv, TestSuite *suite);

#endif /* TEST_H */
