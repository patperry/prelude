#ifndef PRELUDE_ASSERT_H
#define PRELUDE_ASSERT_H

#include "prelude.h"

#define Assert_Panic(func, arg) \
    Assert_Panic_(func, arg, S(#func), S(__func__), S(__FILE__), __LINE__)
void Assert_Panic_(void (*func)(void *arg), void *arg,
                   String *expr, String *funcs, String *file, Int line);

#define Assert_NotPanic(func, arg) \
    Assert_NotPanic_(func, arg, S(#func), S(__func__), S(__FILE__), __LINE__)
void Assert_NotPanic_(void (*func)(void *arg), void *arg,
                      String *expr, String *funcs, String *file, Int line);

#define Assert_ErrorNone(err) \
    Assert_ErrorNone_(err, S(__func__), S(__FILE__), __LINE__)
void Assert_ErrorNone_(Error *err, String *func, String *file, Int line);

#define Assert_IntEq(x1, x2) \
    Assert_IntEq_(x1, x2, S(__func__), S(__FILE__), __LINE__)
void Assert_IntEq_(Int x1, Int x2, String *func, String *file, Int line);

#define Assert_IntNotEq(x1, x2) \
    Assert_IntNotEq_(x1, x2, S(__func__), S(__FILE__), __LINE__)
void Assert_IntNotEq_(Int x1, Int x2, String *func, String *file, Int line);

#define Assert_StringEq(s1, s2) \
    Assert_StringEq_(s1, s2, S(__func__), S(__FILE__), __LINE__)
void Assert_StringEq_(String *s1, String *s2, String *func, String *file,
                      Int line);

#define Assert_StringNotEq(s1, s2) \
    Assert_StringNotEq_(s1, s2, S(__func__), S(__FILE__), __LINE__)
void Assert_StringNotEq_(String *s1, String *s2, String *func, String *file,
                         Int line);

#endif /* PRELUDE_ASSERT_H */
