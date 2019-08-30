#ifndef TEST_H
#define TEST_H

#include "prelude.h"
#include "prelude/array.h"
#include "prelude/assert.h"

typedef enum {
    Test_None = 0,
    Test_Property,
    Test_Unit
} TestType;

typedef struct Test {
    TestType type;
    String name;
    union {
        void (*prop)(Int n);
        void (*unit)(void);
    } func;
} Test;

#define Test_Init (Test){Test_None, String_Init, {NULL}}

Define_Array(Test)

void Test_NewProperty(Test *t, String *name, void (*prop)(Int n));
void Test_NewUnit(Test *t, String *name, void (*unit)(void));
void Test_Drop(void *arg);
Bool Test_Run(Test *t);


typedef struct TestGroup {
    String name;
    TestArray tests;
} TestGroup;

#define TestGroup_Init (TestGroup){String_Init, Array_Init(Test)}

Define_Array(TestGroup)

void TestGroup_New(TestGroup *g, String *name);
void TestGroup_Drop(void *arg);
void TestGroup_AddUnit(TestGroup *g, String *name,
                       void (*unit)(void));
void TestGroup_AddProperty(TestGroup *g, String *name,
                           void (*prop)(Int n));
Bool TestGroup_Run(TestGroup *g);

typedef struct TestSuite {
    TestGroupArray groups;
} TestSuite;

#define TestSuite_Init (TestSuite){Array_Init(TestGroup)}

void TestSuite_Drop(void *arg);
Int TestSuite_Group(TestSuite *s, String *name);
void TestSuite_AddUnit(TestSuite *s, Int group, String *name,
                       void (*unit)(void));
void TestSuite_AddProperty(TestSuite *s, Int group, String *name,
                           void (*prop)(Int n));
Bool TestSuite_Run(TestSuite *s);


int Test_Main(int argc, const char **argv, TestSuite *suite);

#endif /* TEST_H */
