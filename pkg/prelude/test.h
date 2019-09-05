#ifndef PRELUDE_TEST_H
#define PRELUDE_TEST_H

#include "prelude.h"
#include "prelude/array.h"
#include "prelude/assert.h"

typedef enum {
    Test_None = 0,
    Test_Case,
    Test_Property
} TestType;

typedef struct Test {
    TestType type;
    String name;
    union {
        void (*tcase)(void);
        void (*prop)(Int n);
    } func;
} Test;

#define Test_Init (Test){Test_None, String_Init, {NULL}}

Define_Array(Test)

void Test_NewCase(Test *t, String *name, void (*tcase)(void));
void Test_NewProperty(Test *t, String *name, void (*prop)(Int n));
void Test_Drop(void *arg);
Bool Test_Run(Test *t);


typedef struct TestSuite {
    TestArray tests;
} TestSuite;

#define TestSuite_Init (TestSuite){Array_Init(Test)}

void TestSuite_Drop(void *arg);

void TestSuite_AddCase(TestSuite *s, String *name, void (*tcase)(void));
void TestSuite_AddProperty(TestSuite *s, String *name, void (*prop)(Int n));
Bool TestSuite_Run(TestSuite *s);

int Test_Main(int argc, const char **argv, TestSuite *suite);

#endif /* PRELUDE_TEST_H */
