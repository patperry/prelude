#ifndef TEST_H
#define TEST_H

typedef enum {
    Test_None = 0,
    Test_Prop
} TestType;

typedef struct Test {
    TestType type;
    const char *name;
    Bool (*func)(void);
} Test;

typedef struct TestGroup {
    const char *name;
    Test *tests;
} TestGroup;

String *Test_String(void);

int Test_Main(int argc, const char **argv, TestGroup *tests);


#endif /* TEST_H */
